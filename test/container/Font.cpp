#define _CRT_SECURE_NO_WARNINGS
#include "Font.h"
using namespace canvas_ity;
string readfile(string filename);
void draw_image(canvas &cvs, int x, int y, const Bitmap &bmp);
bool set_font(canvas &cvs, const string &raw_font_data, int pixel_size);
void set_color(canvas &cvs, brush_type type, color c);

string get_font_dir()
{
	return "res/";
	string font_cpp = __FILE__;
	auto i = font_cpp.find_last_of("\\/");
	return font_cpp.substr(0, i) + "/fonts/";
}

// keys must be in lowcase
string_map OutlineFont::installed_fonts =
	{
		{"roboto", "roboto.ttf"},
		{"ahem", "ahem.ttf"}};
;

OutlineFont *OutlineFont::create(string name, int size)
{
	lcase(name);

	if (installed_fonts.count(name))
		return new OutlineFont(name, size);

	return nullptr;
}

OutlineFont::OutlineFont(string name, int size) : name(name), size(size)
{
	string filename = at(installed_fonts, name);
	data = readfile(get_font_dir() + filename);

	canvas cvs;
	set_font(cvs, data, size);
	cvs.get_font_metrics(ascent, descent, height, x_height);
}

int OutlineFont::text_width(string text)
{
	canvas cvs;
	set_font(cvs, data, size);
	return (int)ceil(cvs.measure_text(text.c_str()));
}

void OutlineFont::draw_text(canvas &cvs, string text, color color, int x, int y)
{
	set_font(cvs, data, size);
	cvs.text_baseline = top;
	set_color(cvs, fill_style, color);
	cvs.fill_text(text.c_str(), (float)x, (float)y);
}

Font *Font::create(string name, int size, int weight)
{
	return OutlineFont::create(name, size);
}
