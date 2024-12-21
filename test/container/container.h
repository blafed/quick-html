#include "litehtml.h"
#include "test.h"
#include "canvas_ity.hpp"

// namespace demo_container
// {
//     using namespace litehtml;
//     using namespace canvas_ity;
//     using rect = position;
//     using byte = litehtml::byte;
//     using size = litehtml::size;

//     struct color
//     {
//         byte r, g, b, a;

//         color();
//         color(byte r, byte g, byte b, byte a);
//         color(web_color c);

//         bool operator==(color c) const;
//         bool operator!=(color c) const;
//     };

//     extern const color white;
//     extern const color black;
//     extern const color transparent;

//     class Bitmap
//     {
//     public:
//         int width;
//         int height;
//         vector<color> data;

//         Bitmap();
//         Bitmap(int width, int height, color color = white);
//         Bitmap(string filename);
//         Bitmap(canvas &canvas);

//         bool operator==(const Bitmap &bmp) const;
//         bool operator!=(const Bitmap &bmp) const;
//         operator bool() const;

//         color get_pixel(int x, int y) const;
//         void set_pixel(int x, int y, color color);
//         void draw_line(int x0, int y0, int x1, int y1, color color);
//         void draw_rect(int x, int y, int width, int height, color color);
//         void fill_rect(rect rect, color color);
//         void replace_color(color original, color replacement);

//         rect find_picture(color bgcolor = white);
//         void load(string filename);
//         void save(string filename);
//     };

//     class test_container : public document_container
//     {
//     public:
//         int width;
//         int height;
//         string basedir;
//         std::map<string, Bitmap> images;

//         test_container(int width, int height, string basedir) : width(width), height(height), basedir(basedir) {}

//         string make_url(const char *src, const char *baseurl);

//         uint_ptr create_font(const char *faceName, int size, int weight, font_style italic, unsigned int decoration, font_metrics *fm) override;
//         void delete_font(uint_ptr /*hFont*/) override {}
//         int text_width(const char *text, uint_ptr hFont) override;
//         void draw_text(uint_ptr hdc, const char *text, uint_ptr hFont, web_color color, const position &pos) override;
//         int pt_to_px(int pt) const override;
//         int get_default_font_size() const override;
//         const char *get_default_font_name() const override;
//         void load_image(const char *src, const char *baseurl, bool redraw_on_ready) override;
//         void get_image_size(const char *src, const char *baseurl, size &sz) override;
//         void draw_image(uint_ptr hdc, const background_layer &layer, const string &url, const string &base_url) override;
//         void draw_solid_fill(uint_ptr hdc, const background_layer &layer, const web_color &color) override;
//         void draw_linear_gradient(uint_ptr hdc, const background_layer &layer, const background_layer::linear_gradient &gradient) override;
//         void draw_radial_gradient(uint_ptr hdc, const background_layer &layer, const background_layer::radial_gradient &gradient) override;
//         void draw_conic_gradient(uint_ptr hdc, const background_layer &layer, const background_layer::conic_gradient &gradient) override;
//         void draw_borders(uint_ptr hdc, const borders &borders, const position &draw_pos, bool root) override;
//         void draw_list_marker(uint_ptr hdc, const list_marker &marker) override;
//         element::ptr create_element(const char * /*tag_name*/,
//                                     const string_map & /*attributes*/,
//                                     const document::ptr & /*doc*/) override { return nullptr; }
//         void get_media_features(media_features &media) const override;
//         void get_language(string & /*language*/, string & /*culture*/) const override {}
//         void link(const document::ptr & /*doc*/, const element::ptr & /*el*/) override {}

//         void transform_text(string & /*text*/, text_transform /*tt*/) override {}
//         void set_clip(const position & /*pos*/, const border_radiuses & /*bdr_radius*/) override {}
//         void del_clip() override {}

//         void set_caption(const char * /*caption*/) override {}
//         void set_base_url(const char * /*base_url*/) override {}
//         void on_anchor_click(const char * /*url*/, const element::ptr & /*el*/) override {}
//         void on_mouse_event(const element::ptr & /*el*/, mouse_event /*event*/) override {};
//         void set_cursor(const char * /*cursor*/) override {}
//         void import_css(string &text, const string &url, string &baseurl) override;
//         void get_client_rect(position &client) const override;
//     };

//     byte max_color_diff(const Bitmap &a, const Bitmap &b);

//     string readfile(string filename);
//     void draw_image(canvas &cvs, int x, int y, const Bitmap &bmp);
//     void set_color(canvas &cvs, brush_type type, color c);
//     void fill_rect(canvas &cvs, rect r);
//     void fill_rect(canvas &cvs, rect r, color color);
//     void fill_circle(canvas &cvs, rect rc, color color);
//     void draw_circle(canvas &cvs, rect rc, color color);
//     void clip_rect(canvas &cvs, rect r);
//     void draw_image(canvas &cvs, int x, int y, const Bitmap &bmp);
//     void draw_image(canvas &cvs, rect rc, const Bitmap &bmp);
//     void add_color_stop(canvas &cvs, brush_type type, float offset, color c, optional<float> hint);
//     bool set_font(canvas &cvs, const string &raw_font_data, int pixel_size);
//     void fill_polygon(canvas &cvs, vector<xy> points, color color);

// }