#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setStyleSheet("QLabel#coverLabel { background-color: #222; color: white; border-radius: 10px; }");

    MainWindow window;
    window.resize(500, 650);
    window.show();

    return app.exec();
}