#ifndef QT_GUI_MAINWINDOW_H
#define QT_GUI_MAINWINDOW_H

// Include the necessary Qt header files
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QPushButton> // Include for QPushButton
#include <QLabel>

class ImageWindow : public QWidget {
    Q_OBJECT

public:
    ImageWindow();

private slots:
    void ImportImage(); // New slot for handling image import

protected:
    QImage image;
    QPushButton* importButton; // New QPushButton for image import
    QLabel *imagePathLabel_;

    void paintEvent(QPaintEvent*) override;
    void dropEvent(QDropEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    QPalette SetPallete();
    void SetImportButton();
};

#endif // QT_GUI_MAINWINDOW_H