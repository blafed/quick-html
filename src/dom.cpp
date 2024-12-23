#include "litehtml.h"
#include "quickjs.h"
#include "string.h"
#include "dom.h"
#include "fstream"
#include "iostream"
using namespace litehtml;

namespace dom
{
    element::ptr query_selector(const element::ptr &parent, const css_selector &selector, bool apply_pseudo, bool *is_pseudo)
    {
        for (auto &el : parent->children())
        {
            int res = el->select(selector, apply_pseudo);
            if (res != select_no_match)
            {
                if (is_pseudo)
                {
                    if (res & select_match_pseudo_class)
                        *is_pseudo = true;
                    else
                        *is_pseudo = false;
                }
                return el;
            }
            auto found = query_selector(el, selector, apply_pseudo, is_pseudo);
            if (found.get() != nullptr)
                return found;
        }
        return nullptr;
    }

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

    JSValue new_function(JSContext *ctx, JSValue obj, const char *name, JSCFunction *func)
    {
        auto function = JS_NewCFunction(ctx, func, name, strlen(name));
        JS_SetPropertyStr(ctx, obj.tag == JS_TAG_UNDEFINED ? JS_GetGlobalObject(ctx) : obj, name, function);
        return function;
    }

}

namespace dom::static_fields
{
    document::ptr doc;
}

namespace dom::bindings
{
    using namespace dom::static_fields;

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

    JSValue set_element_attr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
    {
        auto el = get_element(ctx, argv[0]);
        const char *attr = JS_ToCString(ctx, argv[1]);
        const char *val = JS_ToCString(ctx, argv[2]);
        el->set_attr(attr, val);
        JS_FreeCString(ctx, attr);
        JS_FreeCString(ctx, val);
        return JS_UNDEFINED;
    }

    JSValue append_child(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
    {
        auto el = get_element(ctx, argv[0]);
        auto child = get_element(ctx, argv[1]);
        el->appendChild(child);
        return JS_UNDEFINED;
    }

    JSValue document_body(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
    {
        css_selector selector;
        selector.parse("body", no_quirks_mode);
        auto el = query_selector(doc->root(), selector);
        if (el == nullptr)
            return JS_NULL;
        return new_element(ctx, el);
    }

    JSValue document_head(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
    {
        css_selector selector;
        selector.parse("head", no_quirks_mode);
        auto el = query_selector(doc->root(), selector);
        if (el == nullptr)
            return JS_NULL;
        return new_element(ctx, el);
    }
}

namespace dom
{
    using namespace dom::bindings;
    using namespace dom::static_fields;
    using namespace std;

    const char *read_resfile(const char *path, size_t *len)
    {
        string sPath(path);
        std::ifstream file("res/" + sPath, ios::ate);
        if (!file.is_open())
        {
            std::cout << "ERROR: " << "Could not open file res" << sPath << '\n';
            return "";
        }

        size_t c = file.tellg();
        file.seekg(0, ios::beg);
        char *buffer = new char[c + 1];
        file.read(buffer, c);
        buffer[c] = '\0';
        file.close();

        if (len != nullptr)
            *len = c;

        return buffer;
    }

    JSValue log(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
    {
        for (int i = 0; i < argc; i++)
        {
            JSValue str = JS_ToString(ctx, argv[i]);
            const char *cstr = JS_ToCString(ctx, str);
            printf("LOG: %s\n", cstr);
            JS_FreeCString(ctx, cstr);
            JS_FreeValue(ctx, str);
        }
        return JS_NewString(ctx, "logged ");
    }

    void init_bindings(JSContext *ctx, document::ptr doc)
    {
        static_fields::doc = doc;
        JSValue obj = JS_NewObject(ctx);
        JS_SetPropertyStr(ctx, JS_GetGlobalObject(ctx), "binding", obj);

        new_function(ctx, obj, "create_element", create_element);
        new_function(ctx, obj, "get_element_attr", get_element_attr);
        new_function(ctx, obj, "set_element_attr", set_element_attr);
        new_function(ctx, obj, "append_child", append_child);
        new_function(ctx, obj, "document_body", document_body);
        new_function(ctx, obj, "document_head", document_head);

        new_function(ctx, JS_UNDEFINED, "log", log);
    }

    JSValue load_js_dom(JSContext *ctx)
    {
        size_t len;
        // char *word = "\"use strict\";\n1 + 2;\n";
        const char *buffer =
            // "\"use strict\";\n1+2";
            read_resfile("dom.js", &len);

        // char *safe = new char[len];
        // memcpy(safe, buffer, len);

        // string str(buffer);
        // int any = false;
        // for (int i = 0; i < len; i++)
        // {
        //     if (buffer[i] != word[i])
        //         any = true;
        //     cout << "char at " << i << " is " << buffer[i] << " EQUALS =" << (buffer[i] == word[i]) << '\n';
        // }
        // cout << "ANY " << any << '\n'
        //      << "LEN " << strlen(word) << " LEN2 " << strlen(buffer) << '\n';
        // cout << buffer;
        JSValue v = JS_Eval(ctx, buffer, len, "", 0);

        // char *eval = "log('hehe')";

        // auto v = JS_Eval(ctx, eval, strlen(eval), "", 0);

        // cout << "VALUE IS " << v.tag << " len " << len << '\n';

        return v;
    }

    JSValue init_dom(JSContext *ctx, document::ptr doc)
    {
        init_bindings(ctx, doc);
        return load_js_dom(ctx);
    }

}