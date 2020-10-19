class_name BoxField
extends "res://stardust/scripts/types/field/Field.gd"

export var size = Vector3.ONE

func setSize(_sessionID: int, size: Vector3):
	self.size = size

func localDistance(offsetPoint: Vector3) -> float:
	offsetPoint.x = abs(offsetPoint.x) - (size.x * 0.5)
	offsetPoint.y = abs(offsetPoint.y) - (size.y * 0.5)
	offsetPoint.z = abs(offsetPoint.z) - (size.z * 0.5)

	var outsideVector := Vector3.ZERO
	outsideVector.x = max(offsetPoint.x, 0);
	outsideVector.y = max(offsetPoint.y, 0);
	outsideVector.z = max(offsetPoint.z, 0);

	var outsideDistance := outsideVector.length()
	var insideDistance := min(max(offsetPoint.x, max(offsetPoint.y, offsetPoint.z)), 0.0)

	return outsideDistance
