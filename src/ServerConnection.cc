#include "../include/ServerConnection.h"
#include <QByteArray>
#include <QBuffer>
#include <QString>
#include "../include/ConfigImporter.h"

namespace Client {
    ServerConnection::ServerConnection()
    {
        Client::ConfigImporter cfImporter;
        try {
            cfImporter.ReadConfig();
        }
        catch (const std::exception& e) {
            std::cout << e.what();
        }
        ip_ = cfImporter.GetAddr();
        port_ = cfImporter.GetPort();
    }

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
        channel_ = grpc::CreateCustomChannel(ip_ + ":" + port_, grpc::InsecureChannelCredentials(), ch_args);
        
         std::unique_ptr<ImageService::Stub> imageStub(ImageService::NewStub(channel_));
    // Create a request message for the image service
    ImageRequest request;
    request.set_image_id(std::to_string(imageId));
    // Convert QImage to QByteArray with JPEG format
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    // Check if image data is valid
    if (!imageData.isNull() && imageData.save(&buffer, "PNG")) {
        // Set the image data in the request
        request.set_image_data(byteArray.toBase64().toStdString());
        // Create a response message for the image service
        ImageResponse response;
        // Create a new context for the image service RPC
        grpc::ClientContext imageContext;

        // Create a stub for the status service
        std::unique_ptr<StatusService::Stub> statusStub(StatusService::NewStub(channel_));
        // Create a request message for the status service
        PercentageRequest percentageRequest;
        percentageRequest.set_request(1);
        // Create a new context for the status service RPC
        grpc::ClientContext statusContext;

        // Create threads for the services
        std::thread imageThread([this, &imageStub, &request, &response, &imageContext]() {
            // Call the RPC method for the image service
            grpc::Status imageStatus = imageStub->ProcessImage(&imageContext, request, &response);
            // Handle the response or error as needed for the image service
            if (imageStatus.ok()) {
                // Handle response for the image service
                std::cout << "[Client]: Received set ID: " << response.set_id() << std::endl;
                for (const auto& image_data : response.image_data()) {
                    // Convert QByteArray to QImage
                    QByteArray byteArray = QByteArray::fromBase64(QString::fromStdString(image_data).toUtf8());
                    QImage image = QImage::fromData(byteArray, "PNG");
                    receivedImages_.push_back(image);
                }
            } else {
                // Handle error for the image service
                std::cerr << "[Client]: Error: " << imageStatus.error_message() << std::endl;
            }
        });

        std::thread statusThread([this, &statusStub, &percentageRequest, &statusContext]() {
            // Make the RPC call to get the status
            std::unique_ptr<grpc::ClientReader<PercentageResponse>> reader(statusStub->GetStatus(&statusContext, percentageRequest));
            // Iterate over the stream of responses
            PercentageResponse statusResponse;
            while (reader->Read(&statusResponse)) {
            // Handle each response
                std::cout << "[Client]: Received percentage: " << statusResponse.percentage() << "\r";
                std::cout.flush();
                emit StatusUpdate(statusResponse.percentage());
                if (statusResponse.percentage() == 100) {
                    std::cout << "[Client]: Received 100% completion" << std::endl;
                    break; // Exit the loop to stop processing responses
                }
            }
            // Check if the stream has finished
            grpc::Status status = reader->Finish();
            if (!status.ok()) {
                // Handle error
                std::cerr << "[Client]: Error: " << status.error_message() << std::endl;
            }
            // Reset the request flag
            percentageRequest.set_request(0);
        });

        // Join the threads
        imageThread.join();
        statusThread.join();
    } else { 
        // Handle error saving QImage
        std::cerr << "[Client]: Error: Failed to save QImage to QByteArray" << std::endl;
    }
    }

}
