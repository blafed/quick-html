#include <iostream>
#include <string>

#include "../lib/quickjs/quickjs.h"
#include "main.h"

using namespace std;

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

    // const auto ptr = (result.u.ptr);
    cout << "type iss " << type;
    printf("\n");

    return 0;
}

void define_dom_bindings()
{
}

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
