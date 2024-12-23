#include "quickjs.h"
#include "litehtml.h"
#include "iostream"
using namespace litehtml;
namespace dom
{
    element::ptr query_selector(const element::ptr &parent, const css_selector &selector, bool apply_pseudo = true, bool *is_pseudo = nullptr);

    JSValue new_pointer(JSContext *ctx, uintptr_t ptr);
    uintptr_t get_pointer(JSContext *ctx, JSValueConst js_buffer);
    JSValue new_element(JSContext *ctx, const element::ptr &el);
    element::ptr get_element(JSContext *ctx, const JSValue &val);
    JSValue new_function(JSContext *ctx, JSValue obj, const char *name, JSCFunction *func);

    // JSValue create_bindings_obj();
    // void set_bindings_target(JSContext *ctx, const document::ptr &doc);

    void init_bindings(JSContext *ctx, document::ptr doc);
    JSValue init_dom(JSContext *ctx, document::ptr doc);
    JSValue load_js_dom(JSContext *ctx);

    void execute_el_script(const element::ptr &el);

    char *read_resfile(const char *relPath);

    // struct bindings
    // {

    //     bindings()
    //     {
    //         JSValue obj = JS_NewObject(ctx);
    //         JS_SetPropertyStr(ctx, JS_GetGlobalObject(ctx), "binding", obj);

    //         new_function(ctx, obj, "create_element", get_body);
    //     }
    //     document::ptr doc;
    //     JSValue get_body(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
    //     {
    //         const element::ptr body1;
    //         css_selector selector;
    //         selector.parse("body", no_quirks_mode);
    //         auto body = query_selector(doc->root(), selector);
    //         cout << "body " << body->tag() << '\n';
    //         // return new_element(ctx, doc->root());
    //         if (body.get() == nullptr)
    //         {
    //             return JS_NULL;
    //         }
    //         return new_element(ctx, body);
    //     }
    // }
}
