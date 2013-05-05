#include "application.h"

using namespace Lxpanel;

int main(int argc, char** argv) {
    Application app(argc, argv);
    app.init();
    return app.exec();
}
