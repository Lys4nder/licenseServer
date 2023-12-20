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


class ImageServiceImpl final : public ImageService::Service {
public:
    grpc::Status ProcessImage(grpc::ServerContext* context, const ImageRequest* request, ImageResponse* response) override {
        QByteArray imageData = QByteArray::fromBase64(request->image_data().c_str());

        // Check if image data is valid
        if (imageData.isNull()) {
            return grpc::Status::CANCELLED;
        } else {
            std::cout << "Image data is valid" << std::endl;
        }

        // Convert QImage to cv::Mat
        QImage image;
        image.loadFromData(reinterpret_cast<const uchar*>(imageData.constData()), imageData.size());

        // Save the image
        QString filePath = "output_image.jpg";
        image.save(filePath);

        Server::ImageConvertor imageConvertor;
        cv::Mat cvImage = imageConvertor.ConvertToCvMat(image);

        // TODO: Do a config.cfg file for folder path, query image path, etc and for ip and port
        // process image
        Server::ImageProcessing imageProcessing;
        imageProcessing.setFolderPath("/Users/lysanderpitu/Desktop/images/");
        imageProcessing.setQueryImagePath("output_image.jpg");
        imageProcessing.QueryImage();

        std::vector<std::pair<std::string, double>> similarityScores = imageProcessing.GetSimilarityScores();
        std::vector<QImage> qImages;

        for (auto const &similarityScore : similarityScores) {
            if (similarityScore.second < 0.15) {
                QImage qImage;
                qImage.load(QString::fromStdString(similarityScore.first));
                qImages.push_back(qImage);
            }
        }

        int counter = 0;
        for (const auto &QI : qImages) {
                response->set_set_id(std::to_string(counter++));
                QByteArray byteArray;
                QBuffer buffer(&byteArray);
                buffer.open(QIODevice::WriteOnly);
                QI.save(&buffer, "JPG");
                response->add_image_data(byteArray.toBase64().toStdString());
        }
    
        return grpc::Status::OK;
    }

};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    ImageServiceImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}



#endif //SERVER_CLIENTCONNECTION_H