extends Node

var pointerInputScene = load("res://stardust/objects/input/PointerInput.tscn")
var inputHandlerScene = load("res://stardust/objects/input/InputHandler.tscn")

var input_methods := []
var input_handlers := []

func _ready():
	input_methods.append(get_node("methods/cursor_pointer"))

func registerPointerInput(_sessionID: int, name: String, origin: Vector3, direction: Vector3, tilt: float):
	var pointerInput : InputMethod = pointerInputScene.instance()
	get_child(1).add_child(pointerInput)
	
	pointerInput.sessionID = _sessionID
	pointerInput.name = name
	pointerInput.setPose(origin, direction, tilt)
	
	input_methods.append(pointerInput)

func registerInputHandler(_sessionID: int, name: String, field: String, callbackPath: String, callbackMethod: String):
	var inputHandler : InputHandler = inputHandlerScene.instance()
	get_child(0).add_child(inputHandler)
	
	inputHandler.sessionID = _sessionID
	inputHandler.name = name
	inputHandler.field = get_node("../field/"+field)
	inputHandler.callbackPath = callbackPath
	inputHandler.callbackMethod = callbackMethod
	
	input_handlers.append(inputHandler)

func sort_distance_links(a, b):
	if abs(a[1]) < abs(b[1]):
		return true
	return false

func _process(delta):
	var distance_links := []
	
	
	# Create distance links from method/distance/handlers
	for input_handler in input_handlers:
		for input_method in input_methods:
			distance_links.append([
				input_method,
				input_method.distance_to_field(input_handler.field),
				input_handler
			])
	
	# Sort by closest distance to surface first
	distance_links.sort_custom(self, "sort_distance_links")
	
	# Repeat this until every single input handler is added to a running chain
	while distance_links.size() != 0:
		var first_distance_link = distance_links.pop_front()
		var first_input_method = first_distance_link[0]
		var first_input_distance = first_distance_link[1]
		var first_input_handler = first_distance_link[2]
		
		var distance_link_chain = []
		
		# Create an input handler chain for each input method to asynchronusly propagate input
		for i in range(0,distance_links.size()-1):
			if distance_links[i][0] == first_input_method:
				distance_link_chain.append(distance_links[i])
				distance_links.remove(i)
		
		var serialized_data = first_input_method.serialize()
		serialized_data["distance"] = first_input_distance
		
		# Start the chain and send the serialized data to with the closest input handler
		first_input_handler.send_input(0, distance_link_chain, serialized_data, false)
