#ifndef QT_GUI_SERVERCONNECTION_H
#define QT_GUI_SERVERCONNECTION_H

#include <grpc++/grpc++.h>
#include "../build/generated/proto.pb.h"
#include "../build/generated/proto.grpc.pb.h"
#include <thread>
#include <mutex>
#include <string>
#include <future>

namespace Client {
    class ServerConnection {
    public:
        ServerConnection(const std::string& ip, uint32_t port);
        ServerConnection(const ServerConnection&) = delete;
        ServerConnection& operator=(const ServerConnection&) = delete;
        ~ServerConnection();

        void FetchData(std::promise<void>& promise, std::shared_ptr<int32_t> index);
        grpc_connectivity_state GetConnectivityState();
        ImageData::ImagesResponse GetData();
        void ReadResponse(std::promise<void>& promise, std::shared_ptr<int32_t> index);

        inline void Kill() { end_ = true; }

        inline bool IsRunning() { return !end_; }

    private:
        bool end_ = false;
        std::unique_ptr<ImageData::ImageService::Stub> stub_;
        std::shared_ptr<grpc::Channel> channel_;
        std::unique_ptr<grpc::ClientReaderWriter<ImageData::Image, ImageData::ImagesResponse>> stream_;
        std::thread response_thread_;
        ImageData::ImagesResponse data_;
        std::mutex data_mutex_;
        grpc::ClientContext context_;
        ImageData::Image request_;
    };
}


#endif // QT_GUI_SERVERCONNECTION_H