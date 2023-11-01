#ifndef SERVER_IMAGECONVERTOR_H
#define SERVER_IMAGECONVERTOR_H

#include <QImage>
#include <opencv2/opencv.hpp>
#include <map>

namespace Server {
    class ImageConvertor {
        public:
            ImageConvertor();
            QImage ConvertToQImage(cv::Mat image);
            cv::Mat ConvertToCvMat(QImage image);
    };
}


#endif //SERVER_IMAGECONVERTOR_H