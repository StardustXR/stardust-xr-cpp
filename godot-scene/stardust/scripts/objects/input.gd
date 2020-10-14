extends Node

var pointerInputScene = load("res://stardust/objects/inputmethods/PointerInput.tscn")

func registerPointerInput(_sessionID: int, name: String, origin: Vector3, direction: Vector3, tilt: float):
	var pointerInput = pointerInputScene.instance()
	add_child(pointerInput)
	
	pointerInput.sessionID = _sessionID
	pointerInput.name = name
	pointerInput.setPose(origin, direction, tilt)
