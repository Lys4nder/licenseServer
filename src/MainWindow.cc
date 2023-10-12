#include "../include/MainWindow.h"
#include <QFileDialog>

ImageWindow::ImageWindow() {
    setWindowTitle("CBIR");
    setGeometry(100, 100, 700, 400);
    setAcceptDrops(true);

    // Set palette
    QPalette pal = SetPallete();
    this->setPalette(pal);

    //Set import button
    SetImportButton();
}

void ImageWindow::SetImportButton() {
    // Create the button, set its position and connect to the slot
    importButton = new QPushButton("Import Image", this);
    importButton->move(10, 10); 
    connect(importButton, &QPushButton::clicked, this, &ImageWindow::ImportImage);

    // Set image label properties
    imagePathLabel_ = new QLabel(this);
    imagePathLabel_->setFrameStyle(QFrame::Panel | QFrame::Plain);
    imagePathLabel_->setText("No image selected");
    imagePathLabel_->move(10,30);
}

QPalette ImageWindow::SetPallete() {
     // Create Color Palette
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, QColor(45, 45, 45));

    // Text Color
    pal.setColor(QPalette::WindowText, Qt::white);

    // Button Color
    pal.setColor(QPalette::Button, QColor(70, 70, 70));
    pal.setColor(QPalette::ButtonText, Qt::white); 

    // Highlight Color (for selection)
    pal.setColor(QPalette::Highlight, QColor(41, 128, 185));
    pal.setColor(QPalette::HighlightedText, Qt::white);
    // Disabled Text Color
    pal.setColor(QPalette::Disabled, QPalette::WindowText, QColor(128, 128, 128));

    // Tooltips
    pal.setColor(QPalette::ToolTipBase, QColor(50, 50, 50));
    pal.setColor(QPalette::ToolTipText, Qt::white);

    return pal;
}

void ImageWindow::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.drawImage(0, 0, image);
}

void ImageWindow::dropEvent(QDropEvent* event) {
    const QMimeData* mimeData = event->mimeData();

    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        if (!urlList.isEmpty()) {
            QString filePath = urlList.first().toLocalFile();
            QImage newImage(filePath);

            if (!newImage.isNull()) {
                image = newImage;
                imagePathLabel_->setText(filePath);
                imagePathLabel_->setFixedSize(imagePathLabel_->sizeHint());
                // Redraw with the new image
                update(); 
            }
        }
    }
}

void ImageWindow::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void ImageWindow::ImportImage() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open Image File", QString(), "Images (*.png *.jpg *.jpeg *.bmp *.gif)");

    if (!filePath.isEmpty()) {
        QImage newImage(filePath);

        if (!newImage.isNull()) {
            image = newImage.scaled(300, 200);
            imagePathLabel_->setText(filePath);
            imagePathLabel_->setFixedSize(imagePathLabel_->sizeHint());
            // Redraw with the new image
            update(); 
        }
    }
}