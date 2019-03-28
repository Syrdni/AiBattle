from AICore2 import AICore
from StrategyManager2 import StrategyManager
from EntityManager2 import EntityManager
from ResourceManager2 import ResourceManager
from BuildingManager2 import BuildingManager
from Worker2 import Worker
from Explorer2 import Explorer
from Craftsman2 import Craftsman
from Soldier2 import Soldier
from Building2 import Building
from Enemy2 import Enemy
from Resource2 import Resource
from Pos2 import Pos

# Startup
def startup():
	pass

# Update
def update():
	StrategyManager.execute()
	BuildingManager.execute()
	EntityManager.execute()
	
# Get messages from c++
def onMessage():
	message = AICore.GetMessage()
	#print(message)
	
	# EntityFound
	if message['type'] == 'entityFound':
		message['ID'] = int(message['ID'])
		message['posx'] = float(message['posx'])
		message['posz'] = float(message['posz'])
		entityPos = Pos(message['posx'] / AICore.tileSize, message['posz'] / AICore.tileSize)
	
		# Resources
		if message['entityType'] == 'tree':
			ResourceManager.trees.append(Resource(message['ID'], entityPos, 'tree'))
		elif message['entityType'] == 'ironOre':
			ResourceManager.ironOre.append(Resource(message['ID'], entityPos, 'iron'))
	
		# Units
		elif message['entityType'] == 'worker':
			if message['team'] == StrategyManager.team:
				Worker(int(message['ID']))
			else:
				EntityManager.enemyUnits[message['ID']] = Enemy(message['ID'])
		elif message['entityType'] == 'explorer':
			if message['team'] == StrategyManager.team:
				Explorer(message['ID'])
			else:
				EntityManager.enemyUnits[message['ID']] = Enemy(message['ID'])
		elif message['entityType'] == 'craftsman':
			if message['team'] == StrategyManager.team:
				Craftsman(message['ID'])
			else:
				EntityManager.enemyUnits[message['ID']] = Enemy(message['ID'])
		elif message['entityType'] == 'soldier':
			if message['team'] == StrategyManager.team:
				Soldier(message['ID'])
			else:
				EntityManager.enemySoldiers[message['ID']] = Enemy(message['ID'])
				
				if message['cause'] == 'created':
					EntityManager.enemySoldierCount += 1
			
		# Buildings
		elif message['entityType'] == 'buildingSite' or message['entityType'] == 'kiln' or message['entityType'] == 'smelter' or message['entityType'] == 'smithy' or message['entityType'] == 'trainingCamp':
			if message['team'] == StrategyManager.team:
				BuildingManager.addBuilding(Building(message['ID'], entityPos), message['entityType'])
			else:
				BuildingManager.enemyBuildings[message['ID']] = Building(message['ID'], entityPos)
		
		# Castle
		elif message['entityType'] == 'castle':
			if message['team'] == StrategyManager.team:
				BuildingManager.castle = Building(message['ID'], entityPos)
			elif message['team'] == StrategyManager.enemyTeam:
				BuildingManager.enemyCastle = Building(message['ID'], entityPos)
				
	# EntityLost
	elif message['type'] == 'entityLost':
		message['ID'] = int(message['ID'])
		
		if message['entityType'] == 'tree' or message['entityType'] == 'ironOre':
			ResourceManager.removeResource(message['ID'], message['cause'])
		elif message['entityType'] == 'worker' or message['entityType'] == 'explorer' or message['entityType'] == 'craftsman' or message['entityType'] == 'soldier':
			EntityManager.remove(message['ID'], message['team'], message['cause'])
		else:
			BuildingManager.removeBuilding(message['ID'], message['team'], message['cause'])
		
	# Send message to entity
	elif message['type'] in EntityManager.entityMessages:
		message['ID'] = int(message['ID'])
		
		entity = EntityManager.getEntity(message['ID'])
		
		if entity:
			entity.onMessage(message)