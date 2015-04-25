from enum import Enum

slots = set([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14])
moves = [(3,1,0),(0,1,3),(6,3,1),(1,3,6),(10,6,3),(3,6,10),(11,7,4),(4,7,11),(10,11,12),(12,11,10),(11,12,13),(13,12,11),(12,13,14),(14,13,12),(12,8,5),(5,8,12),(6,7,8),(8,7,6),(7,8,9),(9,8,7),(5,9,14),(14,9,5),(3,4,5),(5,4,3),(0,2,5),(5,2,0),(7,4,2),(2,4,7),(1,4,8),(8,4,1),(4,8,13),(13,8,4),(3,7,12),(12,7,3),(2,5,9),(9,5,2)]
moves_for_empty_slot = {}
for slot in slots: moves_for_empty_slot[slot] = [m for m in moves if m[2] == slot]
class GameMode(Enum):
	Easy = 1
	Medium = 2
	Hard = 3

START_EMPTY = 6
MODE = GameMode.Hard

class State(object):
	def __init__(self,empty,path = [],move = None, start_empty = None):
		empty_list = empty if isinstance(empty, list) else [empty]
		self.s = empty if isinstance(empty, int) else start_empty
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
	def win(self,mode):
		if mode == GameMode.Easy: return len(self.o) == 1
		elif mode == GameMode.Medium: return len(self.o) == 1 and list(self.o)[0] == self.s
		elif mode == GameMode.Hard: return len(self.o) == 8 and not self.moves()
	def child_states(self): return [State(list(self.e),self.path,m,self.s) for m in self.moves()]

def search(start_empty,mode):
	start_state = State(start_empty)
	stack = start_state.child_states()
	while stack:
		s = stack.pop()
		if s.win(mode): return s.path
		elif not len(s.moves()) == 0: stack.extend(s.child_states())

solution = search(START_EMPTY,MODE)
if solution:
	for s in solution: print("MOVE: " + str(s[0]) + " TO: " + str(s[2]) + " ELIM: " + str(s[1]))
else: print("No solution exists.")
