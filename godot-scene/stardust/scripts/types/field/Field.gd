extends Spatial

export var normalRadius : float = 0.0001
var supportsInsideDistance := true

var localPoint : Vector3
var globalPoint : Vector3

# Property setting for fields
func setOrigin(_sessionID: int, origin: Vector3):
	set_translation(origin)

func setOrientation(_sessionID: int, orientation: Quat):
	set_rotation(orientation.get_euler())

func setPose(_sessionID: int, origin: Vector3, orientation: Quat):
	setOrigin(_sessionID, origin)
	setOrientation(_sessionID, orientation)


# Virtual functions to define distance per field type
func localDistance(point: Vector3) -> float:
	return point.length()

func localNormal(point: Vector3, radius: float) -> Vector3:
	var d := localDistance(point)
	var e := Vector2(radius, 0)

	var n := Vector3(d, d, d) - Vector3(
		localDistance(Vector3(e.x, e.y, e.y)),
		localDistance(Vector3(e.y, e.x, e.y)),
		localDistance(Vector3(e.y, e.y, e.x)))

	return n.normalized()

func localClosestPoint(point: Vector3, radius: float) -> Vector3:
	return point - (localNormal(point, radius) * localDistance(point))


# World space functions
func distance(_sessionID: int, point: Vector3) -> float:
	globalPoint = point
	localPoint = to_local(point)

	if get_scale() == Vector3.ONE:
		# Unscaled
		return localDistance(localPoint)
	elif get_scale().x == get_scale().y && get_scale().y == get_scale().z:
		# Uniformly scaled
		var scaleFactor := get_scale().x
		return localDistance(localPoint * scaleFactor)
	else:
		# Non-uniformly scaled, slow
		var localSurfacePoint := localClosestPoint(localPoint, normalRadius)
		var surfacePoint := to_global(localSurfacePoint)
		var distance := distance(0, globalPoint - surfacePoint)

		# Don't calculate the distance for its sign if we already know the field doesn't support it
		if supportsInsideDistance || distance == 0.0:
			return distance
		else:
			var localScaleDistance := localDistance(localPoint)
			return distance * sign(localScaleDistance)

func normal(_sessionID: int, point: Vector3, radius: float) -> Vector3:
	globalPoint = point
	localPoint = to_local(point)

	var localDirection := localNormal(point, radius)
	return to_global(localDirection).normalized()

func closestPoint(_sessionID: int, point: Vector3) -> Vector3:
	globalPoint = point
	localPoint = to_local(point)

	var localNearestPoint := localClosestPoint(point, normalRadius)
	return to_global(localNearestPoint)
