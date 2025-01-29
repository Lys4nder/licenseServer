#include "ImageConvertorTest.h"

TEST_F(ImageConvertorTest, ConvertToQImageGrayscale) {
    cv::Mat testMat = createTestMat(100, 100, CV_8UC1, cv::Scalar(127));
    QImage qImage = imgConvertor.ConvertToQImage(testMat);
    EXPECT_EQ(qImage.format(), QImage::Format_Grayscale8);
    EXPECT_EQ(qImage.width(), testMat.cols);
    EXPECT_EQ(qImage.height(), testMat.rows);
}

TEST_F(ImageConvertorTest, ConvertToQImageRGB) {
    cv::Mat testMat = createTestMat(100, 100, CV_8UC3, cv::Scalar(0, 127, 255));
    QImage qImage = imgConvertor.ConvertToQImage(testMat);
    EXPECT_EQ(qImage.format(), QImage::Format_RGB888);
    EXPECT_EQ(qImage.width(), testMat.cols);
    EXPECT_EQ(qImage.height(), testMat.rows);
}

TEST_F(ImageConvertorTest, ConvertToCvMatGrayscale) {
    QImage testQImage = createTestQImage(100, 100, QImage::Format_Grayscale8);
    cv::Mat cvMat = imgConvertor.ConvertToCvMat(testQImage);
    EXPECT_EQ(cvMat.type(), CV_8UC1);
    EXPECT_EQ(cvMat.cols, testQImage.width());
    EXPECT_EQ(cvMat.rows, testQImage.height());
}

TEST_F(ImageConvertorTest, ConvertToCvMatRGB) {
    QImage testQImage = createTestQImage(100, 100, QImage::Format_RGB888);
    cv::Mat cvMat = imgConvertor.ConvertToCvMat(testQImage);
    EXPECT_EQ(cvMat.type(), CV_8UC3);
    EXPECT_EQ(cvMat.cols, testQImage.width());
    EXPECT_EQ(cvMat.rows, testQImage.height());
    EXPECT_EQ(cvMat.channels(), 3);
}