#include "../include/MainWindow.h"
#include <QFileDialog>

ImageWindow::ImageWindow() {
    setWindowTitle("CBIR");
    setGeometry(100, 100, 600, 600);  // Set window position and size
    setAcceptDrops(true);

    importButton = new QPushButton("Import Image", this); // Create the button
    importButton->move(10, 10); // Set the button's position
    connect(importButton, &QPushButton::clicked, this, &ImageWindow::importImage); // Connect the button to the slot
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
                update(); // Redraw with the new image
            }
        }
    }
}

void ImageWindow::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void ImageWindow::importImage() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open Image File", QString(), "Images (*.png *.jpg *.jpeg *.bmp *.gif)");

    if (!filePath.isEmpty()) {
        QImage newImage(filePath);

        if (!newImage.isNull()) {
            image = newImage;
            update(); // Redraw with the new image
        }
    }
}