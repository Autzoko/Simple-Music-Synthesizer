#include <QApplication>
#include "SynthesizerUI.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    SynthesizerUI ui;
    ui.show();

    return app.exec();
}
