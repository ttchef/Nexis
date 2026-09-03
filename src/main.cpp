
#include <app.hpp>

i32 main()
{
    auto app = new App{};
    while (!app->should_close()) {
        app->update();
        app->draw();
    }
    delete app;
    
    return 0;
}
