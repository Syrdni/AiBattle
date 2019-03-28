from AICore2 import AICore
from State2 import State
from Pos2 import Pos

# Base class for all entities
class BaseGameEntity:
	def __init__(self, id):
		self.id = id
		self.state = State
		self.pos = AICore.GetPos(id)
		
	# Set a target for the pathfinding
	def setTarget(self, pos):
		AICore.MoveTo(self.id, int(pos.x), int(pos.z))
		
	# Get position
	def getPosition(self):
		return Pos.tilePos(self.pos)
		
	# Handles messages
	def onMessage(self, message):
		pass
	
	# execute current state
	def execute(self):
		self.state.execute(self)

	# Change state
	def changeState(self, newState):
		self.state.exit(self)
		self.state = newState
		self.state.enter(self)