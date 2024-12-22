#include "quickjs.h"
#include "litehtml.h"
#include "display.cpp"
#include "lodepng.h"
#include "test_container.h"

// KEEP THIS IN ORDER
#include "test.h"
#include "canvas_ity.hpp"

using namespace litehtml;
using namespace canvas_ity;

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

    test_container *test = new test_container(500, 1000, "");

    litehtml::estring html = readfile("res/d.html");
    document::ptr doc = document::createFromString(html, test);

    position clip = {0, 0, d->width, d->height};
    auto bestWidth = doc.get()->render(d->width, render_type::render_all);
    if (bestWidth != d->width)
        doc.get()->render(bestWidth, render_type::render_all);

    doc->draw((uint_ptr)&d->canvas, 0, 0, &clip);

    // d->canvas.fill_text("Hello, World!", 10, 10);
    d->run();
    return 0;
}