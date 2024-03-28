#include "../include/ServerConnection.h"
#include <QByteArray>
#include <QBuffer>
#include <QString>

namespace Client {
    ServerConnection::ServerConnection(const std::string& ip, uint32_t port) : ip_(ip), port_(port)
    {}

    ServerConnection::~ServerConnection() {
        // Kill the connection
        Kill();
    }

   void ServerConnection::MakeRequest(QImage& imageData, const uint8_t imageId) {

        // Reset each time received images
        receivedImages_.clear();

        // Remove the limits on sizes
        grpc::ChannelArguments ch_args;
        ch_args.SetMaxReceiveMessageSize(-1); 
        ch_args.SetMaxSendMessageSize(-1);
        channel_ = grpc::CreateCustomChannel(ip_ + ":" + std::to_string(port_), grpc::InsecureChannelCredentials(), ch_args);
        // Create a stub to manage the connection
        stub_ = ImageService::NewStub(channel_);

        // Create a request message
        ImageRequest request;
        request.set_image_id(std::to_string(imageId));

        // Convert QImage to QByteArray with JPEG format
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);

        if (!imageData.isNull() && imageData.save(&buffer, "PNG")) {
            // Set the image data in the request
            request.set_image_data(byteArray.toBase64().toStdString());

            // Create a response message
            ImageResponse response;

            // Create a new context for this RPC
            grpc::ClientContext context;

            // Call the RPC method
            grpc::Status status = stub_->ProcessImage(&context, request, &response);

            // Handle the response or error as needed
            if (status.ok()) {
                // Handle response
                std::cout << "Received set ID: " << response.set_id() << std::endl;
                for (const auto& image_data : response.image_data()) {
                    // Convert QByteArray to QImage
                    QByteArray byteArray = QByteArray::fromBase64(QString::fromStdString(image_data).toUtf8());
                    QImage image = QImage::fromData(byteArray, "PNG");
                    receivedImages_.push_back(image);
                }
            } else {
                // Handle error
                std::cerr << "Error: " << status.error_message() << std::endl;
            }
        } else {
            // Handle error saving QImage
            std::cerr << "Error: Failed to save QImage to QByteArray" << std::endl;
        }
    }

}
