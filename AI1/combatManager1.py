from AICore1 import AICore
import unitManager1 as unitManager
from npcStates1 import *

class combatManager():
    """description of class"""

    listOfEnemies = {}
    soldiers = unitManager.unitManager.soldiers

    def onMessage():
        message = AICore.getMessage()
        if(message["type"] == "entityFound"):
            if(message["team"] == "2"):
                listOfEnemies[int (message["ID"])] = enemy(message["posx"], message["posz"])

        elif(message["type"]) == "entityLost":
            del listOfEnemies[message["ID"]]
   

    def attack(attacker, victim, required = 1):

        soldiersToAttack = unitManager.unitManager.getNClosest(soldiers, required, victim.position.tileSpace())

        for soldier in soldiersToAttack:
            soldier.forceState(attack(soldier, target))

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
        #thinker


#Don't I have to update the coordinate values?
class enemy():

    def __init__(self, type, posX, posY):
        self.type = type
        self.pos = coordinate(posX, posY)