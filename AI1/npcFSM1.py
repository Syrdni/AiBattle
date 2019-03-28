###VARNING!!!! KODEN ÄR TYP 90% PSEUDOKOD-ESQUE, FÖRVÄNTA ER INTE ATT DEN KÖR!!!###
from enum import Enum
from queueFSM1 import queueFSM
from npcStates1 import idle
from types import *


class npcBase(queueFSM):
    def __init__(self, id, positionRef, startState = idle()):
        self.id = id
        self.type = npcType.WORKER
        self.autoFlee = True
        self.stateQueue = [startState]
        self.position = positionRef
        
    #Machine update
    def update(self):
        if(self.stateQueue != []): 
            self.stateQueue[0].run()
        if(autoFlee):
            if(enemiesAround()):
                self.stateQueue.append(flee())


    def goIdle(self):
        self.exit()
        self.stateQueue = [idle()]
        self.enter()

    def onMessage(self, message):
        self[0].onMessage(message)

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


