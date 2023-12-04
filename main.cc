#include "include/MainWindow.h"
#include "include/ServerConnection.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    //QApplication app(argc, argv);

    //ImageWindow window;
    //window.show();

    //return app.exec();

    std::string server_address("localhost:50051");
    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());

    // Create a stub for the ImageService
    std::unique_ptr<ImageService::Stub> stub = ImageService::NewStub(channel);

    // Create an ImageRequest with image data
    ImageRequest request;
    request.set_image_id("123");  // Set a unique identifier
    // Set the image data (replace this with your actual image data)
    request.set_image_data("Your Image Data");

    // Create a variable to store the response
    ImageResponse response;

    // Make the gRPC call to the server
    grpc::ClientContext context;
    grpc::Status status = stub->ProcessImage(&context, request, &response);

    // Check for errors in the gRPC call
    if (status.ok()) {
        // Process the received images
        std::cout << "Received set ID: " << response.set_id() << std::endl;
        for (const auto& image_data : response.image_data()) {
            std::cout << "Received image data: " << image_data << std::endl;
        }
    } else {
        std::cerr << "Error in gRPC call: " << status.error_message() << std::endl;
    }

}
