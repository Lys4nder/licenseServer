#include "include/MainWindow.h"
#include <QApplication>
#include <opencv2/opencv.hpp>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ImageWindow window;
    window.show();

    return app.exec();
}
