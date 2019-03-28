import unitManager1 as unitManager

class exploreManager:

    scouts = unitManager.unitManager.scouts

    def buildPath(positionA, positionB):
        roadBuilder = unitManager.unitManager.getClosest(scouts, positionA, positionB)

