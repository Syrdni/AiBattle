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
#To keep base state separate from idle state (precaution)


from queueFSM1 import state
class idle(state):
    def enter(self):
        #Send message to handler and ask for new task
        pass

from aiTypes import *
from attackPriorityList1 import attackPriorityList
from AICore1 import AICore
import overseer1 
#from architectManager1 import architectManager

#-----GENERAL-----


class moveTo(state):
    def __init__(self, machine, destination):
        self.destination = destination
        self.machine = machine

    def enter(self):
        AICore.MoveTo(self.machine.id, self.destination.x, self.destination.y)

    def exit(self):
        #AICore.cancel(self.machine.id)
        print("Moveto exit")
        pass

    def onMessage(self, message):
        if(message["type"] == "moveComplete"):
            self.machine.nextState()

#Walks in a direction determined by given angle (tangent). Makes new pathfind every 'step' units
class walkInDirection(state):
    step = 10
    #Defines map area (defined by exploreManager)
    min = coordinate() 
    max = coordinate()

    def __init__(self, machine, tangent, maxThreshold, minThreshold):
        self.machine = machine
        self.tangent = tangent
        self.canMove = True
        self.dx = int(walkInDirection.step * tangent)
        self.dy = int(walkInDirection.step / tangent)
        self.maxThreshold = maxThreshold
        self.minThreshold = minThreshold 

    def enter(self):
        self.lastPosition = coordinate.fromStruct(AICore.GetPos(self.machine.id))
        print(str(self.lastPosition))

    def run(self):
        #Checks if currently pathfinding
        if(self.canMove):
            #Checks if within search area, goes to next state if not
            if(self.lastPosition.x > self.maxThreshold.x or self.lastPosition.y > self.maxThreshold.y or
               self.lastPosition.x < self.minThreshold.x or self.lastPosition.y < self.minThreshold.y):
                self.machine.nextState()
            else:
                #Calculates new position and pathfinds there
                self.canMove = False
                self.lastPosition.x = max(min(self.lastPosition.x + self.dx, self.max.x),self.min.x)
                self.lastPosition.y = max(min(self.lastPosition.y + self.dy, self.max.y),self.min.y)
                AICore.MoveTo(self.machine.id, self.lastPosition.x, self.lastPosition.y)

    def exit(self):
        #AICore.cancel(self.machine.id)
        print("Moveto exit")
        pass

    def onMessage(self, message):
        if(message["type"] == "moveComplete"):
            self.canMove = True


#Escapes from nearby enemies for a couple of ticks (might change to use delayed messages instead)
class flee(state):
    def __init__(self, machine, tickThreshHold):
        self.ticksSinceLastEnemy = 0
        self.tickThreshHold = tickThreshHold
        self.machine = machine

    def run(self):

        #Search for nearby enemies
        #enemiesAround = combatManager.getEnemiesWithinRange(position, radius) #Get enemies within range
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
    def __init__(self, machine, objectToHarvest):
        self.objectToHarvest = objectToHarvest
        self.machine = machine

    def enter(self):
        AICore.HarvestResource(self.machine.id, self.objectToHarvest)

    def onMessage(self, message):
        if(message["type"] == "tasksComplete"):
            self.machine.nextState()
        if(message["type"] == "harvestFailed"):
            self.machine.goIdle()

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
    def __init__(self, machine, goalType, upgradeLocation):
        self.goalType = goalType
        self.machine = machine
        self.upgradeLocation = upgradeLocation

    def enter(self):
        AICore.Upgrade(self.machine.id, self.goalType, self.upgradeLocation)
        self.machine.autoFlee = False
        pass

    def onMessage(self, message):
        if(message["type"] == "upgradeComplete" or message["type"] == "tasksComplete" ):
            self.machine.type = self.goalType
            self.machine.goIdle()


#-----BUILDER-----

class build(state):
    def __init__(self, machine, building):
        self.building = building
        self.machine = machine
    def enter(self):
        print(self.building.id)
        AICore.Build(self.machine.id, self.building.id)
    def exit(self):
        if(not self.building.isComplete):
            self.building.currentlyWorkedOn = False
    def onMessage(self, message):
        if(message["type"] == "buildComplete" or message["type"] == "tasksComplete"):
            self.building.isComplete = True
            self.machine.forceState(moveTo(self.machine,overseer1.castle.position))

class craft(state):
    def __init__(self, machine, building, resource):
        self.machine = machine
        self.building = building
        self.resource = resource
    def enter(self):
        AICore.Craft(self.machine.id, self.building.id)
    def exit(self):
        pass
    def onMessage(self, message):
        if(message["type"] == "craftComplete" or message["type"] == "tasksComplete"):
            overseer1.overseer.addResource(self.resource)
            self.building.isBusy = False
            self.machine.goIdle()

#-----SOLDIER-----

#Basically moveTo but is allowed to attack enemies and buildings if deemed appropriate by given attackPriorityList
class moveToAndAttack(state):

    def __init__(self, machine, destination, attackPriority = attackPriorityList()):
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
    reloadRate = 100

    def __init__(self, machine, target, attackPriority = attackPriorityList()):
        self.target = target
        self.attackPriority = attackPriority
        self.machine = machine
        self.tick = 0
        self.canAttack = True

    def enter(self):
        AICore.Attack(self.machine.id, self.target)
        pass

    def run(self):
        #self.tick += 1
        #print("Tick: " + str(self.tick))
        #if(self.tick % attack.reloadRate == 0):
        #    AICore.Abort(self.machine.id)
        #    AICore.Attack(self.machine.id, self.target)
        if(self.canAttack):
            print("I'M GONNA FUCK UP THIS ID: " + str(self.target))
            self.canAttack = False
            self.enter()
            #AICore.Attack(self.machine.id, self.target)

    def exit(self):
        #Stop path follow
        pass

    def onMessage(self, message):
        if(message["type"] == "attackFailed"):
            print("I'M A FUCKING FAILRUE AJKFNSJKAJBGJHABBGHJABGBABGJ")
            self.machine.goIdle()
        elif(message["type"] == "attackSuccessful"):
            print("I AM SUCCESSFUL ATTACKER PERSON WOOOOOOOOOOOOOOOOOOOOOOOOOOOO")
            self.canAttack = False
        elif(message["type"] == "canAttack"):
            print("I'M BOUT TO FUCK THIS NEIGHBOUR UP")
            self.canAttack = True

###########################################################################################

#List given to soldiers to guide them which enemies to prioritize
