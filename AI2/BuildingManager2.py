from AICore2 import AICore
import ResourceManager2
import StrategyManager2
from Pos2 import Pos
from Building2 import Building
import random

# Building Types
class BuildingType:
	kiln = 1
	smelter = 4
	smith = 2
	camp = 3

# Building Manager
class BuildingManager:
	# BuildingCount
	kilnCount = 0
	smelterCount = 0
	smithCount = 0
	campCount = 0

	# Buildings
	workSites = {}
	kilns = {}
	smelters = {}
	smiths = {}
	camps = {}
	enemyBuildings = {}
	
	workSitePositions = []
	
	castle = None
	enemyCastle = None
	
	backupBuildings = 0
	buildRange = 1
	
	# Building backed up numbers
	itemCount = 5
	
	# Building materials
	kilnMaterials = {'wood': 0}
	smelterMaterials = {'wood': 10}
	smithMaterials = {'wood': 10, 'iron': 3}
	campMaterials = {'wood': 10}
	
	# Add building
	def addBuilding(building, type):
		building.type = type
		
		if type == 'buildingSite':
			for i in range(len(BuildingManager.workSitePositions)):
				if BuildingManager.workSitePositions[i].pos == building.pos:
					building.type = BuildingManager.workSitePositions[i].type
					del BuildingManager.workSitePositions[i]
					
					BuildingManager.workSites[building.id] = building
					break
		elif type == 'kiln':
			for item in BuildingManager.kilnMaterials:
				ResourceManager2.ResourceManager.removeItem(item, BuildingManager.kilnMaterials[item])
				
			BuildingManager.kilns[building.id] = building
		elif type == 'smelter':
			for item in BuildingManager.smelterMaterials:
				ResourceManager2.ResourceManager.removeItem(item, BuildingManager.smelterMaterials[item])
				
			BuildingManager.smelters[building.id] = building
		elif type == 'smithy':
			for item in BuildingManager.smithMaterials:
				ResourceManager2.ResourceManager.removeItem(item, BuildingManager.smithMaterials[item])
				
			BuildingManager.smiths[building.id] = building
		elif type == 'trainingCamp':
			for item in BuildingManager.campMaterials:
				ResourceManager2.ResourceManager.removeItem(item, BuildingManager.campMaterials[item])
				
			BuildingManager.camps[building.id] = building
			
	# Remove building
	def removeBuilding(id, team, cause):
		if team == StrategyManager2.StrategyManager.team:
			# Worksites
			if id in BuildingManager.workSites:
				BuildingManager.workSites[id].craftsmanId = None
				del BuildingManager.workSites[id]
				
			# Kilns
			elif id in BuildingManager.kilns:
				BuildingManager.kilns[id].craftsmanId = None
				BuildingManager.kilnCount -= 1
				del BuildingManager.kilns[id]
			
			# Smelters
			elif id in BuildingManager.smelters:
				BuildingManager.smelters[id].craftsmanId = None
				BuildingManager.smelterCount -= 1
				del BuildingManager.smelters[id]
				
			# Smiths
			elif id in BuildingManager.smiths:
				BuildingManager.smiths[id].craftsmanId = None
				BuildingManager.smithCount -= 1
				del BuildingManager.smiths[id]
				
			# Camps
			elif id in BuildingManager.camps:
				BuildingManager.campCount -= 1
				del BuildingManager.camps[id]
				
		elif team == StrategyManager2.StrategyManager.enemyTeam:
			if id in BuildingManager.enemyBuildings:
				del BuildingManager.enemyBuildings[id]
	
	# Get castle position
	def getCastle():
		if BuildingManager.castle:
			return BuildingManager.castle.pos
		
	# Get enemy castle position
	def getEnemyCastle():
		if BuildingManager.enemyCastle:
			return BuildingManager.enemyCastle.pos
		
	# Check if a building is needed
	def buildingNeeded():
		if BuildingManager.kilnCount <= BuildingManager.backupBuildings:
			return BuildingType.kiln
		elif BuildingManager.kilnCount > 0 and BuildingManager.smelterCount <= BuildingManager.backupBuildings:
			return BuildingType.smelter
		elif BuildingManager.smelterCount > 0 and BuildingManager.smithCount <= BuildingManager.backupBuildings:
			return BuildingType.smith
		elif BuildingManager.smithCount > 0 and BuildingManager.campCount <= BuildingManager.backupBuildings:
			return BuildingType.camp
		
		# Build more buildings if they get backed up
		if len(BuildingManager.workSites) == 0:
			if ResourceManager2.ResourceManager.checkResource('wood', len(BuildingManager.kilns) * BuildingManager.itemCount):
				return BuildingType.kiln
			elif ResourceManager2.ResourceManager.checkResource('ironOre', len(BuildingManager.smelters) * BuildingManager.itemCount):
				return BuildingType.smelter
			elif ResourceManager2.ResourceManager.checkResource('iron', len(BuildingManager.camps) * BuildingManager.itemCount):
				return BuildingType.smith
		
		return None
		
	# Execute
	def execute():
		building = BuildingManager.buildingNeeded()
		
		if building:
			castlePos = BuildingManager.getCastle()
			
			if castlePos:
				BuildingManager.createWorksite(building, Pos(
					random.randint(castlePos.x - BuildingManager.buildRange, castlePos.x + BuildingManager.buildRange),
					random.randint(castlePos.z - BuildingManager.buildRange, castlePos.z + BuildingManager.buildRange)
				))
		
	# Create a worksite
	def createWorksite(type, pos):
		building = Building(0, pos)
		
		# Set building type
		if type == BuildingType.kiln:
			building.type = 'kiln'
		elif type == BuildingType.smelter:
			building.type = 'smelter'
		elif type == BuildingType.smith:
			building.type = 'smithy'
		elif type == BuildingType.camp:
			building.type = 'trainingCamp'
		
		# Add worksite
		BuildingManager.workSitePositions.append(building)
		
		# Try to spawn worksite
		if AICore.SpawnWorksite(type, pos.x, pos.z):
			if type == BuildingType.kiln:
				BuildingManager.kilnCount += 1
			elif type == BuildingType.smelter:
				BuildingManager.smelterCount += 1
			elif type == BuildingType.smith:
				BuildingManager.smithCount += 1
			elif type == BuildingType.camp:
				BuildingManager.campCount += 1
		else:
			del BuildingManager.workSitePositions[-1]
		
	# Get a worksite without a craftsman
	def getWorksite():
		for id in BuildingManager.workSites:
			if BuildingManager.workSites[id].craftsmanId == None:
				return BuildingManager.workSites[id]
				
		return None
		
	# Get a building with no craftsman
	def findEmptyBuilding():
		for id in BuildingManager.smiths:
			if BuildingManager.smiths[id].craftsmanId == None:
				return BuildingManager.smiths[id]
				
		for id in BuildingManager.smelters:
			if BuildingManager.smelters[id].craftsmanId == None:
				return BuildingManager.smelters[id]
				
		for id in BuildingManager.kilns:
			if BuildingManager.kilns[id].craftsmanId == None:
				return BuildingManager.kilns[id]
				
		return None