#include "../include/ImageProcessing.h"
#include <iostream>

namespace Server {

    ImageProcessing::ImageProcessing(std::shared_ptr<int> statusPercentagePtr) : statusPercentagePtr_(statusPercentagePtr) {}

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

        size_t totalImages = imagePaths_.size();
        size_t processedImages = 0;
        statusPercentage_ = 0;

        // Set the statusPercentage_ to 0
        {
            std::lock_guard<std::mutex> lock(mutex_);
            *statusPercentagePtr_ = 0;
        }

        for (const std::string& imagePath : imagePaths_) {
            cv::Mat image = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);

            if (image.empty()) {
                std::cout << "[Server]: Could not read the image: " << imagePath << std::endl;
                continue;
            }

            cv::Mat imageHist = calculateHistogram(image);
            double similarityScore = calculateHistogramSimilarity(queryHist_, imageHist);
            similarityScores_.emplace_back(imagePath, similarityScore);
                // Update and display the progress bar
                processedImages++;
                double progress = (double)processedImages / totalImages;
                int barWidth = 70;

                // Set the text color to green
                std::cout << "\033[32m"; 
                std::cout << "Processing image: " << processedImages << "/" << totalImages << " [";
                int pos = barWidth * progress;
                for (int i = 0; i < barWidth; ++i) {
                    if (i < pos) std::cout << "=";
                    else if (i == pos) std::cout << ">";
                    else std::cout << " ";
                }
                statusPercentage_ = int(progress * 100.0);
                {
                    std::lock_guard<std::mutex> lock(mutex_);
                    *statusPercentagePtr_ = statusPercentage_;
                }
                std::cout << "] " << statusPercentage_ << " %\r";
                std::cout.flush();

                // Reset the text color to default
                std::cout << "\033[0m";
        }

        std::cout << std::endl;

        sort(similarityScores_.begin(), similarityScores_.end(), [](const std::pair<std::string, double> &a, const std::pair<std::string, double> &b) {
            return a.second < b.second;
        });

        DisplaySimilarityScores();
    }

    void ImageProcessing::ReadImagesFolder() {

        cv::glob(folderPath_ + "*.png", imagePaths_, false);

        if (imagePaths_.empty()) {
            std::cout << "[Server]: Could not find any image files in the folder: " << folderPath_ << std::endl;
        } else {
            std::cout << "[Server]: Found " << imagePaths_.size() << " image files in the folder: " << folderPath_ << std::endl;
        }
    }

    std::vector<std::pair<std::string, double>> ImageProcessing::GetSimilarityScores() {
        return similarityScores_;
    }
} // namespace Server
