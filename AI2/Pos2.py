from AICore2 import AICore
import math

# Class for positions
class Pos:
	# Create position
	def __init__(self, x, z):
		self.x = x
		self.z = z
		
	# Get distance between two positions
	def getDistance(pos1, pos2):
		return math.sqrt((pos1.x - pos2.x) ** 2 + (pos1.z - pos2.z) ** 2)
		
	# Convert world pos to tile pos
	def tilePos(worldPos):
		return Pos(worldPos.worldX / AICore.tileSize, worldPos.worldZ / AICore.tileSize)
		
	# == operator
	def __eq__(pos1, pos2):
		return pos1.x == pos2.x and pos1.z == pos2.z
		
	# tostring
	def __str__(self):
		return 'Pos(' + str(self.x) + ', ' + str(self.z) + ')'