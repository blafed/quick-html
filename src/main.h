#define DEV


#include "quickjs.h"
#include <string>
#include "litehtml.h"
#include <iostream>
#include <string>

#ifdef DEV
#define CANVAS_ITY_IMPLEMENTATION
#endif

#include "canvas_ity.hpp"

#define MAIN_H

using namespace std;
using namespace litehtml;

JSRuntime* _jsr;
JSContext* _js;

const int TYPE_NULL = 0;
const int TYPE_UNKNOWN = 0;
const int TYPE_INTEGER = 1;
const int TYPE_INT_32_ARRAY = 1;
const int TYPE_DOUBLE = 2;
const int TYPE_FLOAT_64_ARRAY = 2;
const int TYPE_BOOLEAN = 3;
const int TYPE_STRING = 4;
const int TYPE_JS_ARRAY = 5;
const int TYPE_JS_OBJECT = 6;
const int TYPE_JS_FUNCTION = 7;
const int TYPE_JS_TYPED_ARRAY = 8;
const int TYPE_BYTE = 9;
const int TYPE_INT_8_ARRAY = 9;
const int TYPE_JS_ARRAY_BUFFER = 10;
const int TYPE_UNSIGNED_INT_8_ARRAY = 11;
const int TYPE_UNSIGNED_INT_8_CLAMPED_ARRAY = 12;
const int TYPE_INT_16_ARRAY = 13;
const int TYPE_UNSIGNED_INT_16_ARRAY = 14;
const int TYPE_UNSIGNED_INT_32_ARRAY = 15;
const int TYPE_FLOAT_32_ARRAY = 16;
const int TYPE_UNDEFINED = 99;

int GetObjectType(JSContext *ctx, JSValue result)
{
    if (JS_IsArray(ctx, result)) return TYPE_JS_ARRAY;
    else if (JS_IsFunction(ctx, result)) return TYPE_JS_FUNCTION;
    else if (JS_IsObject(result)) return TYPE_JS_OBJECT;
    else if (JS_IsString(result)) return TYPE_STRING;
    else if (JS_IsBigFloat(result)) return TYPE_DOUBLE;
    else if (JS_IsBool(result)) return TYPE_BOOLEAN;
    else if (JS_IsBigInt(ctx, result)) return TYPE_INTEGER;
    else if (JS_IsNull(result)) return TYPE_NULL;
    else if (JS_IsUndefined(result)) return TYPE_UNDEFINED;
    else if (JS_IsNumber(result))
    {
        int tag = JS_VALUE_GET_TAG(result);
        if (tag == JS_TAG_INT) return TYPE_INTEGER;
        return TYPE_DOUBLE;
    }

    return TYPE_UNKNOWN;
}