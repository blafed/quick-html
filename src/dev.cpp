#ifndef MAIN_H
#include "main.h"
#endif

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstring>
#include <iostream>
#include <fstream>

void test_js(){
    auto context = _js;

    const string str = "let s = 'hello '; s;";
    str.length();

    auto result = JS_Eval(context, str.c_str(), str.length(), "", 0);
    string s = (char *)result.u.ptr;

    auto ss = JS_ToCString(context, result);

    printf("result was \n");
    cout << result.u.int32 << "\n";
    cout << result.u.float64 << "\n";
    cout << "the string is" << ss << "\n";
    auto type = GetObjectType(context, result);

    // init_binding(context, m_binding);

    // auto container = new ();

    // auto container = new my_container(100, 200, "/");
    // auto doc = document::createFromString("<html><<body><p>stuff</p></body></html>", container);
    // auto bestWidth = doc.get()->render(1000);

    // cout << "best width" << bestWidth;

    // const auto ptr = (result.u.ptr);
    cout << "type iss " << type;
    printf("\n");
}


using namespace canvas_ity;

class X11Display {
public:
    X11Display(int width, int height)
        : width(width), height(height), display(nullptr), window(0), gc(0), ximage(nullptr) {}

    ~X11Display() {
        cleanup();
    }

    void initialize() {
        display = XOpenDisplay(nullptr);
        if (!display) {
            std::cerr << "Cannot open display.\n";
            exit(1);
        }

        int screen = DefaultScreen(display);
        window = XCreateSimpleWindow(display, RootWindow(display, screen),
                                     10, 10, width, height, 1,
                                     BlackPixel(display, screen),
                                     WhitePixel(display, screen));
        XStoreName(display, window, "Canvasity Output");
        XSelectInput(display, window, ExposureMask | KeyPressMask);
        XMapWindow(display, window);
        gc = DefaultGC(display, screen);
    }

    void displayImage(canvas_ity::canvas* canvas) {
        unsigned char* image = new unsigned char[width * height * 4];
        canvas->get_image_data(image, width, height, width * 4, 0, 0);

        for (int pixel = 0; pixel < width * height; ++pixel) {
            std::swap(image[pixel * 4 + 0], image[pixel * 4 + 2]);
        }

        if (!ximage) {
            ximage = XCreateImage(display, DefaultVisual(display, DefaultScreen(display)),
                                  24, ZPixmap, 0, reinterpret_cast<char*>(image),
                                  width, height, 32, width * 4);
        } else {
            ximage->data = reinterpret_cast<char*>(image);
        }

        XPutImage(display, window, gc, ximage, 0, 0, 0, 0, width, height);

        delete[] image;
    }

    void run(canvas_ity::canvas* canvas) {
        bool running = true;
        while (running) {
            XEvent event;
            XNextEvent(display, &event);

            switch (event.type) {
                case Expose:
                    displayImage(canvas);
                    break;
                case KeyPress:
                    running = false;
                    break;
            }
        }
    }


    void cleanup() {
        if (ximage) {
            XDestroyImage(ximage);
            ximage = nullptr;
        }
        if (display) {
            XDestroyWindow(display, window);
            XCloseDisplay(display);
        }
    }

    void run(){
        this->initialize();

        canvas_ity::canvas canvas(width, height);
        canvas.set_color(brush_type::fill_style, 1, 1, 1, 1);
        canvas.fill_rectangle(0, 0, 100, 100);

        this->run(&canvas);
    }


private:
    int width;
    int height;
    Display* display;
    Window window;
    GC gc;
    XImage* ximage;

};