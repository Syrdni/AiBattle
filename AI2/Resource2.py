from AICore2 import AICore
from Pos2 import Pos

class Resource:
	def __init__(self, id, pos, type = None):
		self.id = id
		self.pos = pos
		self.worker = None
		self.type = type