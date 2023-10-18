#include "include/ImageProcessing.h"

int main() {
    Server::ImageProcessing imageProcessing;
    imageProcessing.setFolderPath("photos/");
    imageProcessing.setQueryImagePath("640px-Clock_Tower_-_Palace_of_Westminster,_London_-_September_2006.jpg");  
    imageProcessing.QueryImage();

    return 0;
}
