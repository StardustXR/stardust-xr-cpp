import os
import threading
from time import sleep
from stardust_xr import messenger
from godot import exposed, export, Node, NodePath

@exposed
class stardust_messenger(Node, Messenger):
	pass
