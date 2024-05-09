#ifndef QT_GUI_SERVERCONNECTION_H
#define QT_GUI_SERVERCONNECTION_H

#include <grpc++/grpc++.h>
#include "../build/generated/proto.pb.h"
#include "../build/generated/proto.grpc.pb.h"
#include <thread>
#include <mutex>
#include <string>
#include <future>
#include <QImage>
#include <QObject>

namespace Client {
    class ServerConnection : public QObject{
        Q_OBJECT
    public:
        ServerConnection();
        ServerConnection(const ServerConnection&) = delete;
        ServerConnection& operator=(const ServerConnection&) = delete;
        ~ServerConnection();

        inline void Kill() { end_ = true; }

        inline bool IsRunning() { return !end_; }

        void MakeRequest(QImage& imageData, const uint8_t imageId);

        inline void SetIp(const std::string& ip) { ip_ = ip; }

        inline void SetPort(const std::string& port) { port_ = port; }

        inline std::vector<QImage> GetReceivedImages() { return receivedImages_; }

    signals:
        void StatusUpdate(int percentage);

    private:
        bool end_ = false;
        std::unique_ptr<ImageService::Stub> stub_;
        std::unique_ptr<StatusService::Stub> statusStub_;
        std::shared_ptr<grpc::Channel> channel_;
        grpc::ClientContext context_;
        std::string ip_;
        std::string port_;
        std::vector<QImage> receivedImages_;
    };
}

#endif // QT_GUI_SERVERCONNECTION_H
