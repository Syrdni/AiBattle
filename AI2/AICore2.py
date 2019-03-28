import AI

class __AICore__:
	ai = AI.player2.ai
	msg = AI.player2
	
	def GetMessage(self):
		return __AICore__.msg.msg
	
	def __getattr__(self, name):
		return getattr(self.ai, name)
		
AICore = __AICore__()