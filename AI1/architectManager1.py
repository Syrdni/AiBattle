from queueFSM1 import * 
from npcFSM1 import *
from npcStates1 import *

##TODO: kanske ha metoder för de olika lägena såsom defcon eller offensive/defensive -
##för att prioritera byggnader och platser för byggnader. det kanske inte ska vara här utan hos strategen

class architectManager:
    buildQueue = []
    buildableTerrain = []
    unPassableTerrain = []
    treeNodes = []
    ironNodes = []
    def __init__(self):
        pass

    def addBuilding(self,buildingType):
        self.buildQueue.append(buildingType)

    def buildBuilding(self):
        #Måste checka om tilen är ett "M", annars går det inte att bygga
        #Det är viktigast när AI:t väljer plats autonomt
        #Kanske värt att kolla om vissa byggnader finns redan
        if(self.buildQueue[0] != None):
            if(self.buildQueue[0] == KOLMILA):
                buildLocation = castle.pos(x,y+2)
                builder.build(self.buildQueue[0],buildLocation)
                self.buildQueue.pop[0]

            elif(self.buildQueue[0] == SMEDJA):
                buildLocation = castle.pos(x,y-2)
                builder.build(self.buildQueue[0],buildLocation)
                self.buildQueue.pop[0]

            elif(self.buildQueue[0] == SMÄLTVERK):
                buildLocation = castle.pos(x+2,y)
                builder.build(self.buildQueue[0],buildLocation)
                self.buildQueue.pop[0]

            elif(self.buildQueue[0] == TRÄNINGSLÄGER):
                buildLocation = castle.pos(x-2,y)
                builder.build(self.buildQueue[0],buildLocation)
                self.buildQueue.pop[0]
    
        else:
            if(defcon >= 3):
                builder.idle()
            else:
                builder.flee()

    def changePrority(self,buildingType): #flyttar den valda buildingType till först på listan
        if self.buildQueue.count(buildingType) >= 1:
            x = self.buildQueue.index(buildingType)
            self.buildQueue.pop(x)
            self.buildQueue.insert(0,buildingType)
        else:
            pass


    def terrainAnalysis(self): #kan säkert på nåt sätt attachas på explorer så man slipper
        for tiles in clearedFog: #stega igenom varje tile om och om igen
            if tiles.tag("M"):
                buildableTerrain.append(tiles.pos())

            elif tiles.tag("B") or tiles.tag("V"):
                unPassableTerrain.append(tiles.pos())

            elif tiles.tag("T"):
                treeNodes.append(tiles.pos())

            elif tiles.tag("I"):
                ironNodes.append(tiles.pos())

            else:
                pass
    
    def findNode(self,nodeType): #returnerar just nu bara kopia på listor,
        if nodeType == "M":      #den borde bara returnera typ närmsta
            buildableTerrain.copy()
        elif nodeType == "B" or nodeType == "V":
            unPassableTerrain.copy()
        elif nodeType == "T":
            treeNodes.copy()
        elif nodeType == "I":
            ironNodes.copy()
        else:
            pass