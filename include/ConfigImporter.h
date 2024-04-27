#ifndef QT_GUI_CONFIGIMPORTER_H
#define QT_GUI_CONFIGIMPORTER_H

#include <string>

// This class is used to import data
// from the config.cfg file
namespace Client
{
    class ConfigImporter
    {
    private:
    
        std::string port_;
        std::string addr_;

    public:
        ConfigImporter() = default;
        void ReadConfig();
        inline std::string GetPort() {return port_;}
        inline std::string GetAddr() {return addr_;}
        
    };
    
} // namespace Client

#endif //QT_GUI_CONFIGIMPORTER_H