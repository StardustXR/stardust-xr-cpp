extends Spatial


func _ready():
	return
	var arvr_interface = ARVRServer.find_interface("OpenXR")
	if arvr_interface and arvr_interface.initialize():
		# list devices
		print("AR VR Interface initialized")

		# and tell our viewport to render
		get_viewport().arvr = true
		print("Started ARVR")
		
		Engine.iterations_per_second = 120


func _process(_delta):
	# Test for escape to close application, space to reset our reference frame
	if (Input.is_key_pressed(KEY_ESCAPE)):
		get_tree().quit()
	elif (Input.is_key_pressed(KEY_SPACE)):
		# Calling center_on_hmd will cause the ARVRServer to adjust all tracking data so the player is centered on the origin point looking forward
		ARVRServer.center_on_hmd(true, true)

