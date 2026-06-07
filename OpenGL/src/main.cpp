#include "config.hpp"
#include "controller/app.hpp"

int main() {
    App* app = new App();

    app->run();

    delete app;
    return 0;
}