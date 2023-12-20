#include "../include/ImageConvertor.h"


namespace Server {

    ImageConvertor::ImageConvertor() {}

    QImage ImageConvertor::ConvertToQImage(cv::Mat image) {
        
        QImage qImage;

        switch (image.channels())
        {
        case 1:
            qImage = QImage((const unsigned char*)(image.data), image.cols, image.rows, image.step, QImage::Format_Grayscale8);
            break;
        case 3:
            cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
            qImage = QImage((const unsigned char*)(image.data), image.cols, image.rows, image.step, QImage::Format_RGB888);
            break;
        default:
            qImage = QImage((const unsigned char*)(image.data), image.cols, image.rows, image.step, QImage::Format_RGB888);
            break;
        }

        return qImage;
    }

    cv::Mat ImageConvertor::ConvertToCvMat(QImage image) {
        cv::Mat cvMat;

        switch (image.format())
        {
        case QImage::Format_Grayscale8:
            cvMat = cv::Mat(image.height(), image.width(), CV_8UC1, (uchar*)image.bits(), image.bytesPerLine());
            break;
        case QImage::Format_RGB888:
            cvMat = cv::Mat(image.height(), image.width(), CV_8UC3, (uchar*)image.bits(), image.bytesPerLine());
            cv::cvtColor(cvMat, cvMat, cv::COLOR_RGB2BGR);
            break;
        default:
            cvMat = cv::Mat(image.height(), image.width(), CV_8UC3, (uchar*)image.bits(), image.bytesPerLine());
            cv::cvtColor(cvMat, cvMat, cv::COLOR_RGB2BGR);
            break;
        }

        return cvMat;
    }
}