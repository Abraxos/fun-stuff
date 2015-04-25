#ifndef GRAPH_H
#define GRAPH_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define TRUE        1
#define FALSE       0

#define TAKE_EDGE   1
#define LEAVE_EDGE -1
#define UNKNOWN     0

#define VERTEX_DOES_NOT_EXIST -1

typedef struct Vertex 
{
    const char*     _label;
    unsigned int      _idx;
    unsigned int _numEdges;
} Vertex;

typedef struct Edge
{
    unsigned int _v1;
    unsigned int _v2;
} Edge;

typedef struct Graph 
{
    Vertex**        _vertices;
    Edge**             _edges;
    unsigned int _numVertices;
    unsigned int    _numEdges;
    unsigned int** _adjMatrix;
    unsigned int       _sizeV;
    unsigned int       _sizeE;
} Graph;

typedef struct FSM_SearchState
{
    int*                     _edges;
    int                       _size;
    int                        _idx;
    unsigned int          _numEdges;
    struct FSM_SearchState*   _next;
    struct FSM_SearchState*   _prev;
} FSM_SearchState;

typedef struct FSM_SearchStack
{
    int                _size;
    FSM_SearchState*    _top;
    FSM_SearchState* _bottom;
} FSM_SearchStack;

typedef struct FSM_Search_Queue
{
    int               _count;
    FSM_SearchState*    _top;
    FSM_SearchState* _bottom;
} FSM_Search_Queue;

typedef struct Isomorphism_Class
{
    unsigned int                 _size;
    unsigned int                  _idx;
    unsigned int                _count;
    Graph*                    _example;
    Graph**                    _graphs;
    struct Isomorphism_Class*    _next;
    struct Isomorphism_Class*    _prev;
} Isomorphism_Class;

typedef struct WorkerTParam
{
    FSM_SearchState*  _startState;
    Graph*                 _graph;
    Isomorphism_Class*   _results;
} WorkerTParam;

// General Utility Functions
unsigned int factorial(int i);
unsigned int** permutations(unsigned int* array, int size);
void print_matrix(unsigned int** m, unsigned int rs, unsigned int cs);
void free_matrix(unsigned int** m, unsigned int nr);

// Graph Isomorphism functions
int isomorphic(Graph* g1,Graph* g2);
int smart_isomorphic(Graph* g1,Graph* g2);
unsigned int* degree_sequence(Graph* g);
unsigned int* line_graph_degree_sequence(Graph* g);
int degree_seq_eq(unsigned int* ds1, unsigned int s1, unsigned int* ds2, unsigned int s2);

// Search Utilities
void pushState(FSM_SearchStack* s, FSM_SearchState* f);
FSM_SearchState* popState(FSM_SearchStack* s);
void enqueueState(FSM_Search_Queue* q, FSM_SearchState* f);
FSM_SearchState* dequeueState(FSM_Search_Queue* q);
FSM_SearchStack* init_search_stack();
FSM_Search_Queue* init_search_queue();
FSM_SearchState* init_search_state(Graph* g);
void free_state(FSM_SearchState* s);
void free_iso_class(Isomorphism_Class* ic, int free_example_graph);
void free_iso_classes(Isomorphism_Class* ic);
int completely_labeled_state(FSM_SearchState* s);
int emptyStack(FSM_SearchStack* s);
int emptyQueue(FSM_Search_Queue* q);
Isomorphism_Class* init_iso_class(Graph* g);
int add_to_iso_class(Graph* g, Isomorphism_Class* ic);
void merge_isomorphism_class_counters(Isomorphism_Class* to, Isomorphism_Class* from);
int increment_iso_class(Graph* g, Isomorphism_Class* ic);
FSM_SearchState** generate_initial_search_states(Graph* g, unsigned int numStates);

// Subgraph Mining Functions
FSM_SearchState** child_states(FSM_SearchState* s);
Graph* graph_from_search_state(FSM_SearchState* s, Graph* parentGraph);
Isomorphism_Class* basicSearch(Graph* g);
Isomorphism_Class* parallelSearch(Graph* g, int numThreads);
Isomorphism_Class* searchFromState(FSM_SearchState* f, Graph* g);
void search_state(Graph* p, FSM_SearchState* f, Isomorphism_Class* ic);

// Basic graph functions
Graph* init_graph(unsigned int sizeV, unsigned int sizeE);
unsigned int add_vertex(const char* label, Graph* g);
void connect_by_idx(unsigned int x, unsigned int y, Graph* g);
void connect_by_label(const char* l1, const char* l2, Graph* g);
void free_graph(Graph* g);
void print_graph(Graph* g);

#endif	/* GRAPH_H */

