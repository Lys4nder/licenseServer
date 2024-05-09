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

class ImageServiceImpl final : public ImageService::Service {
private:
public:
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
        Server::ImageProcessing imageProcessing;
        imageProcessing.setFolderPath(cfImporter.GetImageFolderPath());
        imageProcessing.setQueryImagePath("output_image.png");
        imageProcessing.QueryImage();

        std::vector<std::pair<std::string, double>> similarityScores = imageProcessing.GetSimilarityScores();
        std::vector<QImage> qImages;
        qImages.reserve(similarityScores.size());

        for (const auto &similarityScore : similarityScores) {
            if (similarityScore.second < 0.15) {
                QImage qImage;
                qImage.load(QString::fromStdString(similarityScore.first));
                qImages.push_back(std::move(qImage));
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
        return grpc::Status::OK;
    }
};

class StatusServiceImpl final : public StatusService::Service {
public:
    grpc::Status GetStatus(grpc::ServerContext* context, const PercentageRequest* request, grpc::ServerWriter<PercentageResponse>* writer) override {
        std::cout << "[Server]: Status request function called" << std::endl;

        // Assuming some logic here to determine when to send responses
        int i = 0;
        while (i <= 100){
            PercentageResponse response;
            response.set_percentage(i); // Use the shared statusPercentage_
            writer->Write(response); // Send the response to the client
            i++;
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Sleep for 100ms
        }
        i = 0; // Reset the statusPercentage_ to 0
        return grpc::Status::OK;
    }

};

void RunServer() {
    Server::ConfigImporter cfImporter;
    try {
        cfImporter.ReadConfig();
    }
    catch (const std::exception& e) {
        std::cout << e.what();
    }
    std::string server_address(cfImporter.GetAddr() + ':' + cfImporter.GetPort());

    ImageServiceImpl imageService;
    StatusServiceImpl statusService;


    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&imageService);
    builder.RegisterService(&statusService);
    builder.SetMaxReceiveMessageSize(-1);
    builder.SetMaxSendMessageSize(-1);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "[Server]: Started listening on " << server_address << std::endl;
    server->Wait();
}

#endif //SERVER_CLIENTCONNECTION_H
