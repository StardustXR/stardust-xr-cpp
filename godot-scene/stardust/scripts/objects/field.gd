extends Node

var boxFieldScene = load("res://stardust/objects/fields/BoxField.tscn")
var sphereFieldScene = load("res://stardust/objects/fields/SphereField.tscn")

func createBoxField(_sessionID: int, name: String, origin: Vector3, orientation: Quat, size: Vector3):
	var boxField = boxFieldScene.instance()
	add_child(boxField)
	
	boxField.name = name
	boxField.set_translation(origin)
	boxField.set_rotation(orientation.get_euler())
	boxField.size = size

func createSphereField(_sessionID: int, name: String, origin: Vector3, radius: float):
	var sphereField = sphereFieldScene.instance()
	add_child(sphereField)
	
	sphereField.name = name
	sphereField.set_translation(origin)
	sphereField.radius = radius
