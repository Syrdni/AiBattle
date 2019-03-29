from npcStates1 import idle, upgrade, moveTo
from npcFSM1 import *
from math import ceil
from random import randint
import time
import sys
import overseer1 


class ratio:
    def __init__(self, workers = 0.5, scouts = 0.5, builders = 0, soldiers = 0):
        self.workers  = workers
        self.scouts   = scouts
        self.builders = builders
        self.soldiers = soldiers

    def __add__(self, rhs):
        return ratio(self.workers   +  rhs.workers,
                     self.scouts    +  rhs.scouts,
                     self.builders  +  rhs.builders,
                     self.soldiers  +  rhs.soldiers)

    def __sub__(self, rhs):
        return ratio(self.workers   -  rhs.workers,
                     self.scouts    -  rhs.scouts,
                     self.builders  -  rhs.builders,
                     self.soldiers  -  rhs.soldiers)

    def __mul__(self, factor):
        return ratio(self.workers   *  factor,
                     self.scouts    *  factor,
                     self.builders  *  factor,
                     self.soldiers  *  factor)

    def __truediv__(self, denominator):
        if denominator == 0:
            return ratio(0,0,0,0)
        return ratio(self.workers   /  denominator,
                     self.scouts    /  denominator,
                     self.builders  /  denominator,
                     self.soldiers  /  denominator)
    
    def __str__(self):
        return str("(" + str(self.workers) + ", " + str(self.scouts) + ", " + str(self.builders) + ", " + str(self.soldiers) + ")")

    def fromList(self, list):
       if(len(list)!=4):
           return
       self.workers  = list[0]
       self.scouts   = list[1]
       self.builders = list[2]
       self.soldiers = list[3]

    def __eq__(self, rhs):
       if(len(list)!=4):
           return
       self.workers  = rhs[0]
       self.scouts   = rhs[1]
       self.builders = rhs[2]
       self.soldiers = rhs[3]

    #Replaces all negative values with 0
    def clearNegatives(self):
        ret = ratio(self.workers, self.scouts, self.builders, self.soldiers)
        for index in range(0,3):
            if(ret[index] < 0): 
                ret[index] = 0
        return ret

    def normalize(self):
        sum = self.workers + self.scouts + self.builders + self.soldiers
        return ratio(self.workers, self.scouts, self.builders, self.soldiers)/sum

    def getMax(self):
        max = self[0]
        maxIndex = 0
        for index in range(1,4):
            if self[index] > max:
                max = self[index]
                maxIndex = index
        return maxIndex

    def getMinIndex(self):
        min = self[0]
        minIndex = 0
        for index in range(1,3):
            if self[index] < min:
                min = self[index]
                minIndex = index
        return minIndex

    def getMinValue(self):
        min = self[0]
        minIndex = 0
        for index in range(1,3):
            if self[index] < min:
                min = self[index]
                minIndex = index
        return min

    def getCurrent():
        return ratio(len(unitManager.workers), len(unitManager.scouts), len(unitManager.builders), len(unitManager.soldiers))/unitManager.unitCount

    def __getitem__(self, key):
        if(key==0):
            return self.workers
        elif(key==1):
            return self.scouts  
        elif(key==2):
            return self.builders
        elif(key==3):
            return self.soldiers
        else:
            return 0

    def __setitem__(self, key, value):
        if(key==0):
            self.workers = value
        elif(key==1):
            self.scouts = value
        elif(key==2):
            self.builders = value
        elif(key==3):
            self.soldiers = value

class unitManager:
    workers = []
    scouts = []
    builders = []
    soldiers = []

    units = [workers, scouts, builders, soldiers]

    unitGoalRatio = ratio(.25,.25,.25,.25)
    unitCount = 0

    def update():
        #print("Maintainging ratio....")
        
        currentRatio = ratio.getCurrent()
        delta = unitManager.unitGoalRatio - currentRatio
        #Counts surplus workers and reassigns the free ones
        
        workerSurplus = len(unitManager.workers) - ceil(unitManager.unitGoalRatio.workers*unitManager.unitCount)
        if(workerSurplus > 0):
            workersToBeAssigned = unitManager.getFree(unitManager.workers, workerSurplus)
            wishfulOutcome = delta.clearNegatives().normalize() * workerSurplus

            #Assigns workers in fitting places
            for unit in workersToBeAssigned:
                type = wishfulOutcome.getMax()
                if(type != 0):
                    if(type==npcType.SOLDIER.value):
                        if(len(overseer1.overseer.barrackList) > 0):
                            if(overseer1.overseer.reserveResources(0,0,0,0,1)):
                                unit.exit()
                                unit.stateQueue = [moveTo(unit, overseer1.overseer.barrackList[0].position), upgrade(unit, npcType.SOLDIER.value, overseer1.overseer.barrackList[0].id)]
                                unit.enter()
                                unitManager.workers.remove(unit)
                                unitManager.units[type].append(unit)
                    else:
                        unit.forceState(upgrade(unit, type,0xFFFFFFFFFFFFFFFF))
                        unitManager.workers.remove(unit)
                        unitManager.units[type].append(unit)

                wishfulOutcome[type] -= 1
        else:
            pass

        for list in unitManager.units:
            for unit in list:
                unit.update()
                pass
        #printWholeStatus()
        
    #Returns all idle units from unitList
    def getAllFree(unitList):
        returnList = []
        for unit in unitList:
            if(unit[0] == idle):
                returnList.append(unit)
        return returnList

    #Returns 'goalQuantity' workers from unitList
    def getFree(unitList, goalQuantity):
        returnList = []
        count = 0
        for unit in unitList:
            if(unit[0] == idle):
                returnList.append(unit)
                count+=1
            if(count==goalQuantity):
                return returnList
        return returnList

    #Returns unit from unitList closest to positionA (or positionB if defined)
    def getClosest(unitList, positionA, positionB = None):
        minDistance = 500
        closestUnit = None
        for unit in unitList:
            localMinDistance = (unit.position-positionA).__len__()
            if(positionB != None):
                distanceB = (unit.position-positionB).__len__()
                localMinDistance = distanceB if distanceA > distanceB else localMinDistance
            if(localMinDistance < minDistance):
                minDistance = localMinDistance
                closestUnit = unit
        if(positionB != None):
            return (closestUnit, (unit.position-positionA).__len__() < (unit.position-positionB).__len__() )
        return closestUnit

    #def onMessage(message):
       # if(message["entityType"] == "worker"): #On entity created

    def getUnitById(id):
        for list in unitManager.units:
            for unit in list:
                if(unit.id == id):
                    return unit

    def addUnit(id):
        #unitManager.workers.append(npcBase(id,AICore.GetPos(id)))
        unitManager.workers.append(npcBase(id,0))
        unitManager.unitCount+=1

    def onMessage(message):
        if(message["type"] == "entityFound"):
            if(message["entityType"] == "worker"):
                unitManager.addUnit(int(message["ID"]))
        elif(message["type"] == "entityLost"):
            unit = unitManager.getUnitById(int(message["ID"]))

            if(unit.type == npcType.WORKER):
                unitManager.workers.remove(unit)
            elif(unit.type == npcType.EXPLORER):
                unitManager.scouts.remove(unit)
            elif(unit.type == npcType.BUILDER):
                unitManager.builders.remove(unit)
            elif(unit.type == npcType.SOLDIER):
                unitManager.soldiers.remove(unit)


def printWholeStatus():
    print("________________________")
    print("===Workers===")
    for worker in unitManager.workers:
        print(str(worker.stateQueue))
    print("===Scouts===")
    for scout in unitManager.scouts:
        print(str(scout.stateQueue))
    print("===Builders===")
    for builder in unitManager.builders:
        print(str(builder.stateQueue))
    print("===Soldiers===")
    for soldier in unitManager.soldiers:
        print(str(soldier.stateQueue))

    print("________________________")

def printStatus():
    
    print("________________________")
    print("Workers  :" + str(len(unitManager.workers)))
    print("Scouts   :" + str(len(unitManager.scouts)))
    print("Builders :" + str(len(unitManager.builders)))
    print("Soldiers :" + str(len(unitManager.soldiers)))
    print("________________________")

#printShit()
#unitManager.upholdRatio()
#printShit()
#for i in range(0,4):
#    unitManager.scouts.append(soldier())
#    unitManager.unitCount += 1
#print("added")
#printShit()
#unitManager.upholdRatio()
#printShit()