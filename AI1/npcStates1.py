###VARNING!!!! KODEN �R TYP 90% PSEUDOKOD-ESQUE, F�RV�NTA ER INTE ATT DEN K�R!!!###

#Contains states used by NPCs
#
# Currently (semi-)implemented states and status:
#   General: 
#     state(base):
#     idle:  
#     moveTo:  
#     flee: 
#     upgrade:
#
#   Worker: 
#     harvest: 
#     collectResource: 
#     returnResource: 
#     upgrade:
#
#   Builder: 
#     build:
#   
#   Soldier:
#     moveToAndAttack:

from types import *
from combatManager1 import *
from AICore1 import AICore
from queueFSM1 import state

#-----GENERAL-----

#To keep base state separate from idle state (precaution)
class idle(state):
    def enter(self):
        #Send message to handler and ask for new task
        pass

class moveTo(state):
    def __init__(self, machine, destination):
        self.destination = destination
        self.machine = machine

    def enter(self):
        AICore.moveTo(self.machine.id, self.destination.x, self.destination.z)

    def exit(self):
        AICore.cancel(self.machine.id)

    def onMessage(self, message):
        if(message["type"] == moveComplete):
            self.machine.nextState()
        


#Escapes from nearby enemies for a couple of ticks (might change to use delayed messages instead)
class flee(state):
    def __init__(self, machine, tickThreshHold):
        self.ticksSinceLastEnemy = 0
        self.tickThreshHold = tickThreshHold
        self.machine = machine

    def run(self):

        #Search for nearby enemies
        enemiesAround = combatManager.getEnemiesWithinRange(position, radius) #Get enemies within range
        deltaSum = (0,0)

        if(len(enemiesAround)==0):
            self.ticksSinceLastEnemy += 1
            if(self.ticksSinceLastEnemy >= self.tickThreshHold):
                self.machine.nextState()
        else:
            self.ticksSinceLastEnemy = 0
            for enemy in enemiesAround:
                delta = (self.machine.pos - enemy.position)
                deltaSum += delta.norm()/delta.length()

        self.machine.move(deltaSum.norm())

        



#-----WORKER-----

class harvest(state):
    def __init__(self, machine, objectToHarvest): #harvestObject = ID perhaps??
        self.objectToHarvest = objectToHarvest
        self.machine = machine

    def run(self):
        if((self.machine.position-self.objectToHarvest.position).abs()<=(1,1)): #Range check
            #Performs harvesting
            pass
        else:
            #Change state to moveTo(position of object to harvest)
            machine.goIdle()

        if(not objectToHarvest.exists):
            #Do next state
            pass

#Picks up a resource within reach
class collectResource(state):
    def __init__(self, machine, objectToCollect):
        self.objectToCollect = objectToCollect
        self.machine = machine

    def enter(self):
        resourcesAround = ()
        if resourcesAround:
            self.machine.goIdle()

    def run(self):
        if(self.objectToCollect.exists):
            self.machine.pickup(resource)

class returnResource(state):
    def enter(self):
        if(home.isWithinRange):
            self.machine.dropOff()

class upgrade(state):
    def __init__(self, machine, goalType):
        self.goalType = goalType
        self.machine = machine
    def enter(self):
        #AICore.upgrade(machine.id, self.goalType, machine.id)
        pass
    def onMessage(self, message):
        if(message["type"] == upgradeComplete):
            self.machine.changeType(self.goalType)

#-----BUILDER-----

class build(state):
    def __init__(self, machine, building):
        self.building = building
        self.machine = machine
    def run(self):
        self.buildling.addProgress()
        if(building.isComplete()):
            self.machine.goIdle()

#-----SOLDIER-----

#Basically moveTo but is allowed to attack enemies and buildings if deemed appropriate by given attackPriorityList
class moveToAndAttack(state):

    def __init__(self, machine, destination, attackPriority = attackPrioritylist()):
        self.destination = destination
        self.attackPriority = attackPriority
        self.machine = machine

    def enter(self):
        pathFinder.pathFindTo(destination)

    def update(self):
        attackRadius = 3
        if(attackRadius>0):
            targets = getEnemiesAndBuildingsWithinRange() 
            highestValue = 0
            attackTarget = None
            for target in targets:
                priorityValue = attackPriority.getValue(target)
                if(priorityValue>highestValue):
                    highestValue = priorityValue
                    attackTarget = target
            if(attackTarget != None):
                #Change to attack state
                pass

    def exit(self):
        #Stop path follow
        pass

    def onMessage(self, message):
        #Parse messages from pathFollower
        pass

class attack(state):

    def __init__(self, machine, target, attackPriority = attackPrioritylist()):
        self.target = target
        self.attackPriority = attackPriority
        self.machine = machine

    def enter(self):
        AICore.attack(self.machine.id, self.target.id)

    def update(self):
        pass

    def exit(self):
        #Stop path follow
        pass

    def onMessage(self, message):
        #Parse messages from pathFollower
        pass

###########################################################################################

#List given to soldiers to guide them which enemies to prioritize
