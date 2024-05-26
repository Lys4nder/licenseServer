#include "../include/MainWindow.h"
#include "../include/SecondaryWindow.h"
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtConcurrent/QtConcurrent>


MainWindow::MainWindow() {
    setWindowTitle("CBIR");
    setGeometry(100, 100, 900, 400);
    setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
    setAcceptDrops(true);

    // Create the main layout for the window
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    //Create secondary layouts
    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    QHBoxLayout* contentLayout = new QHBoxLayout;

    // Set palette
    QPalette pal = SetPalette();
    this->setPalette(pal);

    // Create and set the import button
    SetImportButton(buttonsLayout);
    SetQueryButton(buttonsLayout);

    mainLayout->addLayout(buttonsLayout);

    // Create and set the text label
    SetTextLabel(mainLayout, "No picture selected");

    // Create and set the image label
    SetImageLabel(contentLayout);
    SetStatusLabel(contentLayout);

    mainLayout->addLayout(contentLayout);

    SetStatusBar(mainLayout);

    connect(&connection_, &Client::ServerConnection::StatusUpdate, this, &MainWindow::UpdateProgressBar);
}

void MainWindow::UpdateProgressBar(int percentage) {
    // Update progress bar and label with the received percentage
    progressBar_->setValue(percentage);
    progressBarLabel_->setText(QString::number(percentage) + "%");
    QCoreApplication::processEvents();
}

void MainWindow::SetImportButton(QHBoxLayout* layout) {
    // Create the import button
    importButton_ = new QPushButton("Import Image");
    connect(importButton_, &QPushButton::clicked, this, &MainWindow::ImportImage);

    // Add the import button to the main layout
    layout->addWidget(importButton_);
}

void MainWindow::SetQueryButton(QHBoxLayout* layout) {
    queryButton_ = new QPushButton("Query Image");
    connect(queryButton_, &QPushButton::clicked, this, &MainWindow::QueryImage);
    layout->addWidget(queryButton_);
}

void MainWindow::SetImageLabel(QHBoxLayout* layout) {
    // Create a QLabel for the image
    imageLabel_ = new QLabel(this);
    imageLabel_->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    // Add the image label to the main layout
    layout->addWidget(imageLabel_);
}

void MainWindow::SetTextLabel(QVBoxLayout* layout, QString text) {
    // Create a QLabel for the text
    imagePathLabel_ = new QLabel(text);
    imagePathLabel_->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    imagePathLabel_->setFixedHeight(imagePathLabel_->fontMetrics().height() + 20);

    // Allow the label to expand horizontally
    imagePathLabel_->setWordWrap(true);
    imagePathLabel_->setAlignment(Qt::AlignTop);

    // Add the text label to the main layout
    layout->addWidget(imagePathLabel_);
}

void MainWindow::SetStatusLabel(QHBoxLayout* layout) {
    // Create a QLabel for the image
    rgbLabel = new QLabel(this);
    rgbLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    // Add the image label to the main layout
    layout->addWidget(rgbLabel);
}


QPalette MainWindow::SetPalette() {
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

void MainWindow::dropEvent(QDropEvent* event) {
    const QMimeData* mimeData = event->mimeData();

    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        if (!urlList.isEmpty()) {
            QString filePath = urlList.first().toLocalFile();
            QImage newImage(filePath);

            if (!newImage.isNull()) {
                //image_ = newImage;
                imagePathLabel_->setText(filePath);
                imagePathLabel_->setFixedSize(imagePathLabel_->sizeHint());
                // Redraw with the new image
                update(); 
            }
        }
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::ImportImage() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open Image File", QString(), "Images (*.png)");

    if (!filePath.isEmpty()) {
        QImage newImage(filePath);
        toBeSentImage_ = newImage;

        if (!newImage.isNull()) {
            // Calculate the scaled dimensions based on the size of the imageLabel_
            QSize scaledSize = imageLabel_->size();
            image_ = QImage(newImage.scaled(scaledSize, Qt::KeepAspectRatio));

            // Set the text and imageLabel
            imagePathLabel_->setText(filePath);
            imageLabel_->setPixmap(QPixmap::fromImage(image_));
            statusBar_->showMessage("Image imported");
        } else {
            statusLabel_->setText("Failed to import image");
        }
    }
    else {
        statusLabel_->setText("No selection made.");
    }
}


void MainWindow::QueryImage() {
    if (image_.isNull()) {
        statusBar_->showMessage("No image imported");
    }
    else {
        statusBar_->showMessage("Processing image...");

        // Run MakeRequest function on a separate thread
        QFuture<void> future = QtConcurrent::run([this]() {
            connection_.MakeRequest(toBeSentImage_, 1);
        });

        // Connect a watcher to the future to handle result when done
        QFutureWatcher<void>* watcher = new QFutureWatcher<void>();
        connect(watcher, &QFutureWatcher<void>::finished, this, [this, watcher]() {
            std::cout << "[Client]: Watcher finished" << std::endl;
            watcher->deleteLater();
            std::vector<QImage> receivedImages = connection_.GetReceivedImages();

            //scale images to fit label
            QSize scaledSize = imageLabel_->size();
            int i = 0;
            for (auto& img : receivedImages) {
                if (i == 0) {
                    img = img.scaled(scaledSize, Qt::KeepAspectRatio);
                    rgbLabel->setPixmap(QPixmap::fromImage(img));
                    i++;
                } else {
                    img = img.scaled(scaledSize, Qt::KeepAspectRatio);
                }
            }

            if (!receivedImages.empty()) {
                receivedImages.erase(receivedImages.begin());
            }

            statusBar_->showMessage("Displaying result");

            // Create a secondary window to display the results
            Client::SecondaryWindow* secondaryWindow = new Client::SecondaryWindow();
            secondaryWindow->SetImages(receivedImages);
            secondaryWindow->DisplayImages();
            secondaryWindow->show();
        });
        watcher->setFuture(future);
    }
}

void MainWindow::SetStatusBar(QVBoxLayout* layout) {
    statusBar_ = new QStatusBar(this);
    statusBar_->setFixedHeight(20);
    statusBar_->showMessage("Ready");

    progressBar_ = new QProgressBar(this);
    progressBar_->setRange(0, 100);
    progressBar_->setValue(0);
    progressBar_->setFormat("%p%");
    
    progressBarLabel_ = new QLabel(this);
    progressBarLabel_->setText("0%");
    statusBar_->addPermanentWidget(progressBar_);
    statusBar_->addPermanentWidget(progressBarLabel_);

    layout->addWidget(statusBar_);
}