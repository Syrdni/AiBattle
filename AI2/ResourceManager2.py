from AICore2 import AICore
import Config2 as Config
import BuildingManager2
from Pos2 import Pos

class ResourceManager:
	# Keep track of all items
	items = {'wood': 0, 'charCoal': 0, 'ironOre': 0, 'iron': 0, 'sword': 0}
	backupItems = {'wood': 10, 'charCoal': 0, 'ironOre': 0, 'iron': 0, 'sword': 0}
	charCoal = 10
	
	# Keep track of all resources on the map
	ironOre = []
	trees = []
	
	# Item materials
	charCoalMaterials = {
		'wood': Config.charcoal['requiredMaterials']['wood'],
		'charCoal': Config.charcoal['requiredMaterials']['charcoal'],
		'ironOre': Config.charcoal['requiredMaterials']['ironOre'],
		'iron': Config.charcoal['requiredMaterials']['ironIngot'],
		'sword': Config.charcoal['requiredMaterials']['sword']
	}
	
	ironMaterials = {
		'wood': Config.ironIngot['requiredMaterials']['wood'],
		'charCoal': Config.ironIngot['requiredMaterials']['charcoal'],
		'ironOre': Config.ironIngot['requiredMaterials']['ironOre'],
		'iron': Config.ironIngot['requiredMaterials']['ironIngot'],
		'sword': Config.ironIngot['requiredMaterials']['sword']
	}
	
	swordMaterials = {
		'wood': Config.sword['requiredMaterials']['wood'],
		'charCoal': Config.sword['requiredMaterials']['charcoal'],
		'ironOre': Config.sword['requiredMaterials']['ironOre'],
		'iron': Config.sword['requiredMaterials']['ironIngot'],
		'sword': Config.sword['requiredMaterials']['sword']
	}
	
	# Add items
	def addItem(item, amount = 1):
		ResourceManager.items[item] += amount
		
	# Remove items
	def removeItem(item, amount = 1):
		ResourceManager.items[item] -= amount
		
	def removeResource(id, cause):
		for i, tree in enumerate(ResourceManager.trees):
			if tree.id == id:
				del ResourceManager.trees[i]
				return
				
		for i, ore in enumerate(ResourceManager.ironOre):
			if ore.id == id:
				del ResourceManager.ironOre[i]
				return
		
	# Check if we have a certain amount of resources
	def checkResource(item, amount = 0):
		if type(item) == str:
			return ResourceManager.items[item] >= amount + ResourceManager.backupItems[item]
		else:
			for material in item:
				if not ResourceManager.checkResource(material, item[material]):
					return False
			return True
		
	# Get the best tree to chop
	def findTree(pos = None):
		# Resource distance priority
			# Close faster
			# Far away safer early
			
		if len(ResourceManager.trees):
			if pos == None:
				pos = BuildingManager2.BuildingManager.getCastle()
		
			bestTree = None
			bestDistance = 1000
			
			for tree in ResourceManager.trees:
				if not tree.worker:
					d = Pos.getDistance(tree.pos, pos)
					
					if d < bestDistance:
						bestDistance = d
						bestTree = tree
					
			return bestTree
	
	# Get the best ore to chop
	def findOre(pos = None):
		# Resource distance priority
			# Close faster
			# Far away safer early
			
		if len(ResourceManager.ironOre):
			if pos == None:
				pos = BuildingManager2.BuildingManager.getCastle()
		
			bestOre = None
			bestDistance = 1000
			
			for ore in ResourceManager.ironOre:
				if not ore.worker:
					d = Pos.getDistance(ore.pos, pos)
					
					if d < bestDistance:
						bestDistance = d
						bestOre = ore
					
			return bestOre
	
	# Get the type of resource to gather
	def resourceNeeded():
		if len(ResourceManager.ironOre) > 0 and ResourceManager.items['wood'] > ResourceManager.items['ironOre'] * 2.5:
			return 'iron'
		return 'tree'