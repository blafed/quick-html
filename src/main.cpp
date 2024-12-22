#include "quickjs.h"
#include <string.h>
#include "litehtml.h"
using namespace litehtml;

// struct obj_ptr
// {
//     char bytes[sizeof(uint_ptr)];

//     uint_ptr as_ptr()
//     {
//         return reinterpret_cast<uint_ptr>(bytes);
//     }
// };

// const char *encode_ptr(uint_ptr ptr)
// {
//     obj_ptr obj;
//     memcpy(obj.bytes, &ptr, sizeof(uint_ptr));
//     return obj.bytes;
// }

// const obj_ptr decode_ptr(const char *bytes)
// {
//     obj_ptr obj;
//     memcpy(obj.bytes, bytes, sizeof(uint_ptr));
//     return obj;
// }

JSValue pass_pointer_to_js(JSContext *ctx, uintptr_t ptr)
{
    JSValue buffer = JS_NewArrayBuffer(ctx, sizeof(ptr));
    uint8_t *data = JS_GetArrayBuffer(ctx, nullptr, buffer);
    std::memcpy(data, &ptr, sizeof(ptr)); // Copy pointer to the buffer
    return buffer;
}

uintptr_t parse_pointer_from_js(JSContext *ctx, JSValueConst js_buffer)
{
    size_t size;
    uint8_t *data = JS_GetArrayBuffer(ctx, &size, js_buffer);
    if (size != sizeof(uintptr_t))
    {
        // Error: Invalid buffer size
        return 0;
    }

    uintptr_t ptr;
    std::memcpy(&ptr, data, sizeof(ptr)); // Reconstruct the pointer
    return ptr;
}

namespace binding
{
    document::ptr doc;

    JSValue new_function(JSContext *ctx, JSValue obj, const char *name, JSCFunction *func)
    {
        auto function = JS_NewCFunction(ctx, func, name, strlen(name));
        JS_SetPropertyStr(ctx, obj.tag == JS_TAG_UNDEFINED ? JS_GetGlobalObject(ctx) : obj, name, function);
    }

    JSValue create_element(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
    {
        JSValue vTag = JS_GetPropertyStr(ctx, argv[0], "tag");
        const char *tag = JS_ToCString(ctx, vTag);
        const string_map attrs;
        litehtml::element::ptr el = doc->create_element(tag, attrs);

        JS_FreeCString(ctx, tag);

        return JS_NewInt64(ctx, reinterpret_cast<uintptr_t>(el.get()));
    }
    JSValue get_element_attr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
    {
        JSValue vEl = JS_GetPropertyStr(ctx, argv[0], "el");
        element *el = reinterpret_cast<element *>(JS_VALUE_GET_INT(vEl));
        JSValue vAttr = JS_GetPropertyStr(ctx, argv[0], "attr");
        const char *attr = JS_ToCString(ctx, vAttr);
        const char *val = el->get_attr(attr);
        JS_FreeCString(ctx, attr);
        return JS_NewString(ctx, val);
    }

    void init_bindings(JSContext *ctx, document::ptr doc)
    {
        binding::doc = doc;
        JSValue obj = JS_NewObject(ctx);
        JS_SetPropertyStr(ctx, JS_GetGlobalObject(ctx), "binding", obj);

        new_function(ctx, obj, "create_element", create_element);
        new_function(ctx, obj, "get_element_attr", get_element_attr);
    }
}