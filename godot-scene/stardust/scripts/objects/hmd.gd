extends Spatial


func setPosition(pos: Vector3):
	set_translation(pos)

func setRotation(rot: Quat):
	set_rotation(rot.get_euler())

func setPose(pos: Vector3, rot: Quat):
	setPosition(pos)
	setRotation(rot)
