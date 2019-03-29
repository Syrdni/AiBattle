import json

variables = json.load(open('variables.json', 'r'))

worker = None
explorer = None
craftsman = None
soldier = None
castle = None

for unit in variables['units']:
	if unit['name'] == 'worker':
		worker = unit
	elif unit['name'] == 'explorer':
		explorer = unit
	elif unit['name'] == 'craftsman':
		craftsman = unit
	elif unit['name'] == 'soldier':
		soldier = unit
	elif unit['name'] == 'castle':
		castle = unit
		

buildingSite = None
charcoalPile = None
smithy = None
smelter = None
trainingCamp = None

for unit in variables['buildings']:
	if unit['name'] == 'buildingSite':
		buildingSite = unit
	elif unit['name'] == 'charcoalPile':
		charcoalPile = unit
	elif unit['name'] == 'smithy':
		smithy = unit
	elif unit['name'] == 'smelter':
		smelter = unit
	elif unit['name'] == 'trainingCamp':
		trainingCamp = unit
		

wood = None
charcoal = None
ironOre = None
ironIngot = None
sword = None

for unit in variables['materials']:
	if unit['name'] == 'wood':
		wood = unit
	elif unit['name'] == 'charcoal':
		charcoal = unit
	elif unit['name'] == 'ironOre':
		ironOre = unit
	elif unit['name'] == 'ironIngot':
		ironIngot = unit
	elif unit['name'] == 'sword':
		sword = unit
		
