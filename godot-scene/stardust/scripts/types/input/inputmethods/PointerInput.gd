class_name PointerInput
extends "res://stardust/scripts/types/input/InputMethod.gd"

const MAX_RAY_STEPS := 1000

const MIN_RAY_MARCH := 0.001
const MAX_RAY_MARCH := INF

var direction := Vector3.FORWARD
var tilt := 0.0

func setDirection(_sessionID: int, direction: Vector3):
	self.direction = direction
	update_transform()

func setTilt(_sessionID: int, tilt: float):
	self.tilt = tilt
	update_transform()

func setOrientation(_sessionID: int, direction: Vector3, tilt: float):
	self.direction = direction
	self.tilt = tilt
	update_transform()
	
func setPose(_sessionID: int, origin: Vector3, direction: Vector3, tilt: float):
	self.origin = origin
	self.direction = direction
	self.tilt = tilt
	update_transform()

func update_transform():
	look_at_from_position(origin, origin+direction, Vector3.UP)
	rotate_object_local(Vector3.BACK, tilt) 

func distanceToField(field: String) -> float:
	var distance := 0.0
	var ray_steps := 0
	
	var ray_point := origin
#	get_node("../../field/"+field).callv()
	
	return distance
