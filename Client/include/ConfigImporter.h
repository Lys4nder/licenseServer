<<<<<<< HEAD
#ifndef SERVER_CONFIGIMPORTER_H
#define SERVER_CONFIGIMPORTER_H
=======
#ifndef QT_GUI_CONFIGIMPORTER_H
#define QT_GUI_CONFIGIMPORTER_H
>>>>>>> source/main

#include <string>

// This class is used to import data
// from the config.cfg file
<<<<<<< HEAD
namespace Server
=======
namespace Client
>>>>>>> source/main
{
    class ConfigImporter
    {
    private:
    
<<<<<<< HEAD
        std::string imageFolderPath_;
=======
>>>>>>> source/main
        std::string port_;
        std::string addr_;

    public:
        ConfigImporter() = default;
        void ReadConfig();
<<<<<<< HEAD
        inline std::string GetImageFolderPath() {return imageFolderPath_;}
=======
>>>>>>> source/main
        inline std::string GetPort() {return port_;}
        inline std::string GetAddr() {return addr_;}
        
    };
    
<<<<<<< HEAD
} // namespace Server

#endif //SERVER_CONFIGIMPORTER_H
=======
} // namespace Client

#endif //QT_GUI_CONFIGIMPORTER_H
>>>>>>> source/main
