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

namespace litehtml
{
    element::ptr query_selector(const element::ptr &parent, const css_selector &selector, bool apply_pseudo = true, bool *is_pseudo = nullptr)
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
}
namespace binding
{

}