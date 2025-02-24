#include "ImageProcessingTest.h"

TEST_F(ImageProcessingTest, ConstructorInitialization) {
    EXPECT_EQ(*statusPercentagePtr, 0);
}

TEST_F(ImageProcessingTest, SetFolderPath) {
    std::string folderPath = "../test/testImages/";
    imgProc->setFolderPath(folderPath);
    EXPECT_EQ(imgProc->folderPath_, folderPath);
}

TEST_F(ImageProcessingTest, SetQueryImagePath) {
    std::string queryImagePath = "../test/testImages/image1.png";
    imgProc->setQueryImagePath(queryImagePath);
    EXPECT_EQ(imgProc->queryImagePathName_, queryImagePath);
}

TEST_F(ImageProcessingTest, SetImagePaths) {
    std::vector<std::string> imagePaths = {"../test/testImages/image1.png", "../test/testImages/image2.png"};
    imgProc->setImagePaths(imagePaths);
    EXPECT_EQ(imgProc->imagePaths_, imagePaths);
}

TEST_F(ImageProcessingTest, CalculateHistogram) {
    cv::Mat image = cv::Mat::zeros(100, 100, CV_8UC1);
    cv::Mat hist = imgProc->calculateHistogram(image);
    EXPECT_EQ(hist.rows, 256);
    EXPECT_EQ(hist.cols, 1);
}

TEST_F(ImageProcessingTest, CalculateSURFDescriptors) {
    cv::Mat image = cv::Mat::zeros(100, 100, CV_8UC1);
    cv::Mat descriptors = imgProc->calculateSURFDescriptors(image);
    EXPECT_TRUE(descriptors.empty());
}

TEST_F(ImageProcessingTest, CalculateCombinedSimilarity) {
    cv::Mat image = cv::Mat::zeros(100, 100, CV_8UC1);
    cv::Mat hist = imgProc->calculateHistogram(image);
    cv::Mat descriptors = imgProc->calculateSURFDescriptors(image);

    double similarity = imgProc->calculateCombinedSimilarity(hist, descriptors, hist, descriptors);
    EXPECT_GE(similarity, 0.0);
    EXPECT_LE(similarity, 1.0);
}

TEST_F(ImageProcessingTest, QueryImageProcessing) {
    std::string queryImagePath = "../test/testImages/image1.png";
    imgProc->setQueryImagePath(queryImagePath);

    std::string folderPath = "../test/testImages/";
    imgProc->setFolderPath(folderPath);
    imgProc->ReadImagesFolder();
    imgProc->QueryImage();

    EXPECT_EQ(*statusPercentagePtr, 100);
}