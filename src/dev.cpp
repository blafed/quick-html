void test_js(){
    auto context = _js;

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

    // init_binding(context, m_binding);

    // auto container = new ();

    // auto container = new my_container(100, 200, "/");
    // auto doc = document::createFromString("<html><<body><p>stuff</p></body></html>", container);
    // auto bestWidth = doc.get()->render(1000);

    // cout << "best width" << bestWidth;

    // const auto ptr = (result.u.ptr);
    cout << "type iss " << type;
    printf("\n");
}