<<<<<<< HEAD
#include "include/ImageProcessing.h"
#include <iostream>
#include "include/ImageConvertor.h"
#include "include/ClientConnection.h"

int main() {
    RunServer();
    return 0;
}
=======
#include "include/MainWindow.h"
#include "include/ServerConnection.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
>>>>>>> source/main
