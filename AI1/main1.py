import AI
from AICore1 import AICore
Main = AI.player1
from overseer1 import overseer
from overseerStates1 import *
def startup():
    overseer.loadSettings(start)
    pass

def update():
    overseer.update()
    pass

def onMessage():
    message = AICore.GetMessage()
    overseer.onMessage(message)