#include "../include/ServerConnection.h"

namespace Client {
    ServerConnection::ServerConnection(const std::string& ip, uint32_t port) {
        // Create a channel to connect to the server
        channel_ = grpc::CreateChannel(ip + ":" + std::to_string(port), grpc::InsecureChannelCredentials());
        // Create a stub to manage the connection
        stub_ = ImageData::ImageService::NewStub(channel_);
    }

    ServerConnection::~ServerConnection() {
        // Kill the thread
        end_ = 1;
        // Close the stream
        stream_->WritesDone();
        // Wait for the thread to finish
        response_thread_.join();
    }

    void ServerConnection::FetchData(std::promise<void>& promise, std::shared_ptr<int32_t> index) {
        
        response_thread_ = std::thread(&ServerConnection::ReadResponse, this, std::ref(promise), index);
    }

    grpc_connectivity_state ServerConnection::GetConnectivityState() {
        return channel_->GetState(true);
    }

    ImageData::ImagesResponse ServerConnection::GetData() {
        std::lock_guard<std::mutex> lock(data_mutex_);
        return data_;
    }

    void ServerConnection::ReadResponse(std::promise<void>& promise, std::shared_ptr<int32_t> index) {
        // Create a stream to send and receive data
        ImageData::ImagesResponse response;
        request_.set_image(std::to_string(*index));
        //stream_ = stub_->StreamData(&context_);
        stream_->Write(request_);
        while(stream_->Read(&response)) {
            if (end_) {
                return;
            }
            std::lock_guard<std::mutex> lock(data_mutex_);
            data_ = response;
            promise.set_value();
        }

    }
}