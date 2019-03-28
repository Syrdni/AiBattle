import AI

class __AICore__:
	ai = AI.player1.ai
	msg = AI.player1
	
	def GetMessage(self):
		return __AICore__.msg.msg
	
	def __getattr__(self, name):
		return getattr(self.ai, name)
		
AICore = __AICore__()