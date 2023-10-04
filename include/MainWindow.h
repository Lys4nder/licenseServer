#ifndef QT_GUI_MAINWINDOW_H
#define QT_GUI_MAINWINDOW_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <iostream>

class ImageWindow : public QWidget {
    Q_OBJECT

public:
    ImageWindow();

protected:
    QImage image;

    void paintEvent(QPaintEvent*) override;

    void dropEvent(QDropEvent* event) override;

    void dragEnterEvent(QDragEnterEvent* event) override;
};
#endif // QT_GUI_MAINWINDOW_H