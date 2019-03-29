
from aiTypes import *
class castle:
    ID = None
    position = coordinate()
    team = 0

    def __str__():
        return ("Castle: " + str(castle.team) + " @ " + str(castle.position))

from queueFSM1 import queueFSM, state

from unitManager1 import *
from resourceManager1 import *
from combatManager1 import *
from exploreManager1 import *
from architectManager1 import *

import attackPriorityList1 as apl
import json
import os, inspect

class overseer():
    currentJson = None
    unitGoalRatio = unitRatio()
    attackPriority = apl.attackPriorityList()
    team = 1
    barrackList = architectManager.barrackList

    aiFolder = os.path.abspath(inspect.getfile(inspect.currentframe()))

    def importFromJson(path):
        path = overseer.aiFolder.join("./test.json")
        print(str(path))
        with open(path) as json_file:
            overseer.currentJson = json.load(json_file)

            #Resource manager
            resourceManager.resourceGoalRatio = overseer.currentJson["resourceManager"]["resourceGoalRatio"]

            #Unit manager
            unitManager.unitGoalRatio = overseer.currentJson["unitManager"]["unitGoalRatio"]

            #Explore manager

            #Combat manager

            #ArchitectManager
            architectManager.buildQueue = overseer.currentJson["architectManager"]["buildQueue"]

    def loadSettings(settings):
        if(settings.resourceGoalRatio != None):
            resourceManager.resourceGoalRatio.fromList(settings.resourceGoalRatio)
        
        if(settings.unitGoalRatio != None):
            unitManager.unitGoalRatio.fromList(settings.unitGoalRatio)
            exploreManager.rayCount = int(settings.unitGoalRatio[1] * 50)
            
        
        if(settings.buildQueue != None):
            architectManager.buildQueue = settings.buildQueue
        
        #if(settings.resourceGoalRatio != None):
            #resourceManager.resourceGoalRatio = settings.resourceGoalRatio
    
    def update():
            
        unitManager.update()
        resourceManager.update()
        exploreManager.update()
        architectManager.update()
        combatManager.update()

    def onMessage(message):
        #print("Message received: " + str(message))
        if(message["type"] == "tasksComplete" or message["type"] == "moveComplete" or message["type"] == "upgradeComplete" or message["type"] == "craftComplete" or message["type"] == "harvestFailed" or message["type"] == "attackSuccessful" or message["type"] == "attackFailed" or message["type"] == "canAttack"):
            unit = unitManager.getUnitById(int(message["ID"]))
            unit.onMessage(message)
        elif(message["type"] == "dropOffComplete"):
            unit = unitManager.getUnitById(int(message["ID"]))
            unit.onMessage(message)
            resourceManager.onMessage(message)
        elif(message["type"] == "buildComplete"):
            unit = unitManager.getUnitById(int(message["ID"]))
            unit.onMessage(message)
            architectManager.onMessage(message)
        elif(message["type"] == "entityFound" or message["type"] == "entityLost"):
            if(message["team"]=="0"):
                resourceManager.onMessage(message)
            elif(int(message["team"])==overseer.team):
                if(message["entityType"] == "castle"):
                    castle.id = int(message["ID"])
                    castle.position = coordinate.fromWorldSpace(float(message["posx"]), float(message["posz"]))
                    castle.team = overseer.team
                elif(message["entityType"] == "buildingSite" or message["entityType"] == "trainingCamp" or message["entityType"] == "kiln" or message["entityType"] == "smithy" or message["entityType"] == "smelter"):
                    architectManager.onMessage(message)
                    pass
                elif(message["entityType"] == "worker"):
                    unitManager.onMessage(message)
                pass
            else:
                combatManager.onMessage(message)
                pass


        else:
            print("Message not parsed: " + str(message))

    def reserveResources(wood, ore, coal, ingot, sword):
        return resourceManager.reserveResources(wood, ore, coal, ingot, sword)

    def cancelReserve(wood, iron, coal, ingot, sword):
        return resourceManager.cancelReserve(wood, ore, coal, ingot, sword)

    def addResource(type):
        resourceManager.resourceQuantity[type] += 1



class resourceRush():
    def enter():
        overseer.importFromJson("../overseerStates/resourceRush.json")
        pass


