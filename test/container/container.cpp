#include "container.h"
#include "lodepng.h"

// namespace demo_container
// {
//     using namespace canvas_ity;
//     void set_color(canvas &cvs, brush_type type, color c)
//     {
//         cvs.set_color(type, c.r / 255.f, c.g / 255.f, c.b / 255.f, c.a / 255.f);
//     }

//     void fill_rect(canvas &cvs, rect r)
//     {
//         cvs.fill_rectangle((float)r.x, (float)r.y, (float)r.width, (float)r.height);
//     }

//     void fill_rect(canvas &cvs, rect r, color color)
//     {
//         set_color(cvs, fill_style, color);
//         fill_rect(cvs, r);
//     }

//     void fill_circle(canvas &cvs, rect rc, color color)
//     {
//         float r = min(rc.width, rc.height) / 2.f;
//         float x = rc.x + rc.width / 2.f;
//         float y = rc.y + rc.height / 2.f;
//         set_color(cvs, fill_style, color);
//         cvs.begin_path();
//         cvs.arc(x, y, r, 0, 2 * pi);
//         cvs.fill();
//     }

//     void draw_circle(canvas &cvs, rect rc, color color)
//     {
//         float r = min(rc.width, rc.height) / 2.f - .5f;
//         float x = rc.x + rc.width / 2.f;
//         float y = rc.y + rc.height / 2.f;
//         set_color(cvs, stroke_style, color);
//         cvs.begin_path();
//         cvs.arc(x, y, r, 0, 2 * pi);
//         cvs.stroke();
//     }

//     void clip_rect(canvas &cvs, rect r)
//     {
//         cvs.begin_path();
//         cvs.rectangle((float)r.x, (float)r.y, (float)r.width, (float)r.height);
//         cvs.clip();
//     }

//     // without scaling
//     void draw_image(canvas &cvs, int x, int y, const Bitmap &bmp)
//     {
//         cvs.draw_image((byte *)bmp.data.data(), bmp.width, bmp.height, bmp.width * 4, (float)x, (float)y, (float)bmp.width, (float)bmp.height);
//     }

//     // with scaling
//     void draw_image(canvas &cvs, rect rc, const Bitmap &bmp)
//     {
//         cvs.draw_image((byte *)bmp.data.data(), bmp.width, bmp.height, bmp.width * 4, (float)rc.x, (float)rc.y, (float)rc.width, (float)rc.height);
//     }

//     void add_color_stop(canvas &cvs, brush_type type, float offset, color c, optional<float> hint)
//     {
//         cvs.add_color_stop(type, offset, c.r / 255.f, c.g / 255.f, c.b / 255.f, c.a / 255.f, hint);
//     }

//     bool set_font(canvas &cvs, const string &raw_font_data, int pixel_size)
//     {
//         return cvs.set_font((byte *)raw_font_data.data(), (int)raw_font_data.size(), (float)pixel_size);
//     }

//     void fill_polygon(canvas &cvs, vector<xy> points, color color)
//     {
//         cvs.begin_path();
//         cvs.polygon(points);
//         set_color(cvs, fill_style, color);
//         cvs.fill();
//     }

//     Bitmap::Bitmap(canvas &canvas) : Bitmap(canvas.width(), canvas.height())
//     {
//         canvas.get_image_data((byte *)data.data(), width, height, width * 4, 0, 0);
//     }

//     color Bitmap::get_pixel(int x, int y) const
//     {
//         if (x < 0 || x >= width || y < 0 || y >= height)
//             return black;
//         else
//             return data[x + y * width];
//     }

//     void Bitmap::set_pixel(int x, int y, color color)
//     {
//         if (x < 0 || x >= width || y < 0 || y >= height)
//             return;
//         if (color.a == 0)
//             return;
//         data[x + y * width] = color;
//     }

//     // endpoint is not drawn, like in GDI
//     void Bitmap::draw_line(int x0, int y0, int x1, int y1, color color)
//     {
//         if (x0 != x1 && y0 != y1)
//             return; // only horz and vert lines supported

//         if (x0 == x1) // vert line
//         {
//             if (y0 > y1)
//                 swap(y0, y1);
//             for (int y = y0; y < y1; y++)
//                 set_pixel(x0, y, color);
//         }
//         else if (y0 == y1) // horz line
//         {
//             if (x0 > x1)
//                 swap(x0, x1);
//             for (int x = x0; x < x1; x++)
//                 set_pixel(x, y0, color);
//         }
//     }

//     void Bitmap::draw_rect(int x, int y, int _width, int _height, color color)
//     {
//         draw_line(x, y, x + _width, y, color);                             // top
//         draw_line(x, y + _height - 1, x + _width, y + _height - 1, color); // bottom
//         draw_line(x, y, x, y + _height, color);                            // left
//         draw_line(x + _width - 1, y, x + _width - 1, y + _height, color);  // right
//     }

//     void Bitmap::fill_rect(rect rect, color color)
//     {
//         for (int y = rect.top(); y < rect.bottom(); y++)
//             for (int x = rect.left(); x < rect.right(); x++)
//                 set_pixel(x, y, color);
//     }

//     void Bitmap::replace_color(color original, color replacement)
//     {
//         for (auto &pixel : data)
//         {
//             if (pixel == original)
//                 pixel = replacement;
//         }
//     }

//     // find minimal rectangle containing pixels different from bgcolor
//     rect Bitmap::find_picture(color bgcolor)
//     {
//         auto horz_line_empty = [&](int y)
//         {
//             for (int x = 0; x < width; x++)
//                 if (data[x + y * width] != bgcolor)
//                     return false;
//             return true;
//         };
//         auto vert_line_empty = [&](int x)
//         {
//             for (int y = 0; y < height; y++)
//                 if (data[x + y * width] != bgcolor)
//                     return false;
//             return true;
//         };

//         rect rect;
//         int y;
//         for (y = 0; y < height && horz_line_empty(y); y++)
//             ;
//         if (y == height)
//             return rect; // no picture
//         rect.y = y;
//         for (y = height - 1; y >= 0 && horz_line_empty(y); y--)
//             ;
//         rect.height = y + 1 - rect.y;

//         int x;
//         for (x = 0; x < width && vert_line_empty(x); x++)
//             ;
//         rect.x = x;
//         for (x = width - 1; x >= 0 && vert_line_empty(x); x--)
//             ;
//         rect.width = x + 1 - rect.x;

//         return rect;
//     }

//     void Bitmap::load(string filename)
//     {
//         vector<byte> image;
//         unsigned w, h;
//         lodepng::decode(image, w, h, filename);
//         if (w * h == 0)
//             return;

//         width = w;
//         height = h;
//         data.resize(w * h);
//         memcpy(data.data(), image.data(), w * h * 4);
//     }

//     void Bitmap::save(string filename)
//     {
//         lodepng::encode(filename, (byte *)data.data(), width, height);
//     }

//     // This function can be used to compare gradient rendering between different browsers.
//     byte max_color_diff(const Bitmap &a, const Bitmap &b)
//     {
//         if (a.width != b.width || a.height != b.height)
//             return 255;

//         int diff = 0;
//         for (int y = 0; y < a.height; y++)
//             for (int x = 0; x < a.width; x++)
//             {
//                 color A = a.get_pixel(x, y);
//                 color B = b.get_pixel(x, y);
//                 diff = max({diff, abs(A.r - B.r), abs(A.g - B.g), abs(A.b - B.b), abs(A.a - B.a)});
//             }
//         return (byte)diff;
//     }

// }