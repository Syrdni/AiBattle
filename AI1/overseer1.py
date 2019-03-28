from unitManager1 import unitManager
from queueFSM1 import queueFSM
#from resourceManager1 import *
#from combatManager1 import *
#from exploreManager1 import *
from aiTypes import *
import attackPriorityList1 as apl
import json

class overseer(queueFSM):
    currentJson = None
    unitGoalRatio = ratio()
    attackPriority = apl.attackPriorityList()

    def importFromJson(path):
        with open(path) as json_file:
            overseer.currentJson = json.load(json_file)

            #Resource manager

            #Unit manager
            unitManager.unitGoalRatio = overseer.currentJson["unitManager"]["unitGoalRatio"]

            #Explore manager

            #Combat manager
    
    
    def update():
        if(overseer.stateQueue != []): 
            overseer.stateQueue[0].run()
            
        #resourceManager.update()
        unitManager.update()
        #exploreManager.update()
        #combatManager.update()

        


class resourceRush():
    def enter(self):
        pass



#Test code
#overseer.importFromJson("c:/users/ludfra-7-local/desktop/test.json")




