#include <QApplication>
#include "UI.h"
#include "Synthesizer.h"

int main(int argc, char* argv[]) {
    try {
        QApplication app(argc, argv);
        
        SynthesizerUI ui;
        ui.show();

        return app.exec();
    } catch (const std::exception& e) {
        qCritical() << "Unhandled exception: " << e.what();
        return -1;
    }
}
