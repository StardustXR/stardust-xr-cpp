#include "../globals.hpp"
#include "stereokit.h"

using namespace sk;

sk::material_t fieldDebugMat;
sk::mesh_t boxFieldMesh;
sk::mesh_t cylinderFieldMesh;
sk::mesh_t sphereFieldMesh;

void debugSetup() {
	fieldDebugMat = material_copy(material_find(default_id_material));
	material_set_color(fieldDebugMat, "color", {0, 1, 0.69921875, 0.25f});
	material_set_transparency(fieldDebugMat, transparency_blend);
	material_set_depth_write(fieldDebugMat, false);

	boxFieldMesh = mesh_gen_cube(vec3_one);
	cylinderFieldMesh = mesh_gen_cylinder(2, 1, vec3_forward, 32);
	sphereFieldMesh = mesh_gen_sphere(2, 4);
}
