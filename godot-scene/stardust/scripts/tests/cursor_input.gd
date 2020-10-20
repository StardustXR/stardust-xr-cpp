extends PointerInput

func _physics_process(_delta):
	datamap["select"] = int(Input.is_mouse_button_pressed(BUTTON_LEFT))
	datamap["grab"] = int(Input.is_mouse_button_pressed(BUTTON_RIGHT))
	datamap["scroll"] = int(Input.is_mouse_button_pressed(BUTTON_WHEEL_UP)) - int(Input.is_mouse_button_pressed(BUTTON_WHEEL_UP)) * 0.5
	
	origin = get_viewport().get_camera().get_translation()
	direction = get_viewport().get_camera().project_ray_normal(get_viewport().get_mouse_position()).normalized()
	tilt = 0.0
