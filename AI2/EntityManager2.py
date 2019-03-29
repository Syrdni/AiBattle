from AICore2 import AICore
from BuildingManager2 import BuildingManager
from ResourceManager2 import ResourceManager
import StrategyManager2
from Pos2 import Pos
import Explorer2
import random

import Config2 as Config

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
	
	explorerAmount = 19
	choppers = 12
	builders = 4
	
	mapSize = 100
	
	currentExploreX = 102
	currentExploreZ = 102
	exploreMaxX = 102
	exploreMaxZ = 102
	exploreSide = 0
	deltaExploreX = 5
	deltaExploreZ = 5

	# Add an entity to the list
	def add(entity, type):
		if type == 'worker':
			if EntityManager.choppers > 0:
				EntityManager.choppers -= 1
				entity.chopper = True
			elif EntityManager.builders > 0:
				EntityManager.builders -= 1
				entity.skipState = True
		
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
		if EntityManager.explorerCount < EntityManager.explorerAmount:
			return True
		return False
		
	# Get the position for an explorer to go to
	def getExplorePosition(explorer):
		# Make sure explorers are not exploring the same area
		# Try to explore enemy base as fast as possible to avoid being killed
		# Avoid enemy base when they have soldiers
		
		explorer.x -= EntityManager.deltaExploreX
		explorer.z -= EntityManager.deltaExploreZ
		
		if EntityManager.deltaExploreX > 0:
			if explorer.x < -1:
				explorer.x += EntityManager.deltaExploreX
			
				if EntityManager.setExploreArea(explorer):
					return EntityManager.getExplorePosition(explorer)
					
			if explorer.x < 3:
				explorer.x = 3
		else:
			if explorer.x > EntityManager.mapSize + 1:
				explorer.x += EntityManager.deltaExploreX
			
				if EntityManager.setExploreArea(explorer):
					return EntityManager.getExplorePosition(explorer)
			
			if explorer.x > EntityManager.mapSize - 3:
				explorer.x = EntityManager.mapSize - 3
				
		if EntityManager.deltaExploreZ > 0:
			if explorer.z < -1:
				explorer.z += EntityManager.deltaExploreZ
			
				if EntityManager.setExploreArea(explorer):
					return EntityManager.getExplorePosition(explorer)
					
			if explorer.z < 3:
				explorer.z = 3
		else:
			if explorer.z > EntityManager.mapSize + 1:
				explorer.z += EntityManager.deltaExploreZ
			
				if EntityManager.setExploreArea(explorer):
					return EntityManager.getExplorePosition(explorer)
			
			if explorer.z > EntityManager.mapSize - 3:
				explorer.z = EntityManager.mapSize - 3
			
		return Pos(explorer.x, explorer.z)
		
	# Sets the explore area of the explorer
	def setExploreArea(explorer):
		if explorer.side != 0:
			side = explorer.side
		else:
			side = EntityManager.exploreSide
		
		explorer.side = side
	
		if side == 0:
			charge = False
			
			if EntityManager.deltaExploreX > 0 and EntityManager.currentExploreX < 3:
				charge = True
			elif EntityManager.deltaExploreX < 0 and EntityManager.currentExploreX > EntityManager.mapSize - 3:
				charge = True
			elif EntityManager.deltaExploreZ > 0 and EntityManager.currentExploreZ < 3:
				charge = True
			elif EntityManager.deltaExploreZ < 0 and EntityManager.currentExploreZ > EntityManager.mapSize - 3:
				charge = True
					
			if charge:
				explorer.target = BuildingManager.getEnemyCastle()
						
				if explorer.target:
					explorer.changeState(Explorer2.Charge)
					return False
				return True
		
			explorer.x = EntityManager.currentExploreX
			explorer.z = EntityManager.currentExploreZ
			EntityManager.exploreSide = 1
		elif side == 1:
			EntityManager.currentExploreX -= EntityManager.deltaExploreX
			
			charge = False
			
			if EntityManager.deltaExploreX > 0 and EntityManager.currentExploreX < 3:
				charge = True
			elif EntityManager.deltaExploreX < 0 and EntityManager.currentExploreX > EntityManager.mapSize - 3:
				charge = True
			elif EntityManager.deltaExploreZ > 0 and EntityManager.currentExploreZ < 3:
				charge = True
			elif EntityManager.deltaExploreZ < 0 and EntityManager.currentExploreZ > EntityManager.mapSize - 3:
				charge = True
					
			if charge:
				explorer.target = BuildingManager.getEnemyCastle()
						
				if explorer.target:
					explorer.changeState(Explorer2.Charge)
					return False
				return True
			
			explorer.x = EntityManager.currentExploreX
			explorer.z = EntityManager.exploreMaxZ
			EntityManager.exploreSide = 2
		else:
			EntityManager.currentExploreZ -= EntityManager.deltaExploreZ
			
			charge = False
			
			if EntityManager.deltaExploreX > 0 and EntityManager.currentExploreX < 3:
				charge = True
			elif EntityManager.deltaExploreX < 0 and EntityManager.currentExploreX > EntityManager.mapSize - 3:
				charge = True
			elif EntityManager.deltaExploreZ > 0 and EntityManager.currentExploreZ < 3:
				charge = True
			elif EntityManager.deltaExploreZ < 0 and EntityManager.currentExploreZ > EntityManager.mapSize - 3:
				charge = True
			
			if charge:
				explorer.target = BuildingManager.getEnemyCastle()
				
				if explorer.target:
					explorer.changeState(Explorer2.Charge)
					return False
				return True
			
			explorer.x = EntityManager.exploreMaxX
			explorer.z = EntityManager.currentExploreZ
			EntityManager.exploreSide = 1
			
		return True
		
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
		