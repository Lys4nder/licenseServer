#include "../include/SecondaryWindow.h"

namespace Client {
    SecondaryWindow::SecondaryWindow() {
        // set the window title
        setWindowTitle("Query Results");
        // set the window geometry
        setGeometry(300, 200, 700, 400);
        // create the layout
        QVBoxLayout* layout = new QVBoxLayout(); // Use a QVBoxLayout for vertical layout
        // set the image label
        SetImageLabel(layout);
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
        // create a new layout
        QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(this->layout()); // Use existing layout
        // add the label to the layout
        layout->addWidget(imageLabel_);
        // iterate through the images
        for (auto& image : receivedImages_) {
            // create a new label
            QLabel* imageLabel = new QLabel(this);
            // set the image
            imageLabel->setPixmap(QPixmap::fromImage(image));
            // add the label to the layout
            layout->addWidget(imageLabel);
        }
    }
}
