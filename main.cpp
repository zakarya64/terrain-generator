#include <QApplication>
#include "terrainwidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    TerrainWidget window;
    window.resize(800, 600);
    window.show();

    return app.exec();
}