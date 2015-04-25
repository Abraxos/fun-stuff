#include "graph.h"

void print_unsigned_arr(unsigned int* a, unsigned int s)
{
    unsigned int i;
    for (i = 0; i < s; i++)
    {
        printf(" %i",a[i]);
    }
    printf("\n");
}

void print_arr(int* a, unsigned int s)
{
    unsigned int i;
    for (i = 0; i < s; i++)
    {
        printf(" %i,",a[i]);
    }
    printf("\n");
}

void free_matrix(unsigned int** m, unsigned int nr)
{
    if (nr > 1)
    {
        unsigned int i;
        for (i = 0; i < nr; i++) free(m[i]);
        free(m);
    }
}

unsigned int factorial(int i)
{
    unsigned int sum = 1;
    while(i > 0)
    {
        sum *= i--;
    }
    return sum;
}

void swap(unsigned int* a, unsigned int x, unsigned int y)
{
    unsigned int t = a[x];
    a[x] = a[y];
    a[y] = t;
}

unsigned int* copy_unsigned_array(unsigned int* a, unsigned int size)
{
    int i;
    unsigned int* c = (unsigned int*)malloc(sizeof(unsigned int) * size);
    for (i = 0; i < size; i++) c[i] = a[i];
    return c;
}

int* copy_array(int* a, int size)
{
    int i;
    int* c = (int*)malloc(sizeof(int) * size);
    for (i = 0; i < size; i++) c[i] = a[i];
    return c;
}

int isPowerOfTwo (unsigned int x)
{
    return ((x != 0) && !(x & (x - 1)));
}

unsigned int shift_log2(unsigned int x)
{
    unsigned int n;
    for (n = 1; x > 2; n++)
    {
        x = x >> 1;
    }
    return n;
}

void permute(unsigned int** p, unsigned int* pi, unsigned int* a, int fi, int s)
{
    if (fi < s - 1)
    {
        unsigned int i;
        fi++;
        for (i = fi; i < s; i++)
        {
            unsigned int* c = copy_unsigned_array(a,s);
            swap(c,fi,i);
            permute(p,pi,c,fi,s);
            if (fi < s - 1) free(c);
        }
    }
    else p[(*pi)++] = a;
}

void print_matrix(unsigned int** m, unsigned int rs, unsigned int cs)
{
    printf("Printing Matrix\n");
    unsigned int r,c;
    for (r = 0; r < rs; r++)
    {
        for (c = 0; c < cs; c++)
        {
            printf(" %i",m[r][c]);
        }
        printf("\n");
    }
}

unsigned int** permutations(unsigned int* array, int s)
{
    unsigned int** p = (unsigned int**)malloc(sizeof(unsigned int*) * factorial(s));
    unsigned int i = 0;
    permute(p,&i,array,-1,s);
    return p;
}

unsigned int* vertex_indices(Graph* g)
{
    unsigned int i;
    unsigned int* vi = (unsigned int*)malloc(sizeof(unsigned int) * g->_numVertices);
    for (i = 0; i < g->_numVertices; i++)
    {
        vi[i] = g->_vertices[i]->_idx;
    }
    return vi;
}

int smart_isomorphic(Graph* g1,Graph* g2)
{
    if (g1->_numVertices != g2->_numVertices || g1->_numEdges != g2->_numEdges)
    {
        return FALSE;
    }
    
    unsigned int* ds1 = degree_sequence(g1);
    unsigned int* ds2 = degree_sequence(g2);
    if (!degree_seq_eq(ds1, g1->_numVertices, ds2, g2->_numVertices))
    {
        free(ds1);
        free(ds2);
        return FALSE;
    }
    free(ds1);
    free(ds2);
    
    ds1 = line_graph_degree_sequence(g1);
    ds2 = line_graph_degree_sequence(g2);
    if (!degree_seq_eq(ds1, g1->_numEdges, ds2, g2->_numEdges))
    {
        free(ds1);
        free(ds2);
        return FALSE;
    }
    free(ds1);
    free(ds2);
    
    return isomorphic(g1,g2);
}

int isomorphic(Graph* g1,Graph* g2)
{
    if (g1->_numVertices != g2->_numVertices || g1->_numEdges != g2->_numEdges) return FALSE;
    
    unsigned int i,j,valid_mapping_found = FALSE;
    unsigned int mapping_invalid;
    unsigned int* vi = vertex_indices(g2);
    unsigned int** p = permutations(vi,g2->_numVertices);
    unsigned int num_permutations = factorial(g2->_numVertices);
    free(vi);
    
    for (i = 0; i < num_permutations; i++)
    {
        mapping_invalid = FALSE;
        unsigned int* m = p[i];
        for (j = 0; j < g1->_numEdges && !mapping_invalid; j++)
        {
            unsigned int v1 = g1->_edges[j]->_v1;
            unsigned int v2 = g1->_edges[j]->_v2;
            if (g1->_adjMatrix[v1][v2] != g2->_adjMatrix[m[v1]][m[v2]])
            {
                mapping_invalid = TRUE;
            }
        }
        if (!mapping_invalid)
        {
            valid_mapping_found = TRUE;
            break;
        }
    }
    free_matrix(p,num_permutations);
    return valid_mapping_found;
}

Isomorphism_Class* init_iso_class(Graph* g)
{
    Isomorphism_Class* ic = (Isomorphism_Class*)malloc(sizeof(Isomorphism_Class));
    ic->_example = g;
    ic->_size = 2;
    ic->_graphs = (Graph**)malloc(sizeof(Graph*) * 2);
    ic->_graphs[0] = g;
    ic->_idx = 1;
    ic->_count = 1;
    ic->_next = NULL;
    ic->_prev = NULL;
    return ic;
}

Isomorphism_Class* add_iso_class(Isomorphism_Class* to, Isomorphism_Class* from)
{
    Isomorphism_Class* ret_val = from->_next; // the next value in the from linked list is returned
    Isomorphism_Class* t;
    Isomorphism_Class* p;
    int sorted = FALSE;
    for (t = to; t; t = t->_next)
    {
        p = t;
        if (smart_isomorphic(t->_example,from->_example))
        {
            t->_count += from->_count;
            free_iso_class(from,!(t->_example == from->_example));
            sorted = TRUE;
            break;
        }
    }
    if (!sorted)
    {
        p->_next = from;
        from->_prev = p;
        from->_next = NULL;
    }
        
    return ret_val;
}

void merge_isomorphism_class_counters(Isomorphism_Class* to, Isomorphism_Class* from)
{
    int counter = 0;
    while(from)
    {
        from = add_iso_class(to,from);
        counter++;
    }
    printf("%i Isomorphism classes were merged.\n",counter);
}

int add_to_iso_class(Graph* g, Isomorphism_Class* ic)
{
    if (smart_isomorphic(g,ic->_example))
    {
        ic->_graphs[ic->_idx] = g;
        ic->_idx++;
        ic->_count++;
        if (ic->_idx >= ic->_size)
        {
            unsigned int i;
            ic->_size *= 2;
            Graph** graphs = (Graph**)malloc(sizeof(Graph*) * ic->_size);
            for (i = 0; i < ic->_idx; i++) graphs[i] = ic->_graphs[i];
            free(ic->_graphs);
            ic->_graphs = graphs;
        }
        return TRUE;
    }
    else return FALSE;
}

int increment_iso_class(Graph* g, Isomorphism_Class* ic)
{
    if (smart_isomorphic(g,ic->_example))
    {
        ic->_count++;
        free_graph(g);
        return TRUE;
    }
    else return FALSE;
}

FSM_SearchState* init_search_state(Graph* g)
{
    unsigned int i;
    FSM_SearchState* init_s = (FSM_SearchState*)malloc(sizeof(FSM_SearchState));
    init_s->_size = g->_numEdges;
    init_s->_idx = 0;
    init_s->_numEdges = 0;
    init_s->_edges = (int*)malloc(sizeof(int) * init_s->_size);
    for (i = 0; i < init_s->_size; i++) init_s->_edges[i] = UNKNOWN;
    init_s->_next = NULL;
    init_s->_prev = NULL;
    return init_s;
}

FSM_SearchState** generate_initial_search_states(Graph* g, unsigned int numStates)
{
    // This function conducts a breadth first search to generate initial search
    // states which are meant to be used by individual threads to conduct a
    // parallel search.
    //
    // Note: For best performance generate X initial states where X is a power
    // of 2 and smaller than the number of edges in the given graph
    if (numStates > g->_numEdges)
    {
        printf("ERROR: Cannot generate more initial search states than there are edges in the graph.\n");
        exit(1);
    }
    if (!isPowerOfTwo(numStates))
    {
        printf("ERROR: the number of initial search states must be a power of 2\n");
        exit(1);
    }
    if (numStates == 0)
    {
        printf("ERROR: Cannot solve problem with 0 starting states.");
        exit(1);
    }
    
    unsigned int i = 0;
    unsigned int n = shift_log2(numStates);
    
    // create the array into which search states will be placed
    FSM_SearchState** initStates = (FSM_SearchState**)malloc(sizeof(FSM_SearchState*) * numStates);
    
    // create the initial search state
    FSM_SearchState* init_s = init_search_state(g);
    if (numStates == 1)
    {
        initStates[0] = init_s;
        printf("State: [");
        unsigned int j;
        for (j = 0; j < initStates[0]->_size; j++)
        {
            printf(" %i,",initStates[0]->_edges[j]);
        }
        printf("]\n");
        return initStates;
    }
    FSM_Search_Queue* queue = init_search_queue();
    enqueueState(queue,init_s);
    
    FSM_SearchState* c;
    while (i < numStates) // note that as long as the number of states is 
                          // smaller than or equal to the number of edges, the 
                          // queue cannot run out
    {
        c = dequeueState(queue);
        if (c != init_s && c->_idx == n) initStates[i++] = c;
        
        if (i < numStates)
        {
            FSM_SearchState** children = child_states(c); // generate the children of the state, and add them to the stack if there are any
            if (children)
            {
                enqueueState(queue,children[0]);
                enqueueState(queue,children[1]);
            }
            free(children);
        }
        
        if (c != init_s && c->_idx != n) free_state(c);
    }
    free_state(init_s);
    
    // free the states remaining in the queue
    while(!emptyQueue(queue))
    {
        c = dequeueState(queue);
        free_state(c);
    }
    free(queue);
    // return the resulting array of initial search states
    for(i = 0; i < numStates; i++)
    {
        printf("State: [");
        unsigned int j;
        for (j = 0; j < initStates[i]->_size; j++)
        {
            printf(" %i,",initStates[i]->_edges[j]);
        }
        printf("]\n");
    }
    return initStates;
}

void free_state(FSM_SearchState* s)
{
    free(s->_edges);
    free(s);
}

void free_iso_class(Isomorphism_Class* ic, int free_example_graph)
{
    unsigned int i;
    if (free_example_graph) free_graph(ic->_graphs[0]);
    for (i = 1; i < ic->_idx; i++)
    {
        free_graph(ic->_graphs[i]);
    }
    free(ic->_graphs);
    free(ic);
}

void free_iso_classes(Isomorphism_Class* ic)
{
    Isomorphism_Class* n;
    do {
        n = ic->_next;
        free_iso_class(ic,TRUE);
        ic = n;
    } while(ic);
}

int completely_labeled_state(FSM_SearchState* s)
{
    return s->_idx == s->_size;
}


FSM_SearchState** child_states(FSM_SearchState* s)
{
    if (s->_idx == s->_size) return NULL;
    
    FSM_SearchState** r = (FSM_SearchState**)malloc(sizeof(FSM_SearchState*) * 2);
    r[0] = (FSM_SearchState*)malloc(sizeof(FSM_SearchState));
    r[0]->_size = s->_size;
    r[0]->_idx = s->_idx;
    r[0]->_numEdges = s->_numEdges;
    r[0]->_edges = copy_array(s->_edges,r[0]->_size);
    r[0]->_next = NULL;
    r[0]->_prev = NULL;
    
    r[1] = (FSM_SearchState*)malloc(sizeof(FSM_SearchState));
    r[1]->_size = s->_size;
    r[1]->_idx = s->_idx;
    r[1]->_numEdges = s->_numEdges;
    r[1]->_edges = copy_array(s->_edges,r[1]->_size);
    r[1]->_next = NULL;
    r[1]->_prev = NULL;
    
    r[0]->_edges[r[0]->_idx] = TAKE_EDGE;
    r[0]->_idx++;
    r[0]->_numEdges++;
    r[1]->_edges[r[1]->_idx] = LEAVE_EDGE;
    r[1]->_idx++;
    
    return r;
}

Graph* graph_from_search_state(FSM_SearchState* s, Graph* parentGraph)
{
    if (s->_size != parentGraph->_numEdges)
    {
        printf("ERROR: Graph does not match search state to which it was applied!");
        exit(1);
    }
    Graph* g = init_graph(1,s->_numEdges);
    unsigned int i, v1IdxNew, v2IdxNew, v1IdxOld, v2IdxOld;
    int* old2NewMap = (int*)malloc(sizeof(int) * parentGraph->_numVertices);
    for (i = 0; i < parentGraph->_numVertices; i++) old2NewMap[i] = VERTEX_DOES_NOT_EXIST;
    for (i = 0; i < s->_idx; i++)
    {
        if (s->_edges[i] == TAKE_EDGE)
        {
            v1IdxOld = parentGraph->_edges[i]->_v1;
            v2IdxOld = parentGraph->_edges[i]->_v2;
            
            if (old2NewMap[v1IdxOld] == VERTEX_DOES_NOT_EXIST) old2NewMap[v1IdxOld] = add_vertex(parentGraph->_vertices[v1IdxOld]->_label,g);
            if (old2NewMap[v2IdxOld] == VERTEX_DOES_NOT_EXIST) old2NewMap[v2IdxOld] = add_vertex(parentGraph->_vertices[v2IdxOld]->_label,g);
            
            v1IdxNew = old2NewMap[v1IdxOld];
            v2IdxNew = old2NewMap[v2IdxOld];
            
            // connect the two vertices in the new graph
            connect_by_idx(v1IdxNew,v2IdxNew,g);
        }
    }
    free(old2NewMap);
    return g;
}

FSM_SearchStack* init_search_stack()
{
    FSM_SearchStack* s = (FSM_SearchStack*)malloc(sizeof(FSM_SearchStack));
    s->_size = 0;
    s->_top = NULL;
    s->_bottom = NULL;
    return s;
}

void pushState(FSM_SearchStack* s, FSM_SearchState* f)
{
    if (s->_size == 0)
    {
        s->_top = f;
        s->_bottom = f;
    }
    else
    {
        s->_top->_next = f;
        f->_prev = s->_top;
        s->_top = f;
    }
    s->_size++;
}

FSM_SearchState* popState(FSM_SearchStack* s)
{
    if (s->_size == 0)
    {
        return NULL;
    }
    else if (s->_size == 1)
    {
        FSM_SearchState* r = s->_top;
        s->_top = NULL;
        s->_bottom = NULL;
        s->_size--;
        r->_next = NULL;
        r->_prev = NULL;
        return r;
    }
    else
    {
        FSM_SearchState* r = s->_top;
        s->_top = r->_prev;
        s->_size--;
        r->_next = NULL;
        r->_prev = NULL;
        return r;
    }
}

void enqueueState(FSM_Search_Queue* q, FSM_SearchState* f)
{
    if (q->_count == 0)
    {
        q->_top = f;
        q->_bottom = f;
    }
    else
    {
        q->_top->_next = f;
        f->_prev = q->_top;
        q->_top = f;
    }
    q->_count++;
}

FSM_SearchState* dequeueState(FSM_Search_Queue* q)
{
    if (q->_count == 0)
    {
        return NULL;
    }
    else if (q->_count == 1)
    {
        FSM_SearchState* r = q->_top;
        q->_top = NULL;
        q->_bottom = NULL;
        q->_count--;
        r->_next = NULL;
        r->_prev = NULL;
        return r;
    }
    else
    {
        FSM_SearchState* r = q->_bottom;
        q->_bottom = r->_next;
        r->_next->_prev = NULL;
        q->_count--;
        r->_next = NULL;
        r->_prev = NULL;
        return r;
    }
}

FSM_Search_Queue* init_search_queue()
{
    FSM_Search_Queue* s = (FSM_Search_Queue*)malloc(sizeof(FSM_Search_Queue));
    s->_count = 0;
    s->_top = NULL;
    s->_bottom = NULL;
    return s;
}

Isomorphism_Class* basicSearch(Graph* g)
{
    // create the initial search state
    FSM_SearchState* init_s = init_search_state(g);
    return searchFromState(init_s,g);
}

void* searchWorker(void* arg)
{
    WorkerTParam* param = (WorkerTParam*)arg;
    
    param->_results = searchFromState(param->_startState, param->_graph);
    printf("Worker Thread Finished\n");
    
    return NULL;
}

Isomorphism_Class* searchFromState(FSM_SearchState* f, Graph* g)
{
    // create the first isomorphism class in the linked list
    Isomorphism_Class* ismc = init_iso_class(g);
    // create the search stack
    FSM_SearchStack* stack = init_search_stack();
    pushState(stack,f);
    
    FSM_SearchState* c;
    while (!emptyStack(stack)) // loop over the stack while searching each state
    {
        c = popState(stack);
        search_state(g,c,ismc);
        
        FSM_SearchState** children = child_states(c); // generate the children of the state, and add them to the stack if there are any
        if (children)
        {
            pushState(stack,children[0]);
            pushState(stack,children[1]);
        }
        
        free(children);
        free_state(c); // free the state after you're done with it
    }
    free(stack);
    return ismc;
}

Isomorphism_Class* parallelSearch(Graph* g, int numThreads)
{
    if (numThreads > g->_numEdges)
    {
        printf("ERROR: The number of threads is greater than the number of edges.\n");
        exit(1);
    }
    unsigned int i;
    // generate an initial search state for each thread
    FSM_SearchState** init_ss = generate_initial_search_states(g,numThreads);
    
    // allocate the array of resulting isomorphism classes
    Isomorphism_Class** ics = (Isomorphism_Class**)malloc(sizeof(Isomorphism_Class*) * numThreads);
    
    // execute the search algorithm for each state
    // for now its sequential
    pthread_t* tid = (pthread_t*)malloc(sizeof(pthread_t) * numThreads);
    WorkerTParam** params = (WorkerTParam**)malloc(sizeof(WorkerTParam*) * numThreads);
    WorkerTParam* param;
    for(i = 0; i < numThreads; i++)
    {
        // set up the worker thread parameters
        param = (WorkerTParam*)malloc(sizeof(WorkerTParam));
        param->_startState = init_ss[i];
        param->_graph = g;
        param->_results = NULL;
        params[i] = param;
	/* Create worker thread */
	pthread_create(tid + i,NULL,searchWorker,param);
    }
    free(init_ss);
    
    // combine the results
    for(i = 0; i < numThreads; i++)
    {
	pthread_join(tid[i], NULL);
        ics[i] = params[i]->_results;
        if (i > 0) merge_isomorphism_class_counters(ics[0],ics[i]);
        free(params[i]);
    }
    
    Isomorphism_Class* r = ics[0];
    // free memory
    free(ics);
    free(params);
    free(tid);
    // return the results
    return r;
}

void search_state(Graph* p, FSM_SearchState* f, Isomorphism_Class* ic)
{
    if (completely_labeled_state(f))
    {
        // generate the child graph from the search state
        Graph* child = graph_from_search_state(f,p);
        // loop over all isomorphism classes to find out which one it belongs to and place it there
        int sorted = FALSE;
        Isomorphism_Class* prev;
        do {
            if (increment_iso_class(child,ic))
            {
                sorted = TRUE;
                break;
            }
            prev = ic;
            ic = prev->_next;
        } while(ic);
        // if none can be found, create a new isomorphism class
        if (!sorted) {
            Isomorphism_Class* nismc = init_iso_class(child);
            prev->_next = nismc;
            nismc->_prev = prev;
        }
    }
}

int emptyStack(FSM_SearchStack* s)
{
    return s->_size == 0;
}

int emptyQueue(FSM_Search_Queue* q)
{
    return q->_count == 0;
}

Graph* init_graph(unsigned int sizeV, unsigned int sizeE) 
{
    unsigned int i;
    Graph* g = (Graph*) malloc(sizeof(Graph));
    g->_numEdges = 0;
    g->_numVertices = 0;
    g->_sizeV = sizeV;
    g->_sizeE = sizeE;
    g->_vertices = (Vertex**)malloc(sizeof(Vertex*) * g->_sizeV);
    g->_edges = (Edge**)malloc(sizeof(Edge*) * g->_sizeE);
    g->_adjMatrix = (unsigned int**)malloc(sizeof(unsigned int*) * g->_sizeV);

    for (i = 0; i < g->_sizeV; i++) 
        g->_adjMatrix[i] = (unsigned int*)calloc(sizeof(unsigned int), g->_sizeV);

    return g;
}

unsigned int add_vertex(const char* label, Graph* g)
{
    // create the new vertex
    Vertex* v = (Vertex*)malloc(sizeof(Vertex));
    v->_label = label;
    v->_idx = g->_numVertices;
    v->_numEdges = 0;
    
    // insert the vertex into the graph
    g->_vertices[g->_numVertices++] = v;
    
    if (g->_numVertices == g->_sizeV){
        // we need to double the size of the vertex vector and adjacency matrix
        g->_sizeV *= 2;
        
        // create a new vertex vector
        unsigned int i;
        Vertex** vertices = (Vertex**)malloc(sizeof(Vertex*) * g->_sizeV);
        for (i = 0; i < g->_numVertices; i++) vertices[i] = g->_vertices[i];
        free(g->_vertices);
        g->_vertices = vertices;
        
        // create a new adjacency matrix
        unsigned int** adjMatrix = (unsigned int**) malloc(sizeof(unsigned int*) * g->_sizeV);
        for (i = 0; i < g->_sizeV; i++)
        { // for each row of the matrix
            // create a new matrix row
            adjMatrix[i] = (unsigned int*) calloc(sizeof(unsigned int), g->_sizeV);
            // copy memory from the old row to the new one
            unsigned int j;
            for (j = 0; j < g->_numVertices; j++)
            {
                if (i < g->_numVertices)
                {
                    adjMatrix[i][j] = g->_adjMatrix[i][j];
                }
                else break;
            }
            if (i < g->_numVertices) free(g->_adjMatrix[i]);
        }
        // free the old matrix
        free(g->_adjMatrix);
        // set the new matrix
        g->_adjMatrix = adjMatrix;
    }
    
    // return the index value of the vertex that was inserted
    return v->_idx;
}

void connect_by_idx(unsigned int x, unsigned int y, Graph* g)
{
    if (x < g->_numVertices && y < g->_numVertices)
    {
        if (g->_numEdges == g->_sizeE)
        {
            g->_sizeE *= 2;
            
            // create a new edge vector
            unsigned int i;
            Edge** edges = (Edge**)malloc(sizeof(Edge*) * g->_sizeE);
            for (i = 0; i < g->_numEdges; i++) edges[i] = g->_edges[i];
            free(g->_edges);
            g->_edges = edges;
        }
        g->_adjMatrix[x][y] = 1;
        g->_adjMatrix[y][x] = 1;
        g->_vertices[x]->_numEdges++;
        g->_vertices[y]->_numEdges++;
        Edge* n_edge = (Edge*)malloc(sizeof(Edge));
        n_edge->_v1 = x;
        n_edge->_v2 = y;
        g->_edges[g->_numEdges++] = n_edge;
    }
}

int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

unsigned int* degree_sequence(Graph* g)
{
    // create the array
    unsigned int i;
    unsigned int* ds = (unsigned int*)malloc(sizeof(unsigned int) * g->_numVertices);
    for (i = 0; i < g->_numVertices; i++)
    {
        ds[i] = g->_vertices[i]->_numEdges;
    }
    // sort the array
    qsort(ds,g->_numVertices,sizeof(unsigned int),cmpfunc);
    // return the result
    return ds;
}

unsigned int* line_graph_degree_sequence(Graph* g)
{
    // create the array
    unsigned int* lgds = (unsigned int*)malloc(sizeof(unsigned int) * g->_numEdges);
    // populate the array
    unsigned int i;
    for (i = 0; i < g->_numEdges; i++)
    {
        Edge* e = g->_edges[i];
        lgds[i] = g->_vertices[e->_v1]->_numEdges + g->_vertices[e->_v2]->_numEdges - 2;
    }
    // sort the array
    qsort(lgds,g->_numEdges,sizeof(unsigned int),cmpfunc);
    // return the result
    return lgds;
}

int degree_seq_eq(unsigned int* ds1, unsigned int s1, unsigned int* ds2, unsigned int s2)
{
    unsigned int i;
    if (s1 != s2) return FALSE;
    for (i = 0; i < s1; i++)
        if (ds1[i] != ds2[i]) return FALSE;
    return TRUE;
}

void free_graph(Graph* g)
{
    unsigned int r,e;
    for (r = 0; r < g->_sizeV; r++)
    {
        if (r < g->_numVertices) free(g->_vertices[r]);
        free(g->_adjMatrix[r]);
    }
    for (e = 0; e < g->_numEdges; e++)
    {
        free(g->_edges[e]);
    }
    free(g->_edges);
    free(g->_vertices);
    free(g->_adjMatrix);
    free(g);
}

void print_graph(Graph* g)
{
    unsigned int i,r,c;
    printf("      ");
    for ( i = 0; i < g->_numVertices; i++)
        printf("| %s(%i) ", g->_vertices[i]->_label, g->_vertices[i]->_idx);
    printf("|\n");
    for (r = 0; r < g->_numVertices; r++)
    {
        printf(" %s(%i) ",g->_vertices[r]->_label,g->_vertices[r]->_idx);
        for (c = 0; c < g->_numVertices; c++)
            printf("|   %i  ", g->_adjMatrix[r][c]);
        printf("|\n");
    }
    printf("Vertices: ");
    for ( i = 0; i < g->_numVertices; i++)
        printf("%s(%i)(%i) ", g->_vertices[i]->_label, g->_vertices[i]->_idx, g->_vertices[i]->_numEdges);
    printf("\n");
    unsigned int* ds = degree_sequence(g);
    unsigned int* lgds = line_graph_degree_sequence(g);
    printf("Degree Sequence: [");
    for (i = 0; i < g->_numVertices; i++) printf(", %i",ds[i]);
    printf("]  Line Graph Degree Sequence: [");
    for (i = 0; i < g->_numEdges; i++) printf(", %i",lgds[i]);
    printf("]\n");
    printf("Edges: [");
    for (i = 0; i < g->_numEdges; i++) printf(", (%i,%i)",g->_edges[i]->_v1,g->_edges[i]->_v2);
    printf("]\n\n");
    free(ds);
    free(lgds);
}