from AICore2 import AICore
from Pos2 import Pos

class Enemy:
	def __init__(self, id):
		self.id = id
		self.pos = AICore.GetPos(id)
		
	def getPosition(self):
		return Pos.tilePos(self.pos)