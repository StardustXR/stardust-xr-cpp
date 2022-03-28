#include "text.hpp"

using namespace sk;

namespace StardustXRServer {

sk::font_t Text::defaultFont = 0;
std::map<font_t, text_style_t> Text::fontStyles;

Text::Text(Client *client, Spatial *spatialParent, pose_t transform, std::string text, std::string font, float characterHeight, text_align_ textAlign, vec2 bounds, text_fit_ fit, text_align_ boundsAlign, color128 color) :
Drawable(client, spatialParent, transform, true, true, false),
fontPath(font),
characterHeight(characterHeight),
size(bounds),
text(text),
textAlign(textAlign),
bounds(bounds),
fit(fit),
boundsAlign(boundsAlign),
color(color) {
	if(!defaultFont)
		defaultFont = font_find(default_id_font);
	if(bounds.x == 0 && bounds.y == 0)
		size = text_size(text.c_str(), style) * characterHeight;
}
Text::~Text() {

	font_release(font);
}

void Text::draw() {
	if(reloadFont) {
		if(font && fontStyles.find(font) != fontStyles.end()) {
			style = fontStyles[font];
		} else if(fontPath != "") {
			font_t oldFont = font;
			font = font_create(fontPath.c_str());
			if(!font) {
				font = oldFont;
				goto reloadFailed;
			}
		} else {
			font = defaultFont;
			font_addref(defaultFont);
		}

		style = text_make_style(font, 1, color128{1, 1, 1, 1});
		material_set_cull(text_style_get_material(style), cull_back);
		fontStyles[font] = style;

		reloadFailed:
		reloadFont = false;
	}

	if(bounds.x == 0 && bounds.y == 0)
		text_add_at(text.c_str(), matrix_s(vec3_one * characterHeight) * worldTransform(), style, boundsAlign, textAlign, 0, 0, 0, color);
	else
		size.y = text_add_in(text.c_str(), matrix_s(vec3_one * characterHeight) * worldTransform(), bounds / characterHeight, fit, style, boundsAlign, textAlign, 0, 0, 0, color) * characterHeight;
}

}
