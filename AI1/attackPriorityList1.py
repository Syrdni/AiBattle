class attackPriorityList:
    def __init__(self, workerPriority = 1, soldierPriority = 1, builderPriority = 1, explorerPriority = 1, buildingPriority = 1, distanceToSelfMultiplier = 1, distanceToDestinationMultiplier = 1):
        self.workerPriority = workerPriority
        self.soldierPriority = soldierPriority
        self.builderPriority = builderPriority
        self.explorerPriority = explorerPriority
        self.buildingPriority = buildingPriority
        self.distanceToSelfMultiplier = distanceToSelfMultiplier
        self.distanceToDestinationMultiplier = distanceToDestinationMultiplier
    
    def getValue(self, target):
        typeValue = 1

        if(target.type==npcType.WORKER):
            typeValue = self.workerPriority
        elif(target.type==npcType.EXPLORER):
            typeValue = self.explorerPriority
        elif(target.type==npcType.BUILDER):
            typeValue = self.workerPriority
        elif(target.type==npcType.SOLDIER):
            typeValue = self.soldierPriority
        elif(target.type==4): #BUILDING (to be changed)
            typeValue = self.buildingPriority

        return typeValue*(self.distanceMultiplier/distanceToSelf + self.distanceToDestinationMultiplier/distanceToDestination)
    
    def chooseTarget(self, targetList):
        chosenTarget = None
        chosenTargetPriority = 0

        for target in targetList:
            targetValue = getValue(target)
            if(targetValue > chosenTargetPriority):
                chosenTarget = target
                chosenTargetPriority = targetValue
        return chosenTarget

