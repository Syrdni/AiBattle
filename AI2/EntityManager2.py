from AICore2 import AICore
from BuildingManager2 import BuildingManager
from ResourceManager2 import ResourceManager
import StrategyManager2
from Pos2 import Pos
import Explorer2
import random

#import EntityManager2
#import Pos2
#def getpos(): return Pos2.Pos(5, 11)
#EntityManager2.EntityManager.getExplorePosition = getpos

class EntityManager:
	# Message types to send to entities
	entityMessages = ['tasksComplete', 'moveComplete', 'buildComplete', 'upgradeComplete', 'dropOffComplete', 'craftComplete', 'harvestFailed', 'attackFailed', 'canAttack']
	
	# Stores all entities
	workers = {}
	explorers = {}
	craftsmen = {}
	soldiers = {}
	
	explorerCount = 0
	craftsmenCount = 0
	
	# enemies
	enemySoldiers = {}
	enemyUnits = {}
	enemySoldierCount = 0
	
	randomNext = 0
	updateStart = 0
	
	currentExplore = 97

	# Add an entity to the list
	def add(entity, type):
		if type == 'worker':
			EntityManager.workers[entity.id] = entity
		elif type == 'explorer':
			EntityManager.explorers[entity.id] = entity
		elif type == 'craftsman':
			EntityManager.craftsmenCount -= 1
			EntityManager.craftsmen[entity.id] = entity
		elif type == 'soldier':
			EntityManager.soldiers[entity.id] = entity
			
	# Remove an entity
	def remove(id, team, cause):
		if team == StrategyManager2.StrategyManager.team:
			# Worker
			if id in EntityManager.workers:
				if EntityManager.workers[id].resource:
					EntityManager.workers[id].resource.taken = False
				del EntityManager.workers[id]
				
			# Explorer
			elif id in EntityManager.explorers:
				EntityManager.explorerCount -= 1
				del EntityManager.explorers[id]
				
			# Craftsman
			elif id in EntityManager.craftsmen:
				EntityManager.craftsmen[id].building.craftsmanId = None
				del EntityManager.craftsmen[id]
				
			# Soldier
			elif id in EntityManager.soldiers:
				del EntityManager.soldiers[id]
		elif team == StrategyManager2.StrategyManager.enemyTeam:
			if id in EntityManager.enemySoldiers:
				del EntityManager.enemySoldiers[id]
				EntityManager.enemySoldierCount -= 1
			elif id in EntityManager.enemyUnits:
				del EntityManager.enemyUnits[id]

	# Execute all entities
	def execute():
		for id in EntityManager.workers:
			EntityManager.workers[id].execute()
		for id in EntityManager.explorers:
			EntityManager.explorers[id].execute()
		for id in EntityManager.craftsmen:
			EntityManager.craftsmen[id].execute()
		for id in EntityManager.soldiers:
			EntityManager.soldiers[id].execute()
			
		if len(EntityManager.enemySoldiers) > EntityManager.enemySoldierCount:
			EntityManager.enemySoldierCount = len(EntityManager.enemySoldiers)
	
	# Get entity with specific id
	def getEntity(id):
		if id in EntityManager.workers:
			return EntityManager.workers[id]
		elif id in EntityManager.explorers:
			return EntityManager.explorers[id]
		elif id in EntityManager.craftsmen:
			return EntityManager.craftsmen[id]
		elif id in EntityManager.soldiers:
			return EntityManager.soldiers[id]
	
	# Check if an explorer is needed
	def explorerNeeded():
		# Create explorers based on unexplored area size
		if EntityManager.explorerCount < 8:
			return True
		return False
		
	# Get the position for an explorer to go to
	def getExplorePosition(explorer):
		# Make sure explorers are not exploring the same area
		# Try to explore enemy base as fast as possible to avoid being killed
		# Avoid enemy base when they have soldiers
		
		explorer.z += explorer.dz * 6
		
		if explorer.z < 3:
			explorer.z = 3
			explorer.dz = 1
			explorer.x -= 3
		elif explorer.z > 97:
			explorer.z = 97
			explorer.dz = -1
			explorer.x -= 3
			
		if explorer.x < explorer.x2:
			explorer.x = explorer.x1
			explorer.target = BuildingManager.getEnemyCastle()
			
			if explorer.target:
				pass#explorer.changeState(Explorer2.Charge)
			#return None
			
		return Pos(explorer.x, explorer.z)
		
	# Sets the explore area of the explorer
	def setExploreArea(explorer):
		explorer.x1 = EntityManager.currentExplore
		explorer.x2 = EntityManager.currentExplore - 9
		EntityManager.currentExplore -= 12
		
		if EntityManager.currentExplore - 9 < 0:
			EntityManager.currentExplore = 97
		
	# Check if a craftsman is needed
	def craftsmanNeeded():
		if EntityManager.craftsmenCount > 0:
			return False
	
		for id in EntityManager.craftsmen:
			if EntityManager.craftsmen[id].building == None:
				return False
	
		return BuildingManager.findEmptyBuilding() or BuildingManager.getWorksite()
		
	# Check if a soldier is needed
	def soldierNeeded():
		# Wait with creating soldiers to not lose workers?
		if len(BuildingManager.camps) > 0:
			return ResourceManager.checkResource('sword', 1)
			
		return False
		
	# Get the nearest enemy soldier
	def getNearestSoldier(pos, radius = None):
		if radius:
			for id in EntityManager.enemySoldiers:
				enemySoldier = EntityManager.enemySoldiers[id]
				
				if Pos.getDistance(pos, enemySoldier.getPosition()) < radius:
					return enemySoldier
		else:
			best = None
			bestDistance = 1000
			
			for id in EntityManager.enemySoldiers:
				enemySoldier = EntityManager.enemySoldiers[id]
				
				if Pos.getDistance(pos, enemySoldier.getPosition()) < bestDistance:
					best = enemySoldier
					bestDistance = Pos.getDistance(pos, enemySoldier.getPosition())
					
			return best
		