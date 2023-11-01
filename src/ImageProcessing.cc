#include "../include/ImageProcessing.h"
#include <iostream>

namespace Server {

    ImageProcessing::ImageProcessing() {}

    cv::Mat ImageProcessing::calculateHistogram(cv::Mat image) {
        cv::Mat hist;
        int histSize = 256;
        float range[] = {0, 256};
        const float* histRange = {range};
        cv::calcHist(&image, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);
        return hist;
    }

    double ImageProcessing::calculateHistogramSimilarity(cv::Mat hist1, cv::Mat hist2) {
        return cv::compareHist(hist1, hist2, cv::HISTCMP_BHATTACHARYYA);
    }

    void ImageProcessing::setQueryImagePath(std::string queryImagePath) {
        queryImagePathName_ = queryImagePath;
    }

    void ImageProcessing::setFolderPath(std::string folderPath) {
        folderPath_ = folderPath;
    }

    void ImageProcessing::setImagePaths(std::vector<std::string> imagePaths) {
        imagePaths_ = imagePaths;
    }


    void ImageProcessing::DisplaySimilarityScores() {
        for (const auto &similarityScore : similarityScores_) {
            std::cout << similarityScore.first << ": " << "Similarity score: "<< similarityScore.second << std::endl;
        }
    }

    void ImageProcessing::QueryImage() {
        queryImage_ = cv::imread(queryImagePathName_, cv::IMREAD_GRAYSCALE);

        if (queryImage_.empty()) {
            std::cout << "Could not read the image: " << queryImagePathName_ << std::endl;
            return;
        }

        ReadImagesFolder();

        queryHist_ = calculateHistogram(queryImage_);

        for (const std::string&imagePath : imagePaths_) {
            cv::Mat image = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);

            if (image.empty()) {
                std::cout << "Could not read the image: " << imagePath << std::endl;
                continue;
            }

            cv::Mat imageHist = calculateHistogram(image);
            double similarityScore = calculateHistogramSimilarity(queryHist_, imageHist);
            similarityScores_.emplace_back(imagePath, similarityScore);
        }

        sort(similarityScores_.begin(), similarityScores_.end(), [](const std::pair<std::string, double> &a, const std::pair<std::string, double> &b) {
            return a.second < b.second;
        });

        DisplaySimilarityScores();

    }

    void ImageProcessing::ReadImagesFolder() {

        cv::glob(folderPath_ + "*.jpg", imagePaths_, false);

        if (imagePaths_.empty()) {
            std::cout << "Could not find any image files in the folder: " << folderPath_ << std::endl;
        } else {
            std::cout << "Found " << imagePaths_.size() << " image files in the folder: " << folderPath_ << std::endl;
        }
}
} // namespace Server
