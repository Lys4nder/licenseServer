#ifndef SERVER_CLIENTCONNECTION_H
#define SERVER_CLIENTCONNECTION_H

#include <grpc++/grpc++.h>
#include "../build/generated/proto.pb.h"
#include "../build/generated/proto.grpc.pb.h"
#include <string>
#include <QByteArray>
#include <QImage>
#include "../include/ImageConvertor.h"
#include "../include/ImageProcessing.h"
#include <QString>
#include <QByteArray>
#include <QBuffer>
#include "../include/ConfigImporter.h"
#include <thread>
#include <fstream>
#include <sstream>

class ImageServiceImpl final : public ImageService::Service {
public:
    ImageServiceImpl(std::shared_ptr<int> statusPercentage) : statusPercentage_(statusPercentage) {}
    grpc::Status ProcessImage(grpc::ServerContext* context, const ImageRequest* request, ImageResponse* response) override {
        QByteArray imageData = QByteArray::fromBase64(request->image_data().c_str());
        // Check if image data is valid
        if (imageData.isNull()) {
            return grpc::Status::CANCELLED;
        } else {
            std::cout << "[Server]: Image data is valid" << std::endl;
        }

        // Convert QImage to cv::Mat
        QImage image;
        image.loadFromData(reinterpret_cast<const uchar*>(imageData.constData()), imageData.size());

        // Save QImage
        QString savePath = "output_image.png";
        image.save(savePath, "PNG");

        Server::ImageConvertor imageConvertor;
        cv::Mat cvImage = imageConvertor.ConvertToCvMat(image);

        Server::ConfigImporter cfImporter;
        try {
            cfImporter.ReadConfig();
        }
        catch (const std::exception& e) {
            std::cout << e.what();
        }
        Server::ImageProcessing imageProcessing(statusPercentage_);
        imageProcessing.setFolderPath(cfImporter.GetImageFolderPath());
        imageProcessing.setQueryImagePath("output_image.png");
        imageProcessing.QueryImage();

        std::vector<std::pair<std::string, double>> similarityScores = imageProcessing.GetSimilarityScores();
        std::vector<QImage> qImages;
        qImages.reserve(similarityScores.size());
        QImage histImage;
        histImage.load("../build/RGB_Histogram.png");
        qImages.push_back(std::move(histImage));

        for (const auto &similarityScore : similarityScores) {
            if (similarityScore.second < 0.15) {
                QImage qImage;
                qImage.load(QString::fromStdString(similarityScore.first));
                qImages.push_back(std::move(qImage));
            } else {
                break;
            }
        }

        int counter = 0;
        for (const auto &QI : qImages) {
                response->set_set_id(std::to_string(counter++));
                QByteArray byteArray;
                QBuffer buffer(&byteArray);
                buffer.open(QIODevice::WriteOnly);
                QI.save(&buffer, "PNG");
                response->add_image_data(byteArray.toBase64().toStdString());
        }
        std::cout << "[Server]: Image processing and sending finished" << std::endl;
        return grpc::Status::OK;
    }
    private:
        std::shared_ptr<int> statusPercentage_;
};

class StatusServiceImpl final : public StatusService::Service {
public:
    StatusServiceImpl(std::shared_ptr<int> statusPercentage) : statusPercentage_(statusPercentage) {}
    grpc::Status GetStatus(grpc::ServerContext* context, const PercentageRequest* request, grpc::ServerWriter<PercentageResponse>* writer) override {
        std::cout << "[Server]: Status request function called" << std::endl;

        {
            std::lock_guard<std::mutex> lock(mutex_);
            *statusPercentage_ = 0;
        }
        while (*statusPercentage_ <= 100){
            {
                {
                    std::lock_guard<std::mutex> lock(mutex_);
                    PercentageResponse response;
                    response.set_percentage(*statusPercentage_); // Use the shared statusPercentage_
                    writer->Write(response); // Send the response to the client
                }

                if (*statusPercentage_ == 100) {
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }
        
        std::cout << "[Server]: Status request function finished" << std::endl;
        PercentageResponse response;
        response.set_percentage(100); // failsafe
        writer->Write(response); // Send the response to the client
        return grpc::Status::OK;
    }
private:
    std::shared_ptr<int> statusPercentage_;
    std::mutex mutex_;
};

std::string ReadFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file) {
        throw std::runtime_error("Unable to open file: " + filepath);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}


void RunServer() {
    Server::ConfigImporter cfImporter;
    try {
        cfImporter.ReadConfig();
    }
    catch (const std::exception& e) {
        std::cout << e.what();
    }
    std::string server_address(cfImporter.GetAddr() + ':' + cfImporter.GetPort());

    std::shared_ptr<int> statusPercentage = std::make_shared<int>(0);
    ImageServiceImpl imageService(statusPercentage);
    StatusServiceImpl statusService(statusPercentage);

    // Load server credentials
    std::string server_key = ReadFile("../ssl_files/server_key.pem");
    std::string server_chain = ReadFile("../ssl_files/server_chain.pem");
    std::string ca_cert = ReadFile("../ssl_files/ca_cert.pem");

    grpc::SslServerCredentialsOptions::PemKeyCertPair key_cert_pair = { server_key, server_chain };
    grpc::SslServerCredentialsOptions ssl_opts;
    ssl_opts.pem_key_cert_pairs.push_back(key_cert_pair);
    ssl_opts.pem_root_certs = ca_cert;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::SslServerCredentials(ssl_opts));
    builder.RegisterService(&imageService);
    builder.RegisterService(&statusService);
    builder.SetMaxReceiveMessageSize(-1);
    builder.SetMaxSendMessageSize(-1);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "[Server]: Started listening on " << server_address << std::endl;
    server->Wait();
}


#endif //SERVER_CLIENTCONNECTION_H
