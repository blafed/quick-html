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

struct el_data
{
    uint32_t id;
    element::ptr *el;
};
namespace binding
{
    JSValue new_pointer(JSContext *ctx, uintptr_t ptr)
    {
        char *buffer = new char[sizeof(ptr)];
        std::memcpy(buffer, &ptr, sizeof(ptr));

        JSValue js_buffer = JS_NewArrayBuffer(ctx, reinterpret_cast<uint8_t *>(buffer), sizeof(ptr), [](JSRuntime *rt, void *opaque, void *ptr)
                                              { delete[] static_cast<char *>(ptr); }, nullptr, false);
        return js_buffer;
    }

    uintptr_t get_pointer(JSContext *ctx, JSValueConst js_buffer)
    {
        uint_ptr size;
        uint8_t *data = JS_GetArrayBuffer(ctx, &size, js_buffer);
        return *reinterpret_cast<uintptr_t *>(data);
    }

    element::ptr get_element(JSContext *ctx, const JSValue &val)
    {
        auto ptr = get_pointer(ctx, val);
        auto raw_pointer = reinterpret_cast<element::ptr *>(ptr);
        return *raw_pointer;
    }

    JSValue new_element(JSContext *ctx, const element::ptr &el)
    {
        auto ptr = reinterpret_cast<uintptr_t>(new element::ptr(el));
        return new_pointer(ctx, ptr);
    }

    document::ptr doc;

    JSValue new_function(JSContext *ctx, JSValue obj, const char *name, JSCFunction *func)
    {
        auto function = JS_NewCFunction(ctx, func, name, strlen(name));
        JS_SetPropertyStr(ctx, obj.tag == JS_TAG_UNDEFINED ? JS_GetGlobalObject(ctx) : obj, name, function);
    }

    JSValue create_element(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
    {
        const char *tag = JS_ToCString(ctx, argv[0]);
        string_map attrs;
        auto el = doc->create_element(tag, attrs);
        JS_FreeCString(ctx, tag);

        return new_element(ctx, el);
    }
    JSValue get_element_attr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
    {
        auto el = get_element(ctx, argv[0]);
        const char *attr = JS_ToCString(ctx, argv[1]);
        const char *val = el->get_attr(attr);
        JS_FreeCString(ctx, attr);
        if (val == nullptr || strlen(val) == 0)
            return JS_NULL;
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