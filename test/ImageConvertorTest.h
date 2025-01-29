#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include <QImage>
#include "../include/ImageConvertor.h"

class ImageConvertorTest : public ::testing::Test {
protected:
    Server::ImageConvertor imgConvertor;

    cv::Mat createTestMat(int rows, int cols, int type, cv::Scalar scalar) {
        return cv::Mat(rows, cols, type, scalar);
    }

    QImage createTestQImage(int width, int height, QImage::Format format) {
        QImage img(width, height, format);
        img.fill(Qt::black);
        return img;
    }
};



