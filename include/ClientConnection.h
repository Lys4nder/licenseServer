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
        QString filePath = "output_image2.jpg";
        image.save(filePath);

        Server::ImageConvertor imageConvertor;
        cv::Mat cvImage = imageConvertor.ConvertToCvMat(image);

        // TODO: Analyze here
        // process image
        Server::ImageProcessing imageProcessing;



        //TODO: Send Back data
        response->set_set_id("2");
        response->add_image_data("Processed Image Data 1");
        response->add_image_data("Processed Image Data 2");

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