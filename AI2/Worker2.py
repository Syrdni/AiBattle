from AICore2 import AICore
from BaseGameEntity2 import BaseGameEntity
from EntityManager2 import EntityManager
from ResourceManager2 import ResourceManager
from StrategyManager2 import StrategyManager
from BuildingManager2 import BuildingManager
from State2 import State
from Pos2 import Pos

from Explorer2 import Explorer, Charge
from Craftsman2 import Craftsman
from Soldier2 import Soldier

import random

# Worker Types
class WorkerTypes:
	explorer = 1
	craftsman = 2
	soldier = 3

# Worker class
class Worker(BaseGameEntity):
	# Create new worker
	def __init__(self, id):
		super().__init__(id)
		self.chopper = False
		self.skipState = False
		EntityManager.add(self, 'worker')
		self.item = None
		self.resource = None
		self.charge = False
		self.upgradeType = None
		self.campId = None
		
		self.changeState(FindWork)
		
	# Handles messages
	def onMessage(self, message):
		if message['type'] == 'upgradeComplete':
			if self.upgradeType == WorkerTypes.explorer:
				Explorer(self.id)
			elif self.upgradeType == WorkerTypes.craftsman:
				Craftsman(self.id)
			elif self.upgradeType == WorkerTypes.soldier:
				Soldier(self.id)
				
			EntityManager.remove(self.id, StrategyManager.team, 'upgrade')
			return
		elif message['type'] == 'dropOffComplete':
			if self.resource.type == 'tree':
				ResourceManager.items['wood'] += 1
			elif self.resource.type == 'iron':
				ResourceManager.items['ironOre'] += 1
				
			self.resource = None
		elif message['type'] == 'harvestFailed':
			self.changeState(FindWork)
			self.resource = None
		
		if not self.charge and message['type'] == 'tasksComplete':
			if self.state == WalkToCamp:
				self.changeState(CreateSoldier)
			else:
				self.changeState(FindWork)
			
		if self.charge and message['type'] == 'tasksComplete':
			self.changeState(Scatter)
		
	# Harvest a resource
	def harvestResource(self, resource):
		resource.worker = self
		self.resource = resource
		AICore.HarvestResource(self.id, resource.id)
		
	# Upgrade worker
	def upgrade(self, type, id = None):
		self.upgradeType = type
		
		if id:
			AICore.Upgrade(self.id, type, id)
		else:
			AICore.Upgrade(self.id, type, 0xFFFFFFFFFFFFFFFF)

# Find work for the worker
class FindWork(State):
	def execute(worker):
		if worker.chopper:
			worker.changeState(ChopTree)
			return
			
		if worker.skipState:
			worker.changeState(CreateCraftsman)
			return
		
		# Create explorers if needed
		if EntityManager.explorerNeeded():
			worker.changeState(CreateExplorer)
			return
		
		# Create craftsman if needed
		elif EntityManager.craftsmanNeeded():
			worker.changeState(CreateCraftsman)
			return
			
		# Create soldier if needed
		elif EntityManager.soldierNeeded():
			worker.changeState(WalkToCamp)
			return
		
		# Gather resources
		if worker.item == None:
			if ResourceManager.resourceNeeded() == 'tree':
				worker.changeState(ChopTree)
			else:
				worker.changeState(MineIron)
		elif worker.item:
			worker.changeState(GoToBase)

# Chop down a tree
class ChopTree(State):
	def enter(worker):
		if len(ResourceManager.trees) > 0:
			resource = ResourceManager.findTree()
			
			if resource:
				worker.harvestResource(resource)
			else:
				worker.changeState(FindWork)
		else:
			worker.changeState(FindWork)

# Mine iron
class MineIron(State):
	def enter(worker):
		if len(ResourceManager.ironOre) > 0:
			resource = ResourceManager.findOre()
			
			if resource:
				worker.harvestResource(resource)
			else:
				worker.changeState(ChopTree)
		else:
			if len(EntityManager.soldiers) >= 3:
				worker.target = BuildingManager.getEnemyCastle()
				
				if worker.target:
					worker.changeState(Charge)
				return
				
			worker.changeState(FindWork)
		
# Grab a dropped item		
class GrabItem(State):
	def enter(worker):
		worker.setTarget(ResourceManager.getItemPosition())

# Go back to base
class GoToBase(State):
	def enter(worker):
		worker.setTarget(BuildingManager.getCastle())

# Convert worker to explorer
class CreateExplorer(State):
	def enter(worker):
		EntityManager.explorerCount += 1
		worker.upgrade(WorkerTypes.explorer)

# Convert worker to crafter
class CreateCraftsman(State):
	def enter(worker):
		EntityManager.craftsmenCount += 1
		worker.upgrade(WorkerTypes.craftsman)
		
class WalkToCamp(State):
	def enter(worker):
		for id in BuildingManager.camps:
			ResourceManager.removeItem('sword')
			worker.campId = id
			worker.setTarget(BuildingManager.camps[id].pos)
			return
		
		worker.changeState(FindWork)

# Convert worker to soldier
class CreateSoldier(State):
	def enter(worker):
		worker.upgrade(WorkerTypes.soldier, worker.campId)