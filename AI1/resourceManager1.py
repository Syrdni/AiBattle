from npcStates1 import harvest, craft
from aiTypes import *
from unitManager1 import unitManager
from architectManager1 import architectManager
from math import ceil
from random import randint
from overseer1 import castle


class resourceRatio:
    def __init__(self, wood, ore, coal, ingot, sword):
        self.wood = wood
        self.ore = ore
        self.coal = coal
        self.ingot = ingot
        self.sword = sword

    def __mul__(self, rhs):
        return resourceRatio(self.wood  * rhs, self.ore  * rhs, self.coal  * rhs, self.ingot * rhs, self.sword * rhs)

    def __sub__(self, rhs):
        return resourceRatio(self.wood  - rhs.wood , 
                             self.ore  - rhs.iron , 
                             self.coal  - rhs.coal , 
                             self.ingot - rhs.ingot, 
                             self.sword - rhs.sword )

    def fromList(self, list):
       if(len(list)!=5):
           return
       self.wood  = list[0]
       self.ore  = list[1]
       self.coal  = list[2]
       self.ingot = list[3]
       self.sword = list[4]

    def __eq__(self, rhs):
       if(len(rhs)!=5):
           return
       self.wood  = rhs[0]
       self.ore  = rhs[1]
       self.coal  = rhs[2]
       self.ingot = rhs[3]
       self.sword = rhs[4]

    def __truediv__(self, rhs):
        return resourceRatio(self.wood  / rhs, self.ore  / rhs, self.coal  / rhs, self.ingot / rhs, self.sword / rhs)

    def getCurrent():
        quantity = resourceManager.resourceQuantity
        return resourceRatio(quantity["wood"], quantity["ore"], quantity["coal"], quantity["ingot"], quantity["sword"])/resourceManager.totalResources

    
    def clearNegatives(self):
        ret = resourceRatio(self.wood, self.ore, self.coal, self.ingot, self.sword)
        for index in range(0,4):
            if(ret[index] < 0): 
                ret[index] = 0
        return ret

    def normalize(self):
        sum = self.wood + self.ore + self.coal + self.ingot + self.sword
        return unitRatio(self.wood, self.ore, self.coal, self.ingot, self.sword)/sum


    def __str__(self):
        return ("wood: " + str(self.wood) + "ore: " + str(self.ore) + ", coal: " + str(self.coal) + ", ingot: " + str(self.ingot) + ", sword: " + str(self.sword))

    def __getitem__(self, key):
        if(key==0):
            return self.wood
        elif(key==1):
            return self.ore  
        elif(key==2):
            return self.coal
        elif(key==3):
            return self.ingot
        elif(key==4):
            return self.sword
        else:
            return 0



class Tree:
    def __init__(self, id, position):
        self.ID = id
        self.isAvailable = True
        self.position = position
        tempDistanceCoord = (position - castle.position).getAbs()
        tempX = tempDistanceCoord.x
        tempY = tempDistanceCoord.y
        #Manhattan distance, except without the smaller axis because we can move diagonally
        self.distance = tempX if tempX > tempY else tempY

    def __eq__(self, other):
        return self.ID == other.ID

    def __le__(self, other):
        return self.distance <= other.distance


#Sorted list that handles IDs or whatever
class SortedList:
    def __init__(self, baselist = []):
        self.itemList = sorted(baselist)

    def __getitem__(self, item):
        return self.itemList[item]

    def add(self, item):
        i = 0
        #If the item is not the biggest item, put it where it belongs
        for x in self.itemList:
            if item <= x:
                self.itemList.insert(i, item)
                return
            else:
                i += 1
        #If it's the biggest item in the list, append it.
        self.itemList.append(item)

    def remove(self, item):
        self.itemList.remove(item)

    def __len__(self):
        return len(self.itemList)

class resourceManager:

    resourceQuantity = {"wood" : 10, "ore" : 0, "coal" : 0, "ingot" : 0, "sword": 0}

    resourceGoalRatio = resourceRatio(0.3,0.3,0.2,0.2,0.1) #Tells resourcemanagers to which ratio resources should exist (percentage)
    totalResources = 10

    castleID = None
    castlePos = coordinate()

    #List of found resource deposits
    treeList = SortedList()
    ironList = SortedList()

    builders = unitManager.builders #List of builders
    workers = unitManager.workers #List of workers


    def update():
        goal = resourceManager.resourceGoalRatio * resourceManager.totalResources
        resourceManager.craft(goal) #Crafts resources

        rand = randint(0,100)

        workersToBeAssigned = unitManager.getAllFree(resourceManager.workers)
        #Compares target wood/ore ratio to actual wood/ore ratio. Sends worker to best possible resource (with failsafes)
        for worker in workersToBeAssigned:
            if((resourceManager.resourceGoalRatio.wood+1)/(resourceManager.resourceGoalRatio.ore+1) > (resourceManager.resourceQuantity["wood"]+1)/(resourceManager.resourceQuantity["ore"]+1)):
                if(not resourceManager.getTree(worker)):
                    resourceManager.getIron(worker)
            else:
                if(not resourceManager.getIron(worker)):
                    resourceManager.getTree(worker)


    #evaluate what needs to be done and append it as task
    def appendTask(workerMessage):
        if workerMessage.sender == idle:
            worker.changeState(checkWhatNeedsToBeDone()) #temp

    #Finds the nearest unassigned tree and orders the worker to harvest it
    def getTree(worker):
        if(len(resourceManager.treeList) > 0):
            for tree in resourceManager.treeList:
                if(tree.isAvailable):
                    tree.isAvailable = False
                    worker.forceState(harvest(worker, tree.ID))
                    return True
        return False #Returns false if failed

    #Finds the nearest unassigned Iron and orders the worker to harvest it
    def getIron(worker):
        if(len(resourceManager.ironList) > 0):
            for iron in resourceManager.ironList:
                if(iron.isAvailable):
                    iron.isAvailable = False
                    worker.forceState(harvest(worker, iron.ID))
                    return True
        return False #Returns false if failed


    def craft(goal):
        #Calculates how much of every resource resourceManager wants to have

        #If lacking a craftable resource, try to craft it
        if(resourceManager.resourceQuantity["coal"] < goal.coal):
            if(resourceManager.reserveResources(2,0,0,0,0)):                                    #Tries to reserve cost
                if(not resourceManager.assignWorker(architectManager.kilnList, "coal")):        #If no worker could be assigned, revert cost reservation
                    resourceManager.cancelReserve(2,0,0,0,0)

        if(resourceManager.resourceQuantity["ingot"] < goal.ingot):
            if(resourceManager.reserveResources(0,2,3,0,0)):                                    #Tries to reserve cost
                if(not resourceManager.assignWorker(architectManager.forgeList, "ingot")):      #If no worker could be assigned, revert cost reservation
                    resourceManager.cancelReserve(0,2,3,0,0)

        if(resourceManager.resourceQuantity["sword"] < goal.sword):
            if(resourceManager.reserveResources(0,0,2,1,0)):                                    #Tries to reserve cost
                if(not resourceManager.assignWorker(architectManager.smithList, "sword")):      #If no worker could be assigned, revert cost reservation
                    resourceManager.cancelReserve(0,0,2,1,0)

    #Method for automatically assigning an idle worker to craft in building
    def assignWorker(buildings, resource):
        unit = unitManager.getFree(unitManager.builders, 1)
        if(len(unit)==0):
            return False
        unit = unit[0]
        building = architectManager.getFreeBuilding(buildings)
        if(building==None):
            return False
        unit.forceState(craft(unit, building, resource))
        building.isBusy = True
        return True

    def onMessage(message):
        if(message["type"] == "entityFound"):
            if(message["entityType"] == "tree"):
                entityPos = coordinate.fromWorldSpace(float(message["posx"]),float(message["posz"]))
                resourceManager.treeList.add(Tree(int(message["ID"]), entityPos))
            
            elif(message["entityType"] == "ironOre"):
                entityPos = coordinate(float(message["posx"]),float(message["posz"]))
                resourceManager.ironList.add(Tree(int(message["ID"]), entityPos))

        elif(message["type"] == "entityLost"):
            if(message["entityType"] == "tree"):
                for tree in resourceManager.treeList:
                    if(tree.ID == int(message["ID"])):
                       resourceManager.treeList.remove(tree)
            
            elif(message["entityType"] == "ironOre"):
                for iron in resourceManager.ironList:
                    if(iron.ID == int(message["ID"])):
                       resourceManager.ironList.remove(iron)

        elif(message["type"] == "dropOffComplete"):
            if(message["resourceType"] == "tree"):
                resourceManager.resourceQuantity["wood"] += 1
                resourceManager.totalResources += 1
            
            elif(message["resourceType"] == "ironOre"):
                resourceManager.resourceQuantity["ore"] += 1
                resourceManager.totalResources += 1

    #Method for reserving resource. Subtracts from resourceQuantity if it has enought, otherwise returns false
    def reserveResources(wood, ore, coal, ingot, sword):
        resources = resourceManager.resourceQuantity
        if(resources["wood"] < wood or resources["ore"] < ore or resources["coal"] < coal or resources["ingot"] < ingot or resources["sword"] < sword):
            return False #Not enough raw materials
        else:
            resources["wood"]  -= wood 
            resources["ore"]  -= ore 
            resources["coal"]  -= coal 
            resources["ingot"] -= ingot
            resources["sword"] -= sword
            resourceManager.totalResources -= (wood + ore + coal + ingot + sword)
            return True

    #Similar as before but adds without check instead. Used to revert from canceled reservation
    def cancelReserve(wood, iron, coal, ingot, sword):
        resources = resourceManager.resourceQuantity
        resources["wood"]  += wood 
        resources["ore"]  += iron 
        resources["coal"]  += coal 
        resources["ingot"] += ingot
        resources["sword"] += sword

        resourceManager.totalResources += wood + iron + coal + ingot + sword

    def retreatWorkers(workers):
        for unit in workers:
            unit.forceState(npcStates.moveTo(unit, basePosition))
    