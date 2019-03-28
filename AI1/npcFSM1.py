###VARNING!!!! KODEN ÄR TYP 90% PSEUDOKOD-ESQUE, FÖRVÄNTA ER INTE ATT DEN KÖR!!!###
from enum import Enum
from queueFSM1 import queueFSM
from npcStates1 import idle
from aiTypes import *


class npcBase(queueFSM):
    def __init__(self, id, positionRef, startState = idle()):
        self.id = id
        self.type = npcType.WORKER
        self.autoFlee = True
        self.stateQueue = [startState]
        
    #Machine update
    def update(self):
        if(self.hasState()): 
            self.stateQueue[0].run()
        else:
            pass
            #self.stateQueue.append(npcState.idle())
        #if(self.autoFlee):
            #if(enemiesAround()):
                #self.stateQueue.append(flee())


    def goIdle(self):
        self.exit()
        self.stateQueue = [idle()]
        self.enter()

    def onMessage(self, message):
        if(self.hasState()): 
            self[0].onMessage(message)


    def hasState(self):
        if(len(self.stateQueue)==0): 
            self.stateQueue = [idle()]
        return True

class soldier(npcBase):
    def __init__(self,id = None, startState = idle()):
        self.id = id
        self.attackPriorityList = None
        self.stateQueue = [startState]
    #Machine update
    def update(self):
        if(self.stateQueue != []): 
            self.stateQueue[0].run()
        enemiesAround = []
        if(len(enemiesAround)!=0):
            if(autoFlee):
                self.stateQueue.append(flee())
            elif(self.attackPriorityList != None):
                self.attackPriorityList.chooseTarget(enemiesAround)


