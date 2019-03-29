#from AICore1 import AICore
import unitManager1 as unitManager
from AICore1 import AICore
from aiTypes import coordinate
from npcStates1 import attack
from overseer1 import castle


class combatManager():
    """description of class"""

    listOfEnemies = []
    estimatedEnemyArmySize = 0
    soldiers = unitManager.unitManager.soldiers

    def onMessage(message):
        if(message["type"] == "entityFound"):
            if(message["team"] != castle.team):
                combatManager.listOfEnemies.append(Enemy(int(message["ID"]), message["entityType"], coordinate(float(message["posx"]), float(message["posz"]))))
                # Army size is at least as big as we can see.
                if(len(combatManager.listOfEnemies) > combatManager.estimatedEnemyArmySize):
                    combatManager.estimatedEnemyArmySize = len(combatManager.listOfEnemies)

        elif(message["type"] == "entityLost") and (message["team"] != castle.team):
            #combatManager.listOfEnemies.remove(message["ID"])
            # Remove any enemies that have died from their estimated army size.
            if(message["cause"] == "dead"):
                combatManager.estimatedEnemyArmySize =-1


   
    #def attack(attacker, victim, required = 1):
    #    soldiersToAttack = unitManager.unitManager.getNClosest(soldiers, required, victim.position.tileSpace())
    #
    #    for soldier in soldiersToAttack:
    #        soldier.forceState(attack(soldier, target))

    def retreat():
        pass

    #a lot of moveTo's
    def formation():
        pass

    #how is this supposed to work?
    def callBackup():
        pass

    def weighArmies():
        #change tactic according to amount of enemy units total + the probable amount of additional characters
        pass

    def squads():
        pass

    def update():
        #print(combatManager.listOfEnemies)
        soldiersToBeAssigned = unitManager.unitManager.getAllFree(combatManager.soldiers)
        for enemy in combatManager.listOfEnemies:
            if enemy.type == "castle":
                for soldier in soldiersToBeAssigned:
                    soldier.forceState(attack(soldier, enemy.ID))



        
        #for soldier in soldiersToBeAssigned:
            #for enemy in combatManager.listOfEnemies:
                #AICore.Attack(soldier.id, enemy.ID)
                #soldier.forceState(attack(soldier, enemy.ID))
#Don't I have to update the coordinate values?
class Enemy():

    def __init__(self, id, type, pos):
        self.ID = id
        self.type = type
        self.pos = pos

    def __eq__(self, other):
        return self.ID == other.ID