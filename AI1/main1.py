import AI
from AICore1 import AICore
Main = AI.player1

import overseer1
def update():
   overseer1.overseer.update()
   pass

def onMessage():
    message = AICore.GetMessage()
    if(message["type"] == "tasksComplete" or message["type"] == "moveComplete" or message["type"] == "buildComplete"):
        unitManager.getWorkerById(message["id"]).onMessage(message)
    elif(message["type"] == "entityFound"):
        if(message["team"]=="0"):
            resourceManager.onMessage(message)
        elif(message["team"]=="1"):
            pass
        elif(message["team"]=="2"):
            
            pass
        else:
            pass

