from enum import Enum

class npcType(Enum):
    WORKER = 0
    EXPLORER = 1
    BUILDER = 2
    SOLDIER = 3

class buildingType(Enum):
    CASTLE  = 0
    COAL    = 1
    FORGE   = 2
    SMITH   = 3

class coordinate:
    def __init__(self, x = 0, y = 0):
        self.x = x
        self.y = y

    def __add__(self, rhs):
        return coordinate(self.x + rhs.x, self.y + rhs.y)
    def __sub__(self, rhs):
        return coordinate(self.x - rhs.x, self.y - rhs.y)
    def __len__(self):
        return math.sqrt(math.pow(self.x, 2) + math.pow(self.y, 2))

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
        for index in range(1,3):
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