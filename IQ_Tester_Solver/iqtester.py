slots = set([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14])
moves = [(3,1,0),(0,1,3),(6,3,1),(1,3,6),(10,6,3),(3,6,10),(11,7,4),(4,7,11),(10,11,12),(12,11,10),(11,12,13),(13,12,11),(12,13,14),(14,13,12),(12,8,5),(5,8,12),(6,7,8),(8,7,6),(7,8,9),(9,8,7),(5,9,14),(14,9,5),(3,4,5),(5,4,3),(0,2,5),(5,2,0),(7,4,2),(2,4,7),(1,4,8),(8,4,1),(4,8,13),(13,8,4),(3,7,12),(12,7,3),(2,5,9),(9,5,2)]
moves_for_empty_slot = {}

for slot in slots: moves_for_empty_slot[slot] = [m for m in moves if m[2] == slot]

class State(object):
	def __init__(self,empty_list,path = [],move = None):
		self.e = set(empty_list)
		self.o = slots - self.e
		self.path = list(path)
		if move:
			self.path.append(move)
			self.e = (self.e - set([move[2]])) | set([move[0],move[1]])
			self.o = (self.o - set([move[0],move[1]])) | set([move[2]])
	def moves(self):
		moves = []
		for empty_slot in self.e: moves.extend([m for m in moves_for_empty_slot[empty_slot] if m[0] in self.o and m[1] in self.o])
		return moves
	def child_states(self): return [State(list(self.e),self.path,m) for m in self.moves()]

def search(start_state):
	stack = start_state.child_states()
	while stack:
		s = stack.pop()
		if len(s.o) == 1: return s.path
		elif not len(s.moves()) == 0: stack.extend(s.child_states())

solution = search(State([7]))
for s in solution: print("MOVE: " + str(s[0]) + " TO: " + str(s[2]) + " ELIM: " + str(s[1]))