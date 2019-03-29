from enum import Enum
from AICore1 import AICore
class npcType(Enum):
    WORKER = 0
    EXPLORER = 1
    BUILDER = 2
    SOLDIER = 3

class buildingType(Enum):
    CASTLE  = 0
    COAL    = 1
    SMITH   = 2
    BARRACK = 3
    FORGE   = 4

class coordinate:
    worldspaceRatio = 2
    def __init__(self, x = 0, y = 0):
        self.x = x
        self.y = y

    def fromStruct(struct):
        return coordinate(int(struct.worldX/AICore.tileSize), int(struct.worldZ/AICore.tileSize))

    def fromWorldSpace(x,y):
        return coordinate(int(x/AICore.tileSize), int(y/AICore.tileSize))

    def min(self):
        return self.x if self.x < self.y else self.y
    
    def _int_(self):
        return coordinate(int(self.x),int(self.y))

    def __add__(self, rhs):
        return coordinate(self.x + rhs.x, self.y + rhs.y)
    def __sub__(self, rhs):
        return coordinate(self.x - rhs.x, self.y - rhs.y)
    def __mul__(self, rhs):
        return coordinate(int(self.x * rhs), int(self.y * rhs))
    def getAbs(self):
        return coordinate(abs(self.x), abs(self.y))
    def __len__(self):
        return math.sqrt(math.pow(self.x, 2) + math.pow(self.y, 2))
    def __floordiv_(self, rhs):
        return coordinate(int(self.x/rhs), int(self.y/rhs))
    def __str__(self):
        return "(" + str(self.x) + "," + str(self.y) + ")"

class unitRatio:
    def __init__(self, workers = 0.5, scouts = 0.5, builders = 0, soldiers = 0):
        self.workers  = workers
        self.scouts   = scouts
        self.builders = builders
        self.soldiers = soldiers

    def __add__(self, rhs):
        return unitRatio(self.workers   +  rhs.workers,
                     self.scouts    +  rhs.scouts,
                     self.builders  +  rhs.builders,
                     self.soldiers  +  rhs.soldiers)

    def __sub__(self, rhs):
        return unitRatio(self.workers   -  rhs.workers,
                     self.scouts    -  rhs.scouts,
                     self.builders  -  rhs.builders,
                     self.soldiers  -  rhs.soldiers)

    def __mul__(self, factor):
        return unitRatio(self.workers   *  factor,
                     self.scouts    *  factor,
                     self.builders  *  factor,
                     self.soldiers  *  factor)

    def __truediv__(self, denominator):
        if denominator == 0:
            return unitRatio(0,0,0,0)
        return unitRatio(self.workers   /  denominator,
                     self.scouts    /  denominator,
                     self.builders  /  denominator,
                     self.soldiers  /  denominator)

    #Replaces all negative values with 0
    def clearNegatives(self):
        ret = unitRatio(self.workers, self.scouts, self.builders, self.soldiers)
        for index in range(0,3):
            if(ret[index] < 0): 
                ret[index] = 0
        return ret

    def normalize(self):
        sum = self.workers + self.scouts + self.builders + self.soldiers
        return unitRatio(self.workers, self.scouts, self.builders, self.soldiers)/sum

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
        return unitRatio(len(unitManager.workers), len(unitManager.scouts), len(unitManager.builders), len(unitManager.soldiers))/unitManager.unitCount

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