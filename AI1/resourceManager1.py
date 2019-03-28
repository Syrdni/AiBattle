#THIS CLASS IS WRITTEN AS PSEUDOCODE, USE IT AS A REFERENCE AND NOT WORKING SOFTWARE
from npcStates1 import moveTo
from aiTypes import *

class resourceManager:

    resourceQuantity = {'wood' : 0, 'iron' : 0}
    
    #dict structure: 
    #key: distance
    #value: entity ID

    treeDict = {}
    ironDict = {}
    
    def onMessage(self,message):
        basePosition = message["entityType"]=="castle".coordinate(float(message["posx"]),float(message["posy"]))
        if(message["entityType"] == "tree"):
            entityPos = coordinate(float(message["posx"]),float(message["posy"]))
            treeDict.update[(basePosition - entityPos).__len__()] = int(message["ID"])
            
            
        elif(message["entityType"] == "ironOre"):
            entityPos = coordinate(float(message["posx"]),float(message["posy"]))
            ironDict.update[(basePosition - entityPos).__len__()] = int(message["ID"])
        
        else:
            pass

        
    #def getXYresource(self):
    #    x,y = coordDict["tree"].split("/")
    #   return x,y

    #type, x, y
    resourceLocation = {''}
    treeList = []


    #Is there anything we need to initiate within a resourceManager?
    def __init__(self):
        pass

        #kolla entitytype, om tree:
            #lägg till i tree listan och hämta posx och posy
        #kolla entitytype, om ironOre:
            #lägg till i ore listan och hämta posx och posy

    def onMessage(scoutMessage): #get info from scouts and append resources to lists
        pass


    #evaluate what needs to be done and append it as task
    def appendTask(workerMessage):
        if workerMessage.sender == idle:
            worker.changeState(checkWhatNeedsToBeDone()) #temp

    #In case we're gathering improportionately, should this run every frame?
    def balanceGathering():

        #if we're gathering 20 or more percent more wood than iron
        if resourceManager.resources.get('wood') >= (resourceManager.resources.get('iron') * resourceManager.diffPercentage):
            #send a couple of workers cutting wood to mine iron instead
            pass

        #if we're gathering 20 or more percent more iron than wood
        if resourceManager.resources.get('iron') >= (resourceManager.resources.get('wood') * resourceManager.diffPercentage):
            #send a couple of workers mining iron to cut wood instead
            pass


        #kolla så det är rätt resurstyp
        #kolla så resursen inte är upptagen
        #kolla så vilken som är närmast

    #Ranks and prioritizes all the available resources
    def rankResourcesDistance(resources):

        resourceDistances = []

        #distance will be int because we're working with tiles
        for resource in resources:
            #check what resource type we need here
                if resource.busy == False: #If someone is already working on the resource avoid conflicts
                    distance = resource.measureDistance(worker.location)
                    resourceDistances.append(distance)
        resourceDistances.sort()

        for resource in resources:
            if resource.measureDistance(toBase) == resourceDistances[0]:
                return resource #returns closest resource
        return


    #Force the workers to retreat to base
    def retreatWorkers(workers):
        for unit in workers:
            unit.forceState(moveTo(unit, basePosition))

