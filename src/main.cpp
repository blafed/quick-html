#define DEV

#ifdef DEV
#include "dev.cpp"
#else
#include "main.h"
#endif

int main()
{
    _jsr = JS_NewRuntime();
    _js = JS_NewContext(_jsr);

    test_js();
    return 0;
}