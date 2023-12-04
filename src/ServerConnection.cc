#include "../include/ServerConnection.h"

namespace Client {
    ServerConnection::ServerConnection(const std::string& ip, uint32_t port) {
        // Create a channel to connect to the server
        channel_ = grpc::CreateChannel(ip + ":" + std::to_string(port), grpc::InsecureChannelCredentials());
        // Create a stub to manage the connection
        stub_ = ImageService::NewStub(channel_);
    }

    ServerConnection::~ServerConnection() {
        // Kill the connection
        Kill();
    }

    void ServerConnection::ProcessImage(const std::string& imageId, const std::vector<uint8_t>& imageData) {
        // Create a request message
        ImageRequest request;
        request.set_image_id(imageId);
        request.set_image_data(imageData.data(), imageData.size());

        // Create a response message
        ImageResponse response;

        // Call the RPC method
        grpc::Status status = stub_->ProcessImage(&context_, request, &response);

        // Handle the response or error as needed
        if (status.ok()) {
            // Handle response
            std::cout << "Received set ID: " << response.set_id() << std::endl;
            for (const auto& image_data : response.image_data()) {
                std::cout << "Received image data: " << image_data << std::endl;
            }
        } else {
            // Handle error
            std::cerr << "Error: " << status.error_message() << std::endl;
        }
    }
}
