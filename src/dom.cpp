#include "litehtml.h"
#include "quickjs.h"

using namespace litehtml;
element::ptr document_create_element(struct document *doc, const char *tag_name)
{

    doc->m_root->m_children.string_map atrs();
    auto elPtr = doc->create_element(tag_name, attrs);
    return elPtr;
}