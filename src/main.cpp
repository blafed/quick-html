#include "main.h"

int main()
{
    _jsr = JS_NewRuntime();
    _js = JS_NewContext(_jsr);

    test_js();
    return 0;
}