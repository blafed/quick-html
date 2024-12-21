#include "quickjs.h"
#include "litehtml.h"
#include "display.cpp"
#include "lodepng.h"
#include "test_container.h"

// KEEP THIS IN ORDER
#include "test.h"
#include "canvas_ity.hpp"

int main()
{
    unsigned char *buffer;
    size_t size;

    lodepng_load_file(&buffer, &size, "res/roboto.ttf");

    X11Display *d = new X11Display(512, 1024);
    d->canvas.set_font(buffer, size, 16);
    d->canvas.set_color(brush_type::fill_style, 1, 1, 1, 1);
    cout << "something" << d->canvas.measure_text("Hello, World!") << '\n';
    //     canvas.set_color(brush_type::fill_style, 1, 1, 1, 1);
    // canvas.fill_rectangle(0, 0, 100, 100);

    d->canvas.fill_text("Hello, World!", 10, 10);
    d->run();
    return 0;
}