#
#   resourceGoalRatio: describes how craftign resources should be divided (percentage)
#   [wood, ironOre, coal, ironIngor, sword]
#
#   unitGoalRatio: describes how units should be divided (percentage)
#   [workers, explorers, builders, soldiers]
#
#   buildQueue: describes which buildings to build and their relative location to castle
#   [[Builing 1 type, Building 1 position], [Building 2 type, Building 2 position]....... [Building n type, Building n position]]
#
#
#

from aiTypes import buildingType, coordinate

class start:
    resourceGoalRatio   = [0.1, 0.1, 0.6, 0.1, 0.1]
    unitGoalRatio       = [0.6, 0.2, 0.05, 0.15] 
    buildQueue          =  [
                            [buildingType.COAL,   coordinate(0,1)], 
                            [buildingType.FORGE,  coordinate(1,0)],
                            [buildingType.COAL,   coordinate(1,1)], 
                            [buildingType.SMITH,  coordinate(0,-1)],
                            [buildingType.BARRACK,coordinate(-1,0)]
                            ]
    resourceGoalRatio   = [0.1,0.1,0.6,0.1,0.1]
