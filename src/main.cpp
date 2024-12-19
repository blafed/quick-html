#include "main.h"
#ifdef DEV
#include "dev.cpp"
#endif

int main()
{
    _jsr = JS_NewRuntime();
    _js = JS_NewContext(_jsr);
    X11Display* d = new X11Display(512,1024);
    d->run();
    // test_js();
    return 0;
}