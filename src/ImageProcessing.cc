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

    cv::Mat ImageProcessing::calculateSURFDescriptors(cv::Mat image) { // Changed function signature
        cv::Ptr<cv::xfeatures2d::SURF> surf = cv::xfeatures2d::SURF::create(); // Changed algorithm
        std::vector<cv::KeyPoint> keypoints;
        cv::Mat descriptors;
        surf->detectAndCompute(image, cv::noArray(), keypoints, descriptors); // Changed algorithm
        return descriptors;
    }

    double ImageProcessing::calculateCombinedSimilarity(cv::Mat queryHist, cv::Mat queryDescriptors, cv::Mat imageHist, cv::Mat imageDescriptors) {
        // Calculate similarity based on histogram
        double histSimilarity = cv::compareHist(queryHist, imageHist, cv::HISTCMP_BHATTACHARYYA);

        // Calculate similarity based on SURF descriptors
        cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("FlannBased");
        std::vector<std::vector<cv::DMatch>> knnMatches;
        matcher->knnMatch(queryDescriptors, imageDescriptors, knnMatches, 2);

        // Filter good matches based on Lowe's ratio test
        const float ratioThreshold = 0.7f;
        std::vector<cv::DMatch> goodMatches;
        for (const auto& match : knnMatches) {
            if (match[0].distance < ratioThreshold * match[1].distance) {
                goodMatches.push_back(match[0]);
            }
        }

        double surfSimilarity = 1.0 - static_cast<double>(goodMatches.size()) / std::max(queryDescriptors.rows, imageDescriptors.rows);

        // Thresholds
        const double histThreshold = 0.2; 
        const double surfThreshold = 0.8;

        // Combine histogram and SURF similarities using adjusted thresholds
        double combinedSimilarity = 0.0;
        if (histSimilarity >= histThreshold && surfSimilarity >= surfThreshold) {
            combinedSimilarity = 0.5 * histSimilarity + 0.5 * surfSimilarity;
        }

        return combinedSimilarity;
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

        cv::Mat queryHist = calculateHistogram(queryImage_);
        cv::Mat queryDescriptors = calculateSURFDescriptors(queryImage_);

        ReadImagesFolder();

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
            cv::Mat imageDescriptors = calculateSURFDescriptors(image);

            double similarityScore = calculateCombinedSimilarity(queryHist, queryDescriptors, imageHist, imageDescriptors);
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

        //DisplaySimilarityScores();
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
