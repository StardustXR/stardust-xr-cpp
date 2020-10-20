class_name InputHandler
extends Node

var sessionID := 0

var field: Field
var callbackPath: String
var callbackMethod: String

func setField(_sessionID: int, field: String):
	self.field = get_node("../../../fields/"+field)

func setCallback(_sessionID: int, callbackPath: String, callbackMethod: String):
	self.callbackPath = callbackPath
	self.callbackMethod = callbackMethod

func send_input(_sessionID: int, distance_link_chain: Array, inputData: Dictionary, captured: bool):
	if !captured:
		if distance_link_chain.size() > 0:
			get_node("../../../").callv("execute_remote_method", [
				sessionID,
				self.callbackPath,
				self.callbackMethod,
				inputData,
				"/input/handlers/"+distance_link_chain.pop_front()[2].get_name(),
				"send_input",
				[distance_link_chain, inputData]
			])
		else:
			get_node("../../../").callv("send_signal", [
				sessionID,
				self.callbackPath,
				self.callbackMethod,
				inputData
			])
