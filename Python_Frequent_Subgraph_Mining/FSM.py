import itertools

RIG_COMPARISONS = 0

def group_graphs_by_isomorphic_profile(graphs):
	graphSet = {}
	for g in graphs:
		if g.isomorphism_profile not in graphSet:
			graphSet[g.isomorphism_profile] = []
		graphSet[g.isomorphism_profile].append(g)
	return graphSet

def rigorous_isomorphism_grouping(graphs):
	graph_set = {}
	graph_set[graphs[0]] = [graphs[0]] # create the first isomorphism class
	for i in range(1,len(graphs)):
		new_isomorphism = True
		for g in graph_set:
			if not graphs[i].is_rigorously_isomorphic(g):
				continue
			else:
				graph_set[g].append(graphs[i])
				new_isomorphism = False
				break
		if new_isomorphism:
			graph_set[graphs[i]] = [graphs[i]]
	return graph_set

class Graph(object):
	def __init__(self,vertices,edges):
		self.vertices = vertices
		self.edges = edges
		self.degree_sequence,self.line_graph_degree_sequence = self.__degree_sequences__()
		self.isomorphism_profile = (len(vertices),len(edges),self.degree_sequence,self.line_graph_degree_sequence)
	def valid_mapping(self,other,order):
		mapping = {}
		for v1,v2 in itertools.izip(self.vertices,order):
			mapping[v1] = v2
		for e in self.edges:
			s1,d1 = e
			s2,d2 = mapping[s1],mapping[d1]
			if (s2,d2) not in other.edges and (d2,s2) not in other.edges:
				return False
		return mapping
	def is_rigorously_isomorphic(self,other):
		if self.isomorphism_profile != other.isomorphism_profile: return False
		return self.is_only_rigorously_isomorphic(other)
	def is_only_rigorously_isomorphic(self,other):
		global RIG_COMPARISONS
		RIG_COMPARISONS += 1
		# test all mappings of vertices from one graph to the other
		for order in itertools.permutations(other.vertices,len(other.vertices)):
			if self.valid_mapping(other,order):
				return True
		return False
	def is_isomorphic(self,other):
		return self.isomorphism_profile == other.isomorphism_profile
	def __degree_sequences__(self):
		degree_sequence = {}
		line_graph_degree_sequence = {}
		for v1,v2 in self.edges:
			if v1 not in degree_sequence: degree_sequence[v1] = 1
			else: degree_sequence[v1] += 1
			if v2 not in degree_sequence: degree_sequence[v2] = 1
			else: degree_sequence[v2] += 1
		for v1,v2 in self.edges:
			line_graph_degree_sequence[(v1,v2)] = degree_sequence[v1] + degree_sequence[v2] - 2
		return (tuple(sorted([degree_sequence[v] for v in degree_sequence])),tuple(sorted([line_graph_degree_sequence[e] for e in line_graph_degree_sequence])))
	def subgraphs_of_size(self,size):
		for es in itertools.combinations(self.edges,size):
			vs = set([item for sublist in es for item in sublist])
			yield Graph(vs,es)
	# def frequentSubgraphsOfSize(self,threshold,size):
	# 	graphSet = {}
	# 	for i in range(size,len(self.edges)):
	# 		graphSet.update(self.generate_subgraphs_of_size(i))
	# 	d = []
	# 	for g in graphSet:
	# 		if len(graphSet[g]) < threshold: d.append(g)
	# 	for g in d:
	# 		del graphSet[g]
	# 	return graphSet
	def __repr__(self):
		s = "       | "
		for v in self.vertices:
			s += str(v) + " | "
		s += '\n'
		for v1 in self.vertices:
			s += "| " + str(v1) + " |"
			for v2 in self.vertices:
				e = '   1  |' if (v1,v2) in self.edges or (v2,v1) in self.edges else '   0  |'
				s += e
			s += '\n'
		return s

def test_isomorphism_profile(g,size):
	num_classes = 0
	graph_set = group_graphs_by_isomorphic_profile(list(g.subgraphs_of_size(size)))
	for ds in graph_set:
		rig = rigorous_isomorphism_grouping(graph_set[ds])
		num_classes += len(rig)
		# print("There are " + str(len(graph_set[ds])) + " graphs with the degree sequence of: " + str(ds) + " in " + str(len(rig)) + " isomorphism classes.")
		# if len(rig) > 1: print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> VIOLATION DISCOVERED <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<")
		# print("\tThere are " + str(len(rig)) + " rigorously computed isomorphism classes.")
		# for ric in rig:
			# print("\t\tThere are " + str(len(rig[ric])) + " isomorphic graphs in this one.")
			# print(str(rig[ric][0]))
	return num_classes

def test_isomorphism_rigorously(g,size):
	graph_set = rigorous_isomorphism_grouping(list(g.subgraphs_of_size(size)))
	# print("There are " + str(len(graph_set)) + " isomorphism classes")
	return len(graph_set)
	# for ds in graph_set:
		# print("\tThere are " + str(len(graph_set[ds])) + " rigorously computed isomorphism classes.")

a = 'A'
b = 'B'
c = 'C'
d = 'D'
e = 'E'
f = 'F'
g = 'G'
h = 'H'
i = 'I'
j = 'J'
k = 'K'
l = 'L'
m = 'M'
n = 'N'
o = 'O'
p = 'P'
q = 'Q'
r = 'R'
s = 'S'
t = 'T'
u = 'U'
v = 'V'
w = 'W'
x = 'X'
y = 'Y'
z = 'Z'
G_HARD = Graph([a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w],[(a,b),(a,c),(a,d),(a,g),(a,f),(a,e),(a,s),(a,r),(b,c),(b,g),(b,f),(b,e),(b,p),(b,q),(e,c),(e,d),(e,g),(e,f),(f,c),(f,d),(f,g),(g,c),(g,d),(d,c),(e,n),(e,o),(f,m),(f,l),(g,j),(g,k),(d,i),(d,h),(c,u),(c,t),(t,u),(h,i),(k,j),(l,m),(o,n),(p,q),(r,s),(i,x),(j,y),(l,z),(n,v),(q,w)])
G_MEDIUM = Graph([a,b,c,d,e,f,g,h],[(a,b),(a,c),(a,e),(c,g),(c,g),(d,h),(d,b),(b,f),(e,f),(e,g),(g,h),(f,h)])

def test_smart(g):
	# testing
	num_classes = 0
	for i in range(len(g.edges)):
		num_classes += test_isomorphism_profile(g,i)
	print("There are " + str(num_classes) + " isomorphism classes")
	print(str(RIG_COMPARISONS) + " rigorous comparisons were required.")

def test_naive(g):
	num_classes = 0
	for i in range(len(g.edges)):
		num_classes += test_isomorphism_rigorously(g,i)
	print("There are " + str(num_classes) + " isomorphism classes")
	print(str(RIG_COMPARISONS) + " rigorous comparisons were required.")

if __name__=='__main__':
    # test_smart(G_MEDIUM)
    test_naive(G_MEDIUM)
