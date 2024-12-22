#include "Bitmap.h"
#include "canvas_ity.hpp"
using size = litehtml::size;
using byte = litehtml::byte;
class Font : public font_metrics
{
public:
	static Font *create(string face, int size, int weight);
	virtual int text_width(string text) = 0;
	virtual void draw_text(canvas &canvas, string text, color color, int x, int y) = 0;
};

class OutlineFont : public Font
{
	string name;
	string data;  // raw contents of ttf file
	int size = 0; // pixels per em
public:
	OutlineFont(string name, int size);

	static string_map installed_fonts;
	static OutlineFont *create(string name, int size);

	int text_width(string text) override;
	void draw_text(canvas &canvas, string text, color color, int x, int y) override;
};
