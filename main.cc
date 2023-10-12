#include "include/MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ImageWindow window;
    window.show();

    return app.exec();
}
