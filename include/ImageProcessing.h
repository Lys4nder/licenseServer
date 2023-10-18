#ifndef SERVER_IMAGEPROCESSING_H
#define SERVER_IMAGEPROCESSING_H
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

namespace Server {
    class ImageProcessing {
        public:
            ImageProcessing();
            void setFolderPath(std::string folderPath);
            void setQueryImagePath(std::string queryImagePath);
            void setImagePaths(std::vector<std::string> imagePaths);
            void DisplaySimilarityScores();
            void QueryImage();
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
    };
}


#endif //SERVER_IMAGEPROCESSING_H