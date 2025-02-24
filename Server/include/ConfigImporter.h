#ifndef SERVER_CONFIGIMPORTER_H
#define SERVER_CONFIGIMPORTER_H

#include <string>

// This class is used to import data
// from the config.cfg file
namespace Server
{
    class ConfigImporter
    {
    private:
    
        std::string imageFolderPath_;
        std::string port_;
        std::string addr_;

    public:
        ConfigImporter() = default;
        void ReadConfig();
        inline std::string GetImageFolderPath() {return imageFolderPath_;}
        inline std::string GetPort() {return port_;}
        inline std::string GetAddr() {return addr_;}
        
    };
    
} // namespace Server

#endif //SERVER_CONFIGIMPORTER_H