#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstring>
#include <iostream>
#include <fstream>

#include "canvas_ity.hpp"

using namespace canvas_ity;
using namespace std;

class X11Display
{
public:
    canvas_ity::canvas canvas;
    X11Display(int width, int height)
        : width(width), height(height), display(nullptr), window(0), gc(0), ximage(nullptr), canvas(width, height) {}

    ~X11Display()
    {
        cleanup();
    }

    void initialize()
    {
        display = XOpenDisplay(nullptr);
        if (!display)
        {
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

    void displayImage(canvas_ity::canvas *canvas)
    {
        unsigned char *image = new unsigned char[width * height * 4];
        canvas->get_image_data(image, width, height, width * 4, 0, 0);

        for (int pixel = 0; pixel < width * height; ++pixel)
        {
            std::swap(image[pixel * 4 + 0], image[pixel * 4 + 2]);
        }

        if (!ximage)
        {
            ximage = XCreateImage(display, DefaultVisual(display, DefaultScreen(display)),
                                  24, ZPixmap, 0, reinterpret_cast<char *>(image),
                                  width, height, 32, width * 4);
        }
        else
        {
            ximage->data = reinterpret_cast<char *>(image);
        }

        XPutImage(display, window, gc, ximage, 0, 0, 0, 0, width, height);

        delete[] image;
    }

    void run(canvas_ity::canvas *canvas)
    {
        bool running = true;
        while (running)
        {
            XEvent event;
            XNextEvent(display, &event);

            switch (event.type)
            {
            case Expose:
                displayImage(canvas);
                break;
            case KeyPress:
                running = false;
                break;
            }
        }
    }

    void cleanup()
    {
        if (ximage)
        {
            XDestroyImage(ximage);
            ximage = nullptr;
        }
        if (display)
        {
            XDestroyWindow(display, window);
            XCloseDisplay(display);
        }
    }

    void run()
    {
        this->initialize();

        // canvas_ity::canvas canvas(width, height);
        // canvas_ity::canvas

        this->run(&canvas);
    }

private:
    int width;
    int height;
    Display *display;
    Window window;
    GC gc;
    XImage *ximage;
};