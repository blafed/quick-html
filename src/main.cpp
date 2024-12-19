#include <iostream>
#include <string>

// #include "lib/litehtml/containers/test/test_container.h"
#include "litehtml.h"
#include "main.h"
#include "quickjs.h"
// #include "docc.h"
// #include "test_container.h"

using namespace std;
using namespace litehtml;

uintptr_t screen_canvas;
binding* m_binding;
JSRuntime* runtime;


int main()
{

    JSRuntime *runtime = JS_NewRuntime();
    JSContext *context = JS_NewContext(runtime);

    const string str = "let s = 'hello world'; s;";
    str.length();

    auto result = JS_Eval(context, str.c_str(), str.length(), "", 0);
    string s = (char *)result.u.ptr;

    auto ss = JS_ToCString(context, result);

    printf("result was \n");
    cout << result.u.int32 << "\n";
    cout << result.u.float64 << "\n";
    cout << "the string is" << ss << "\n";
    auto type = GetObjectType(context, result);

    ws_start();
    return 0;
    



        scanf("you");
        ws_send("hooooooooooooo");
        ws_send("hooooooooooooo");
        ws_send("hooooooooooooo");
        scanf("you");
    // m_binding = new binding();
    // init_binding(context, m_binding);

    // auto container = new ();

    // auto container = new my_container(100, 200, "/");
    // auto doc = document::createFromString("<html><<body><p>stuff</p></body></html>", container);
    // auto bestWidth = doc.get()->render(1000);

    // cout << "best width" << bestWidth;

    // const auto ptr = (result.u.ptr);
    cout << "type iss " << type;
    printf("\n");


    return 0;
}



// void init_binding(const JSContext* context, binding& binding){
//     binding.drawText_f = JS_Call()
// }


// void js_draw_text(JSContext* context, uintptr_t ptr){
//     JS_Call(context, m_binding.drawText_f, )
// }


// JSValue JobjectToJSValue(JNIEnv *env, JSContext *ctx, jobject value)
// {
//     if (value == nullptr)
//     {
//         return JS_NULL;
//     }
//     else if (env->IsInstanceOf(value, integerCls))
//     {
//         return JS_NewInt32(ctx, env->CallIntMethod(value, intValueMethodID));
//     }
//     else if (env->IsInstanceOf(value, longCls))
//     {
//         return JS_NewInt64(ctx, env->CallLongMethod(value, longValueMethodID));
//     }
//     else if (env->IsInstanceOf(value, doubleCls))
//     {
//         return JS_NewFloat64(ctx, env->CallDoubleMethod(value, doubleValueMethodID));
//     }
//     else if (env->IsInstanceOf(value, booleanCls))
//     {
//         return JS_NewBool(ctx, env->CallBooleanMethod(value, booleanValueMethodID));
//     }
//     else if (env->IsInstanceOf(value, stringCls))
//     {
//         return JS_NewString(ctx, env->GetStringUTFChars((jstring)value, nullptr));
//     }
//     else if (env->IsInstanceOf(value, jsValueCls))
//     {
//         return JS_DupValue(ctx, TO_JS_VALUE(env, value));
//     }
//     return JS_UNDEFINED;
// }
