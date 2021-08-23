#include "../globals.h"
#include "stereokit.h"

using namespace sk;

sk::material_t fieldDebugMat;
sk::mesh_t boxFieldMesh;

void debugSetup() {
	fieldDebugMat = material_copy(material_find(default_id_material));
	material_set_color(fieldDebugMat, "color", {0, 1, 0.69921875, 0.25f});
	material_set_transparency(fieldDebugMat, transparency_blend);
	material_set_depth_write(fieldDebugMat, false);

	boxFieldMesh = mesh_gen_cube(vec3_one);
}
