# Multi-Threaded Frequent Subgraph Miner

This is a program I am fairly proud of, it took me a little while to write, especially with my limited knowledge and experience in C, but it does seem to work and it does seem to behave properly when tested for memory bugs in valgrind. This is definitely not the most efficient way to do this, but it is a good place to start. I hope to improve this implementation with at least a couple of new things:

1. Add frequency support. Right now its technically not a *frequent* subgraph miner, its just a subgraph miner. Adding size constraints should be very easy though.
3. Add support for mining only connected graphs. Right now it mines ALL possible subgraphs, which includes disconnected ones. The connected graphs are more interesting.

Useful Information: https://en.wikipedia.org/wiki/Subgraph_isomorphism_problem

## Frequent Subgraph Mining

Frequent Subgraph Mining is a fairly difficult problem in computer science (if anyone is wondering no one has actually proven that it is in NP-Hard which makes it a little unique in that respect because if someone does prove it, that means that P =/= NP). It concerns itself with finding all frequent subgraphs in a given, large graph. There many different approaches to doing this and there are many different variations on this problem. The one shown here works with labeled vertices but not labeled edges and is un-directed, however, the implementation is generalized in such a way that it can actually be modified to do either of those.

Subgraph Mining can also concern itself with different facets of graphs, in this case, we consider a subgraph to be frequent if there are enough other subgraphs of the same larger graph that are isomorphic to it. For a definition of graph isomorphism (which is a complex problem all on its own) please look here: https://en.wikipedia.org/wiki/Graph_isomorphism.

## How to Use This Code

I cannot recommend actually using this code for anything important without thoroughly checking it first. I am not an experienced C programmer. The naming is likely an atrocity to coding style (https://xkcd.com/1513/), and I cannot at all guarantee that there are no bugs. All I can say is that I ran the code on my machine using the approach you can see in the main.c file and valgrind came up with no bugs.

Anyway, if you really wanna try to use this code, this is how it is done:

First you must define your graph:
```c
Graph* G = init_graph(1,1);
```
To do this you must give it two integer values, the first is the number of vertices you expect it to have, and the second is the number of edges. Note that the graph is dynamic meaning that it will expand as needed to allow you to add more edges and vertices, but it will perform faster if you give it the right numbers right away so do that if you can.

Then you add the vertices:
```c
unsigned int a = add_vertex("A",G);
unsigned int b = add_vertex("B",G);
unsigned int c = add_vertex("C",G);
unsigned int d = add_vertex("D",G);
unsigned int e = add_vertex("E",G);
unsigned int f = add_vertex("F",G);
unsigned int g = add_vertex("G",G);
```
The `add_vertex` function takes in a string which is the label of the vertex you wish to add to the graph and returns an unsigned integer representing the index of the vertex in the array of vertices that is part of the graph implementation. You should save this value as it will allow you to connect the vertices of the graph with edges, which is, coincidentally the next step:
```c
connect_by_idx(a,b,G);
connect_by_idx(a,c,G);
connect_by_idx(a,d,G);
connect_by_idx(a,e,G);
connect_by_idx(a,f,G);
connect_by_idx(b,c,G);
connect_by_idx(b,d,G);
connect_by_idx(b,e,G);
connect_by_idx(b,f,G);
connect_by_idx(c,d,G);
connect_by_idx(c,e,G);
connect_by_idx(c,f,G);
connect_by_idx(d,e,G);
connect_by_idx(d,f,G);
connect_by_idx(e,f,G);
connect_by_idx(e,g,G);
```
This part should be fairly simple, here you just give it two vertex indices and the function creates an edge connecting them.

To execute the search for subgraphs simply execute the parallelSearch function by giving it the graph in question and the number of threads you want it to use. Take note that the number of threads needs to be a power of two, and smaller than the number of edges in the graph. I do plan on adding frequency and graph size parameters to this function.

```c
Isomorphism_Class* r = parallelSearch(G,8);
```

Finally if you wish to look at the output, you simply need to know that an IsomorphismClass is a linked list so you can loop over the whole collection of results like so:

```c
Isomorphism_Class* s = r;
int class_count = 0;
int graph_count = 0;
do {
    printf("Isomorphism Class of size %i\n",r->_count);
    print_graph(r->_example);
    class_count++;
    graph_count += r->_count;
    r = r->_next;
} while(r);
printf("%i Isomorphism Classes total.\n", class_count);
printf("%i Subgraphs total.\n", graph_count);
```
