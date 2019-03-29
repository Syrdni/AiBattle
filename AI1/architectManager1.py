from npcStates1 import idle
from aiTypes import coordinate, buildingType
from unitManager1 import unitManager
import npcStates1 as npcStates
import overseer1
from AICore1 import AICore

#Datacontainer for building
class building:
    def __init__(self, id, type):
        self.id = id
        self.type = type
        self.position = coordinate.fromStruct(AICore.GetPos(id))
        self.currentlyWorkedOn = False
        self.isComplete = False
        self.isBusy = False

class architectManager:
    buildQueue = [[buildingType.COAL,   coordinate(0,1)], 
                  [buildingType.COAL,  coordinate(1,0)],
                  [buildingType.COAL,  coordinate(0,-1)],
                  [buildingType.COAL,coordinate(-1,0)]] #Defines what buildings to build and in which order
    
    #Lists for building types
    kilnList = []
    forgeList = []
    smithList = []
    barrackList = []

    workSites = []

    builders = unitManager.builders


    def addBuilding(type):
        architectManager.buildQueue.append(type)

    def update():
        #Checks buildqueue for what to build
        if(len(architectManager.buildQueue) != 0):
            if(architectManager.buildQueue[0][0] == buildingType.COAL):
                if(overseer1.overseer.reserveResources(10,0,0,0,0)): #Tries to reserve cost, aborts if failed
                    buildLocation = overseer1.castle.position + architectManager.buildQueue[0][1]
                    architectManager.spawnWorksite(buildingType.COAL, buildLocation)
                    
            elif(architectManager.buildQueue[0][0] == buildingType.FORGE):
                if(overseer1.overseer.reserveResources(10,0,0,0,0)): #Tries to reserve cost, aborts if failed
                    buildLocation = overseer1.castle.position + architectManager.buildQueue[0][1]
                    architectManager.spawnWorksite(buildingType.FORGE, buildLocation)

            elif(architectManager.buildQueue[0][0] == buildingType.SMITH):
                if(overseer1.overseer.reserveResources(10,0,0,3,0)): #Tries to reserve cost, aborts if failed
                    buildLocation = overseer1.castle.position + architectManager.buildQueue[0][1]
                    architectManager.spawnWorksite(buildingType.SMITH, buildLocation)

            elif(architectManager.buildQueue[0][0] == buildingType.BARRACK):
                if(overseer1.overseer.reserveResources(10,0,0,0,0)): #Tries to reserve cost, aborts if failed
                    buildLocation = overseer1.castle.position + architectManager.buildQueue[0][1]
                    architectManager.spawnWorksite(buildingType.BARRACK, buildLocation)


        #Looks for worksites to place workers on
        for worksite in architectManager.workSites:
            if(not worksite.currentlyWorkedOn):

                #Picks a worker and assignes to work on worksite
                chosen = unitManager.getFree(architectManager.builders, 1)
                if(len(chosen)!=0):
                    chosen[0].forceState(npcStates.build(chosen[0],worksite))
                    worksite.currentlyWorkedOn = True

    def changePrority(type): #flyttar den valda buildingType till först på listan
        if architectManager.buildQueue.count(type) >= 1:
            x = architectManager.buildQueue.index(type)
            architectManager.buildQueue.pop(x)
            architectManager.buildQueue.insert(0,type)
        else:
            pass

    #Returns buildings with no workers assigned from list parameter 'buildings'
    def getFreeBuilding(buildings):
        for building in buildings:
            if(not building.isBusy):
                return building
        return None

    def spawnWorksite(type, pos):
        AICore.SpawnWorksite(type.value, int(pos.x), int(pos.y))

    #Checks for complete worksites, moves to correct list
    def checkComplete():
        for site in architectManager.workSites:
            if(site.isComplete):
                architectManager.workSites.remove(site)
                if(site.type == buildingType.COAL):
                    architectManager.kilnList.append(site)
                elif(site.type == buildingType.FORGE):
                    architectManager.forgeList.append(site)
                elif(site.type == buildingType.SMITH):
                    architectManager.smithList.append(site)
                elif(site.type == buildingType.BARRACK):
                    architectManager.barrackList.append(site)

  

    def onMessage(message):
        if(message["type"] == "buildComplete"):
            architectManager.checkComplete()
        elif(message["type"] == "entityFound"):
            if(message["entityType"] == "buildingSite"):
                architectManager.workSites.append(building(int(message["ID"]), architectManager.buildQueue.pop(0)[0]))
            #elif(message["entityType"] == "kiln"):
            #    architectManager.kilnList.append(building(int(message["ID"]), buildingType.COAL, coordinate(0,0)))
            #elif(message["entityType"] == "smelter"):
            #    architectManager.forgeList.append(building(int(message["ID"]), buildingType.FORGE, coordinate(0,0)))
            #elif(message["entityType"] == "smithy"):
            #    architectManager.smithList.append(building(int(message["ID"]), buildingType.SMITH, coordinate(0,0)))




