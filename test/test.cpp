#include "quickjs.h"
#include "litehtml.h"
#include "display.cpp"
#include "lodepng.h"
#include "test_container.h"

#include "../src/main.cpp"

// KEEP THIS IN ORDER
#include "test.h"
#include "canvas_ity.hpp"

using namespace litehtml;
using namespace canvas_ity;

JSRuntime *jsr;
JSContext *js;
#include <iostream>

JSValue log(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    printf("BEING EVAL\n");
    for (int i = 0; i < argc; i++)
    {
        JSValue str = JS_ToString(ctx, argv[i]);
        const char *cstr = JS_ToCString(ctx, str);
        printf("%s\n", cstr);
        JS_FreeCString(ctx, cstr);
        JS_FreeValue(ctx, str);
    }
    return JS_NewString(ctx, "logged ");
}

// JSValue new_function(JSContext *ctx, optional<JSValue> obj, const char *name, JSCFunction *func)
// {
//     auto function = JS_NewCFunction(ctx, func, name, strlen(name));
//     JS_SetPropertyStr(ctx, obj.value_or(JS_GetGlobalObject(ctx)), name, function);
// }

void js_test()
{

    binding::new_function(js, JS_UNDEFINED, "logs", log);

    // const char *eval = "logs('Hello, World!');";
    const char *eval = "let newEl = binding.create_element('div'); logs(newEl);"
                       "binding.set_element_attr(newEl, 'id', 'test123');"
                       "let el = binding.get_element_attr(newEl, 'id'); logs(el); binding.get_body()";
    ;
    auto v = JS_Eval(js, eval, strlen(eval), "", 0);
    cout << "eval " << v.tag << '\n';
    // cout << "eval " << JS_ToCString(js, v) << '\n';

    // JS_Call(js, f, JS_UNDEFINED, 0, NULL);
}

int main()
{
    jsr = JS_NewRuntime();
    js = JS_NewContext(jsr);

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
    binding::init_bindings(js, doc);
    js_test();

    position clip = {0, 0, d->width, d->height};
    auto bestWidth = doc.get()->render(d->width, render_type::render_all);
    if (bestWidth != d->width)
        doc.get()->render(bestWidth, render_type::render_all);

    doc->draw((uint_ptr)&d->canvas, 0, 0, &clip);

    // d->canvas.fill_text("Hello, World!", 10, 10);
    d->run();
    return 0;
}