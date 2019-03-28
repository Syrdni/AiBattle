from AICore2 import AICore
from Pos2 import Pos

class Building:
	def __init__(self, id, pos, craftsmanId = None):
		self.id = id
		self.pos = pos
		self.type = None
		self.craftsmanId = craftsmanId