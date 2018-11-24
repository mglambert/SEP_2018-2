

class GateStates():
	Closed, Opening, Opened, Closing, Stopped, Error = range(6)

	@staticmethod
	def ToString(state):
		return ['Closed', 'Opening', 'Opened', 'Closing', 'Stopped', 'Error'][state]


class Gate():
	def __init__(self, state=GateStates.Closed):
		self.state = state
		self.prev_state = state

	def setState(self, newState):
		self.prev_state = self.state
		self.state = newState
