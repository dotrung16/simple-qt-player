#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QPushButton button("Hello!");
    button.resize(300, 150);
    button.show();

    return app.exec();
}