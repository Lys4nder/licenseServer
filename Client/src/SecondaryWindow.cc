#include "../include/SecondaryWindow.h"

namespace Client {
    SecondaryWindow::SecondaryWindow() {
        // set the window title
        setWindowTitle("Query Results");
        // set the window geometry
        setGeometry(300, 200, 700, 400);

        // create the main layout
        QVBoxLayout* layout = new QVBoxLayout(this); // Use a QVBoxLayout for vertical layout

        // create the scroll area
        scrollArea_ = new QScrollArea(this);
        // create the scroll widget
        scrollWidget_ = new QWidget();
        // create the scroll layout
        scrollLayout_ = new QVBoxLayout(scrollWidget_);
        // set the scroll widget layout
        scrollWidget_->setLayout(scrollLayout_);
        // set the scroll area widget
        scrollArea_->setWidget(scrollWidget_);
        // set the scroll area widget resizable
        scrollArea_->setWidgetResizable(true);

        // add the scroll area to the main layout
        layout->addWidget(scrollArea_);

        // set the image label
        SetImageLabel(scrollLayout_);

        // set the layout
        setLayout(layout);
}

    SecondaryWindow::~SecondaryWindow() {
        delete imageLabel_;
    }

    void SecondaryWindow::SetImageLabel(QVBoxLayout* layout) {
        // Create a QLabel for the image
        imageLabel_ = new QLabel(this);
        imageLabel_->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
        // Add the image label to the main layout
        layout->addWidget(imageLabel_);
    }

    void SecondaryWindow::DisplayImages() {
        // clear the scroll layout
        QLayoutItem *item;
        while ((item = scrollLayout_->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        // iterate through the images
        for (auto& image : receivedImages_) {
            // create a new label
            QLabel* imageLabel = new QLabel(this);
            // set the image
            imageLabel->setPixmap(QPixmap::fromImage(image));
            // add the label to the scroll layout
            scrollLayout_->addWidget(imageLabel);
        }
    }
}
