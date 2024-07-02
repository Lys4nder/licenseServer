#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include "../include/ImageProcessing.h"

// Derived test class to access protected members and methods
class TestableImageProcessing : public Server::ImageProcessing {
public:
    using Server::ImageProcessing::ImageProcessing;
    using Server::ImageProcessing::calculateHistogram;
    using Server::ImageProcessing::calculateSURFDescriptors;
    using Server::ImageProcessing::calculateCombinedSimilarity;
    using Server::ImageProcessing::ReadImagesFolder;
    using Server::ImageProcessing::folderPath_;
    using Server::ImageProcessing::queryImagePathName_;
    using Server::ImageProcessing::imagePaths_;
};

class ImageProcessingTest : public ::testing::Test {
protected:
    void SetUp() override {
        statusPercentagePtr = std::make_shared<int>(0);
        imgProc = std::make_shared<TestableImageProcessing>(statusPercentagePtr);
    }

    std::shared_ptr<int> statusPercentagePtr;
    std::shared_ptr<TestableImageProcessing> imgProc;
};


