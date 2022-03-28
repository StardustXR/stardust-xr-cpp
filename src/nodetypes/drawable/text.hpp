#pragma once

#include "drawable.hpp"

namespace StardustXRServer {

class Text : public Drawable {
public:
	Text(Client *client, Spatial *spatialParent, sk::pose_t transform, std::string text, std::string font, float characterHeight, sk::text_align_ textAlign, sk::vec2 bounds, sk::text_fit_ fit, sk::text_align_ boundsAlign, sk::color128 color);
	~Text();

	void draw();

protected:
	static sk::font_t defaultFont;
	static std::map<sk::font_t, sk::text_style_t> fontStyles;

	bool reloadFont = true;
	sk::vec2 size = sk::vec2_zero;

	std::string fontPath;
	sk::font_t font;
	sk::text_style_t style;

	std::string text;
	float characterHeight;
	sk::text_align_ textAlign;
	sk::vec2 bounds;
	sk::text_fit_ fit;
	sk::text_align_ boundsAlign;
	sk::color128 color;
};

}
