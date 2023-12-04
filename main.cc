#include "include/ImageProcessing.h"
#include <iostream>
#include "include/ImageConvertor.h"
#include "include/ClientConnection.h"

int main() {
   Server::ImageProcessing imageProcessing;
    imageProcessing.setFolderPath("photos/");
    imageProcessing.setQueryImagePath("640px-Clock_Tower_-_Palace_of_Westminster,_London_-_September_2006.jpg");  
    imageProcessing.QueryImage();

    Server::ImageConvertor imageConvertor;
    cv::Mat image = cv::imread("640px-Clock_Tower_-_Palace_of_Westminster,_London_-_September_2006.jpg", cv::IMREAD_GRAYSCALE);

    QImage qi = imageConvertor.ConvertToQImage(image);

    if (qi.isNull()) {
        std::cout << "QImage is null" << std::endl;
    } else {
        std::cout << "QImage is not null" << std::endl;
    }
    RunServer();
    return 0;
}