from AICore2 import AICore
from BaseGameEntity2 import BaseGameEntity
import EntityManager2
from State2 import State
from Pos2 import Pos
import random

# Class for explorers
class Explorer(BaseGameEntity):
	# Create explorer
	def __init__(self, id):
		super().__init__(id)
		EntityManager2.EntityManager.add(self, 'explorer')
		self.charge = False
		self.target = None
		EntityManager2.EntityManager.setExploreArea(self)
		self.x = self.x1
		self.z = 97
		self.dz = -1
		
		self.changeState(Explore)
	
	# Handles messages
	def onMessage(self, message):
		if message['type'] == 'tasksComplete':
			if not self.charge:
				self.changeState(Explore)
			else:
				self.changeState(Scatter)

# Explore the map
class Explore(State):
	def enter(explorer):
		target = EntityManager2.EntityManager.getExplorePosition(explorer)
		
		if target:
			explorer.setTarget(target)

# Charge strategy
class Charge(State):
	enemyRadius = 3
	chaseRadius = 2

	def enter(unit):
		unit.charge = True
		unit.setTarget(unit.target)

# Scatter strategy
class Scatter(State):
	def enter(unit):
		pos = Pos(
			random.randint(unit.target.x - Charge.enemyRadius, unit.target.x + Charge.enemyRadius),
			random.randint(unit.target.z - Charge.enemyRadius, unit.target.z + Charge.enemyRadius)
		)
		
		unit.setTarget(pos)