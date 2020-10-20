class_name PointerInput
extends InputMethod

const MAX_RAY_STEPS := 1000
const MAX_RAY_LENGTH := 1000

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

func distance_to_field(field: Field) -> float:
	var min_distance := INF
	
	var ray_steps := 0
	var ray_length := 0
	var ray_point := origin
	
	while ray_steps < MAX_RAY_STEPS and ray_length < MAX_RAY_LENGTH:
		ray_steps += 1
		
		var distance: float = field.distance(0, ray_point)
		min_distance = min(min_distance, ray_length+distance)
		
		var march_distance := clamp(distance, MIN_RAY_MARCH, MAX_RAY_MARCH)
		ray_point += march_distance * direction
		
		ray_length += march_distance
	
	return min_distance

func serialize() -> Dictionary:
	return {
		"type": 2,
		"origin": origin,
		"direction": direction,
		"tilt": tilt,
		"datamap": datamap
	}
