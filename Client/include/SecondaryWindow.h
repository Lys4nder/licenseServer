#ifndef QT_GUI_SECONDARYWINDOW_H
#define QT_GUI_SECONDARYWINDOW_H

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>


// this class is only used to display a popup window with the results of the query
namespace Client {
    class SecondaryWindow : public QWidget {
        Q_OBJECT
        public:
            SecondaryWindow();
            SecondaryWindow(const SecondaryWindow&) = delete;
            SecondaryWindow& operator=(const SecondaryWindow&) = delete;
            ~SecondaryWindow();
            inline void SetImages(std::vector<QImage>& images) { receivedImages_ = images; }
            void DisplayImages();
            
        private:
            std::vector<QImage> receivedImages_;
            QLabel *imageLabel_; // QLabel for displaying the image
            void SetImageLabel(QVBoxLayout* layout);
            QScrollArea *scrollArea_; // Scroll area for displaying the images
            QWidget *scrollWidget_; // Widget contained within the scroll area
            QVBoxLayout *scrollLayout_; // Layout for the scroll widget
            
    };
}

#endif // QT_GUI_SECONDARYWINDOW_H