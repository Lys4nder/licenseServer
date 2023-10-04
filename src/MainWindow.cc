#include "../include/MainWindow.h"


ImageWindow::ImageWindow() {
    setWindowTitle("Drag and Drop Test");
    setGeometry(100, 100, 800, 800);  // Set window position and size
    setAcceptDrops(true);
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