#include "mesh.hpp"
using namespace sk;

namespace StardustXRServer {

TestMeshNode::TestMeshNode() {
	material = material_find(default_id_material);
	mesh = mesh_gen_cube(vec3_one*0.1, 1);
}

void TestMeshNode::update() {}

void TestMeshNode::draw() {
	render_add_mesh(mesh, material, matrix_trs(vec3_forward, quat_identity, vec3_one));
}

} // namespace StardustXRServer
