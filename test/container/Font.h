#include "Bitmap.h"
#include "canvas_ity.h"

using namespace canvas_ity;


string readfile(string filename);
void draw_image(canvas &cvs, int x, int y, const Bitmap &bmp);
bool set_font(canvas &cvs, const string &raw_font_data, int pixel_size);
void set_color(canvas &cvs, brush_type type, color c);

class Font : public font_metrics
{
public:
	static Font* create(string face, int size, int weight);
	virtual int text_width(string text) = 0;
	virtual void draw_text(canvas& canvas, string text, color color, int x, int y) = 0;
};

class RasterFont : public Font
{
	int width = 0; // for absent glyphs, see get_glyph
	std::map<int, Bitmap> glyphs;
public:
	RasterFont(int size, int weight);

	static struct size_name { int size; string name; } installed_fonts[];
	static RasterFont* create(string face, int size, int weight);

	Bitmap get_glyph(int ch, color color);
	void load(string filename);

	int text_width(string text) override;
	void draw_text(canvas& canvas, string text, color color, int x, int y) override;
};

class OutlineFont : public Font
{
	string name;
	string data; // raw contents of ttf file
	int size = 0; // pixels per em
public:
	OutlineFont(string name, int size);

	static string_map installed_fonts;
	static OutlineFont* create(string name, int size);

	int text_width(string text) override;
	void draw_text(canvas& canvas, string text, color color, int x, int y) override;
};
