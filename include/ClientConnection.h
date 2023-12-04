#ifndef SERVER_CLIENTCONNECTION_H
#define SERVER_CLIENTCONNECTION_H

#include <grpc++/grpc++.h>
#include "../build/generated/proto.pb.h"
#include "../build/generated/proto.grpc.pb.h"
#include <string>

class ImageServiceImpl final : public ImageService::Service {
public:
    grpc::Status ProcessImage(grpc::ServerContext* context, const ImageRequest* request, ImageResponse* response) override {
        std::cout << "Received image with ID: " << request->image_id() << std::endl;

        response->set_set_id("123");
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