#ifndef SERVER_IMAGEPROCESSING_H
#define SERVER_IMAGEPROCESSING_H
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <atomic>

namespace Server {
    class ImageProcessing {
        public:
            ImageProcessing();
            void setFolderPath(std::string folderPath);
            void setQueryImagePath(std::string queryImagePath);
            void setImagePaths(std::vector<std::string> imagePaths);
            void DisplaySimilarityScores();
            void QueryImage();
            std::vector<std::pair<std::string, double>> GetSimilarityScores();
        private:
            std::string folderPath_;
            std::string queryImagePathName_;
            std::vector<std::string> imagePaths_;
            cv::Mat queryImage_;
            cv::Mat queryHist_;
            std::vector<std::pair<std::string, double>> similarityScores_;

            cv::Mat calculateHistogram(cv::Mat image);
            double calculateHistogramSimilarity(cv::Mat hist1, cv::Mat hist2);
            void ReadImagesFolder();
            int statusPercentage_;
            std::mutex mutex_;
    };
}


#endif //SERVER_IMAGEPROCESSING_H