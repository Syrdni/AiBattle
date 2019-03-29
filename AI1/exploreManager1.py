import unitManager1 as unitManager
from npcStates1 import moveTo, walkInDirection
from aiTypes import coordinate
from overseer1 import castle
from math import tan
import random


class exploreManager:

    scouts = unitManager.unitManager.scouts

    baseScouters = 0
    rayCount = 5

    #Min and max coordinates of the map.
    min = coordinate(3,3)
    max = coordinate(97,97)

    #Defines same for walkInDirection state
    walkInDirection.max = max
    walkInDirection.min = min

    def buildPath(positionA, positionB):
        roadBuilder = unitManager.unitManager.getClosest(scouts, positionA, positionB)

    def update():
        explorersToBeAssigned = unitManager.unitManager.getAllFree(exploreManager.scouts)
        for unit in explorersToBeAssigned:

            #Takes all explorers but one, applies area explore algorithm
            if(exploreManager.baseScouters < exploreManager.rayCount-1):
                exploreManager.baseScouters += 1

                #Calculates tan values for rays depending on # of explorers
                tanValue = tan(1.55*max(min(exploreManager.baseScouters/exploreManager.rayCount, 1),0))
                
                #Adds 4 walkInDirection to unit stateQueue
                unit.stateQueue = exploreManager.getQueue(unit, tanValue)
                unit.enter()
            #One explorer explores across the map, to connect the different routes that will be created at the start
            elif(exploreManager.baseScouters < exploreManager.rayCount):
                unit.stateQueue = [moveTo(unit, coordinate(int(exploreManager.max.x*0.8), int(exploreManager.max.y*0.2))),
                                   moveTo(unit, coordinate(int(exploreManager.max.x*0.2), int(exploreManager.max.y*0.8)))]
                unit.enter()
            #When main explore algorithm is done, explore randomly
            else:
                x = random.randint(exploreManager.min.x, exploreManager.max.x)
                y = random.randint(exploreManager.min.y, exploreManager.max.y)

                unit.forceState(moveTo(unit, coordinate(x,y)))

    def getQueue(unit, tanValue):
        print(castle.__str__())
        if(castle.team == 1):
            return [
            moveTo(unit, exploreManager.min),       walkInDirection(unit, tanValue, exploreManager.max*0.9, coordinate()), 
            moveTo(unit, exploreManager.max),       walkInDirection(unit, -tanValue, exploreManager.max*1.1, exploreManager.max*0.2), 
            moveTo(unit, exploreManager.max*0.2),   walkInDirection(unit, tanValue, exploreManager.max*0.8, coordinate()), 
            moveTo(unit, exploreManager.max*0.8),   walkInDirection(unit, -tanValue, exploreManager.max*1.1, exploreManager.max*0.4)]
        else:
            return [
            moveTo(unit, exploreManager.max),       walkInDirection(unit, -tanValue, exploreManager.max*1.1, exploreManager.max*0.2), 
            moveTo(unit, exploreManager.min),       walkInDirection(unit, tanValue, exploreManager.max*0.8, coordinate()), 
            moveTo(unit, exploreManager.max*0.8),   walkInDirection(unit, -tanValue, exploreManager.max*1.1, exploreManager.max*0.2), 
            moveTo(unit, exploreManager.max*0.2),   walkInDirection(unit, tanValue, exploreManager.max*0.6, coordinate())]

