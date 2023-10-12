#ifndef QT_GUI_MAINWINDOW_H
#define QT_GUI_MAINWINDOW_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class ImageWindow : public QWidget {
    Q_OBJECT

public:
    ImageWindow();

private slots:
    void ImportImage(); // New slot for handling image import

protected:
    QImage image_;
    QPushButton* importButton_; // New QPushButton for image import
    QLabel *imagePathLabel_;
    QLabel *imageLabel_; // QLabel for displaying the image

    void paintEvent(QPaintEvent*) override;
    void dropEvent(QDropEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    QPalette SetPalette();
    void SetImportButton(QVBoxLayout* layout);
    void SetImageLabel(QVBoxLayout* layout);
    void SetTextLabel(QVBoxLayout* layout, QString text);
};
#endif // QT_GUI_MAINWINDOW_H