#include <string.h>
#include "quickjs.h"
#include "litehtml.h"

struct JSValue new_function(JSContext *ctx, optional<JSValue> obj, const char *name, JSCFunction *func);

struct Binding
{
    litehtml::document::ptr doc;
    JSCFunction
        create_element,
        get_element_by_id,
        get_elements_by_tag,
        get_element_attr,
        set_element_attr,
        append_child;

    // JSValue set_body(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
    // JSValue set_head(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
};

init_binding(lithtml::document::ptr doc);