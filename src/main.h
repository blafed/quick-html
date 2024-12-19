#include "quickjs.h"
#include <string>

#include "dev.cpp"

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

struct binding{
    JSValue drawText_f;
};


// JSValue newFunction(long context_ptr, bool void_method, int callbackId) {
//     auto *ctx = reinterpret_cast<JSContext *>(context_ptr);
//     JSValueConst func_data[2];
//     func_data[0] = JS_NewInt32(ctx, callbackId);
//     func_data[1] = JS_NewBool(ctx, void_method);
//     JSValue func = JS_NewCFunctionData(ctx, callJavaCallback, 1, 0, 2, func_data);
//     return func;
// }

// JSValue callJavaCallback(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, int magic, JSValue *func_data) {
//     JNIEnv *env;
//     jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
//     int callbackId = JS_VALUE_GET_INT(func_data[0]);
//     bool void_method = JS_VALUE_GET_BOOL(func_data[1]);
//     auto context_ptr = (jlong) ctx;
//     JSValue args = JS_NewArray(ctx);

//     if (argv != nullptr) {
//         for (int i = 0; i < argc; ++i) {
//             JSValue it = argv[i];
//             JS_SetPropertyUint32(ctx, args, i, JS_DupValue(ctx, it));
//         }
//     }
//     jobject objectHandle = TO_JAVA_OBJECT(env, ctx, this_val);
//     jobject argsHandle = TO_JAVA_OBJECT(env, ctx, args);
//     JSValue global = JS_GetGlobalObject(ctx);
//     if (!JS_Equals(global, this_val)) {
//         JS_DupValue(ctx, this_val);
//     }
//     JS_FreeValue(ctx, global);
//     jobject result = env->CallStaticObjectMethod(quickJSCls, callJavaCallbackMethodID,
//                                                  context_ptr,
//                                                  callbackId,
//                                                  objectHandle,
//                                                  argsHandle,
//                                                  void_method
//     );

//     JSValue value = JobjectToJSValue(env, ctx, result);
//     if (env->IsInstanceOf(result, jsValueCls)) {
// //        JS_FreeValue(ctx,value);
// //        JS_DupValue(ctx, value);
//     }
//     return value;
// }




// string readfile(string s){
//     return "";
// }