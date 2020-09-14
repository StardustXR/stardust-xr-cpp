extends Node

var logicStepSubscriptions := []

func subscribeLogicStep(sessionID: int, path: String, method: String):
	logicStepSubscriptions.append([sessionID, path, method, []])

func _process(delta):
	if logicStepSubscriptions.size() > 0:
		for logicStepSubscription in logicStepSubscriptions:
			logicStepSubscription[3] = [delta, 0]
			get_parent().callv("send_signal", logicStepSubscription)
