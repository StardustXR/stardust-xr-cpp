class_name SphereField
extends Field

export var radius := 0.0

func setRadius(_sessionID: int, radius: float):
	self.radius = radius

func localDistance(point: Vector3) -> float:
	return point.length() - radius

func localNormal(point: Vector3, radius: float) -> Vector3:
	return point.normalized()
