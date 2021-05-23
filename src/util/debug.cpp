#include "../globals.h"

using namespace sk;

sk::material_t fieldDebugMat;

void debugSetup() {
	fieldDebugMat = material_copy(material_find(default_id_material));
	material_set_color(fieldDebugMat, "color", {0, 1, 0.69921875, 0.25f});
	material_set_transparency(fieldDebugMat, transparency_blend);
}
