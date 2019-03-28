###VARNING!!!! KODEN ÄR TYP 90% PSEUDOKOD-ESQUE, FÖRVÄNTA ER INTE ATT DEN KÖR!!!###

#A basic queue-driven Finite State Machine

class queueFSM:
    stateQueue = [] #Holds all queued states

    def __init__(self):
        pass

    #Calls enter for current state
    def enter(self): 
        if(self.stateQueue != []):
            self.stateQueue[0].enter()

    #Machine update
    def update(self):
        if(self.stateQueue != []): 
            self.stateQueue[0].run()
    
    #Calls exit for current state
    def exit(self): 
        if(self.stateQueue != []):
            self.stateQueue[0].exit()

    #Performs state switch
    def nextState(self):
        self.exit()
        self.stateQueue.pop()
        if(self.stateQueue != []):
            self.stateQueue[0].enter()

    #Switches to a given statef
    def setState(self, state):
        self.exit()
        self.stateQueue[0] = state
        self.enter()
    
    #Switches to a given state and clears queue
    def forceState(self, state):
        self.exit()
        self.stateQueue = [state]
        self.enter()

    def __getitem__(self, key):
        if(key>len(self.stateQueue)-1):
            return None
        return self.stateQueue[key]

    def __setitem__(self, key, value):
        if(key>len(self.stateQueue)-1):
            self.stateQueue.append(value)
            return
        self.stateQueue[key] = value
    

class state: 
    def enter(self, machine):
        print("Default state start")
    def run(self):
        print("Default state run")
    def exit(self):
        print("Default state exit")
    def onMessage(self, message):
        print("Default message recieve handler")

    def __eq__(self, rhs):
        return isinstance(self, rhs)

    def __ne__(self, rhs):
        return not isinstance(self, rhs)



