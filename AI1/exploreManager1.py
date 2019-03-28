import unitManager1 as unitManager
from npcStates1 import moveTo
from aiTypes import coordinate
from overseer1 import castle
import random


class exploreManager:

    scouts = unitManager.unitManager.scouts

    baseScouters = 0

    def buildPath(positionA, positionB):
        roadBuilder = unitManager.unitManager.getClosest(scouts, positionA, positionB)

    def update():
        explorersToBeAssigned = unitManager.unitManager.getAllFree(exploreManager.scouts)
        for unit in explorersToBeAssigned:

            #Makes sure the beginning area is a bit more explored so we can quickly start accessing resources
            if(exploreManager.baseScouters <= 10):
                exploreManager.baseScouters = exploreManager.baseScouters + 1

                gotoX = int(castle.position.x + random.randint(-3, 30))
                gotoY = int(castle.position.y + random.randint(-10, 10))
                print("X: ", gotoX, " Y: ", gotoY)

                unit.forceState(moveTo(unit, coordinate(gotoX, gotoY)))

            else:
                x = random.randint(3, 97)
                y = random.randint(3, 97)

                unit.forceState(moveTo(unit, coordinate(x, y)));
