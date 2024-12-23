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

    void init_bindings(JSContext *ctx, document::ptr doc);
    JSValue init_dom(JSContext *ctx, document::ptr doc);
    JSValue load_js_dom(JSContext *ctx);

    void execute_el_script(const element::ptr &el);

    char *read_resfile(const char *relPath);
}
