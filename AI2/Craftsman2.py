from AICore2 import AICore
from BaseGameEntity2 import BaseGameEntity
from EntityManager2 import EntityManager
from BuildingManager2 import BuildingManager
from StrategyManager2 import StrategyManager
from ResourceManager2 import ResourceManager
from State2 import State
from Pos2 import Pos
from Building2 import Building

# Craftsman class
class Craftsman(BaseGameEntity):
	# Create Craftsman
	def __init__(self, id):
		super().__init__(id)
		EntityManager.add(self, 'craftsman')
		self.building = None
		self.charge = False
		self.crafting = False
		self.changeState(Idle)
		
	# Handels messages
	def onMessage(self, message):
		if message['type'] == 'buildComplete':
			newBuilding = Building(self.building.id, self.building.pos)
			buildingType = self.building.type
			BuildingManager.removeBuilding(self.building.id, StrategyManager.team, 'built')
			BuildingManager.addBuilding(newBuilding, buildingType)
			self.changeState(Idle)
		elif message['type'] == 'craftComplete':
			if self.building.type == 'kiln':
				ResourceManager.addItem('charCoal')
			elif self.building.type == 'smelter':
				ResourceManager.addItem('iron')
			elif self.building.type == 'smithy':
				ResourceManager.addItem('sword')
			
			self.crafting = False
			self.changeState(WorkInBuilding)
			
		if self.charge and message['type'] == 'tasksComplete':
			self.changeState(Scatter)
			
	# Build building
	def build(self):
		self.building.craftsmanId = self.id
		AICore.Build(self.id, self.building.id)
		
	# Craft materials in building
	def craft(self, building):
		self.crafting = True
		AICore.Craft(self.id, building.id)
		
# Find something to build
class Idle(State):
	def enter(craftsman):
		craftsman.building = None

	def execute(craftsman):
		# Check if an empty building exists
		craftsman.building = BuildingManager.findEmptyBuilding()
		
		if craftsman.building:
			craftsman.building.craftsmanId = craftsman.id
			craftsman.changeState(WorkInBuilding)
			return
	
		# Check if an empty worksite exists
		craftsman.building = BuildingManager.getWorksite()
		
		if craftsman.building:
			craftsman.changeState(BuildBuilding)
			return

# Build a building
class BuildBuilding(State):
	def enter(craftsman):
		craftsman.build()

# Work in a building
class WorkInBuilding(State):
	def enter(craftsman):
		if craftsman.building.type == 'kiln':
			if ResourceManager.checkResource(ResourceManager.charCoalMaterials):
				for i in ResourceManager.charCoalMaterials:
					ResourceManager.items[i] -= ResourceManager.charCoalMaterials[i]
				craftsman.craft(craftsman.building)
				return
		elif craftsman.building.type == 'smelter':
			if ResourceManager.checkResource(ResourceManager.ironMaterials):
				for i in ResourceManager.ironMaterials:
					ResourceManager.items[i] -= ResourceManager.ironMaterials[i]
				craftsman.craft(craftsman.building)
				return
		elif craftsman.building.type == 'smithy':
			if ResourceManager.checkResource(ResourceManager.swordMaterials):
				for i in ResourceManager.swordMaterials:
					ResourceManager.items[i] -= ResourceManager.swordMaterials[i]
				craftsman.craft(craftsman.building)
				return
				
	def execute(craftsman):
		if not craftsman.crafting:
			craftsman.changeState(WorkInBuilding)
		