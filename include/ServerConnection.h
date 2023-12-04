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

        void ProcessImage(const std::string& imageId, const std::vector<uint8_t>& imageData);

        inline void Kill() { end_ = true; }

        inline bool IsRunning() { return !end_; }

    private:
        bool end_ = false;
        std::unique_ptr<ImageService::Stub> stub_;
        std::shared_ptr<grpc::Channel> channel_;
        grpc::ClientContext context_;
    };
}

#endif // QT_GUI_SERVERCONNECTION_H
