from AICore2 import AICore
from EntityManager2 import EntityManager
from BuildingManager2 import BuildingManager
from State2 import State
from Pos2 import Pos
from Soldier2 import *
from Explorer2 import Charge, Scatter

class StrategyManager:
	team = '2'
	enemyTeam = '1'
	state = None
	
	backupSoldiers = 0
	soldierAttackCount = 1
	
	defendRadius = 10
	defendCount = 5

	def execute():
		StrategyManager.state.execute()
		
	def timeToAttack():
		# Attack enemy base when a certain number of soldiers exists
			# Or if we have more soldiers than the enemy
			# Attack early or late?
			
		# Circle base strategy?
			# Attack as soon as we have a soldier and know the location of their base?
		
		if not BuildingManager.enemyCastle:
			return False
		
		soldiers = len(EntityManager.soldiers)
		enemySoldiers = EntityManager.enemySoldierCount
		
		if soldiers >= StrategyManager.soldierAttackCount + StrategyManager.backupSoldiers:
			return True
			
		return False
		
	# Detect enemy attacks	
	def detectAttacks():
		return False
		soldiers = 0
		castlePos = BuildingManager.getCastle()
	
		for id in EntityManager.enemySoldiers:
			if Pos.getDistance(castlePos, EntityManager.enemySoldiers[id].getPosition()) < StrategyManager.defendRadius:
				soldiers += 1
		
		return soldiers >= StrategyManager.defendCount
		
	def changeState(state):
		StrategyManager.state.exit()
		StrategyManager.state = state
		StrategyManager.state.enter()
		
	def attackIntruders():
		return
		castlePos = BuildingManager.getCastle()
	
		# Attack with guarding soldiers when enemy soldiers near base
		for id in EntityManager.enemySoldiers:
			enemySoldier = EntityManager.enemySoldiers[id]
			
			if Pos.getDistance(castlePos, enemySoldier.getPosition()) < Soldier.guardRange:
				for soldierId in EntityManager.soldiers:
					soldier = EntityManager.soldiers[soldierID]
					
					if soldier.state == Guarding:
						soldier.enemy = enemySoldier
						soldier.changeState(Attack)

class GuardBase(State):
	def enter():
		# Send soldiers back to base
		for soldier in EntityManager.soldiers:
			soldier.changeState(Guarding)

	def execute():
		if StrategyManager.timeToAttack():
			StrategyManager.changeState(AttackEnemyBase)
			return
		elif StrategyManager.detectAttacks():
			StrategyManager.changeState(DefendBase)
			return
		
		StrategyManager.attackIntruders()
		
StrategyManager.state = GuardBase		

class DefendBase(State):
	def enter():
		# Charge-scatter on own base
		#for explorer in EntityManager.getExplorers():
			#explorer.target = BuildingManager.getCastle()
			#explorer.charge = True
			#explorer.changeState(Charge)
			
		#for worker in EntityManager.getWorkers():
			#worker.target = BuildingManager.getCastle()
			#worker.charge = True
			#worker.changeState(Charge)
			
		#for craftsman in EntityManager.getCraftsmen():
			#craftsman.target = BuildingManager.getCastle()
			#craftsman.charge = True
			#craftsman.changeState(Charge)
			
		# Attack enemy base with a couple soldiers and defend base with the rest
		for soldier in soldiers:
			soldier.changeState(Guard)
			
		i = StrategyManager.backupSoldiers
			
		for id in EntityManager.soldiers:
			#EntityManager.soldiers[id].changeState(AttackBase)
			i -= 1
			
			if i <= 0:
				break

	def execute():
		# Make sure the soldiers does not attack the same enemy?
		# Only attack non soldiers if there are no soldiers near by
		StrategyManager.attackIntruders()
		
class AttackEnemyBase(State):
	def enter():
		# Charge-scatter on enemy base
		for id in EntityManager.explorers:
			explorer = EntityManager.explorers[id]
			explorer.target = BuildingManager.getEnemyCastle()
			explorer.charge = True
			explorer.changeState(Charge)
			
		#for worker in EntityManager.getWorkers():
			#worker.target = BuildingManager.getEnemyCastle()
			#worker.charge = True
			#worker.changeState(Charge)
			
		#for craftsman in EntityManager.getCraftsmen():
			#craftsman.target = BuildingManager.getEnemyCastle()
			#craftsman.charge = True
			#craftsman.changeState(Charge)

	def execute():
		# Attack base if charge-scatter is sucessful
		# Attack soldiers if charge-scatter is not sucessful?
		
		# Circle base strategy?
		StrategyManager.attackIntruders()