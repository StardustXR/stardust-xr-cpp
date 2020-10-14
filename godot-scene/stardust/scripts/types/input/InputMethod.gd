extends Spatial

var sessionID := 0
var rejectAction := false
var datamap := {}
var origin := Vector3.ZERO

func setPosition(_sessionID: int, pos: Vector3):
	origin = pos
	set_translation(pos)

func modifyDatamap(_sessionID: int, data: Dictionary):
	for key in data.keys():
		datamap[key] = data[key]
