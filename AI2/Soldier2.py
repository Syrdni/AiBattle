from AICore2 import AICore
from BaseGameEntity2 import BaseGameEntity
from EntityManager2 import EntityManager
from BuildingManager2 import BuildingManager
from State2 import State
from Pos2 import Pos

import random

# Solider class
class Soldier(BaseGameEntity):
	# Guard range
	guardRange = 1
	
	# Create soldier
	def __init__(self, id):
		super().__init__(id)
		EntityManager.add(self, 'soldier')
		self.worker = None
		self.enemy = None
		self.changeState(Guarding)
		
	# Handels messages
	def onMessage(self, message):
		if message['type'] == 'tasksComplete' or message['type'] == 'attackFailed' or message['type'] == 'canAttack':
			if BuildingManager.enemyCastle:
				self.changeState(AttackBase)
		
			#if self.state == LifeGuarding:
			#	self.changeState(LifeGuarding)
			#elif self.state == AttackBase:
			#	self.changeState(AttackBase)
			#elif self.state == Attack:
			#	self.changeState(Attack)
		
	# Attack entity
	def attack(self, target):
		AICore.Attack(self.id, target)

# Guard the castle
class Guarding(State):
	def execute(soldier):
		if BuildingManager.enemyCastle:
			soldier.changeState(AttackBase)
		else:
			pass
			#castle = BuildingManager.getCastle()
			#x = random.randint(castle.x - Soldier.guardRange, castle.x + Soldier.guardRange)
			#z = random.randint(castle.z - Soldier.guardRange, castle.z + Soldier.guardRange)
			#soldier.setTarget(Pos(x, z))

# Guard a worker
class LifeGuarding(State):
	def enter(soldier):
		soldier.setTarget(soldier.worker.position)

# Attack enemy
class Attack(State):
	def enter(soldier):
		soldier.attack(soldier.enemy.id)

	def execute(soldier):
		# Stop attacking
		if soldier.worker:
			if Pos.getDistance(soldier.getPosition(), soldier.worker.getPosition()) > Soldier.guardRange:
				soldier.changeState(LifeGuarding)
		elif Pos.getDistance(soldier.getPosition(), BuildingManager.getCastle()) > Soldier.guardRange:
			soldier.changeState(Guarding)

# Attack enemy castle
class AttackBase(State):
	def enter(soldier):
		soldier.attack(BuildingManager.enemyCastle.id)