#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

void test_parallel_search()
{
    printf("Testing subgraph mining...\n");
    Graph* G = init_graph(1,1);
    unsigned int a = add_vertex("A",G);
    unsigned int b = add_vertex("B",G);
    unsigned int c = add_vertex("C",G);
    unsigned int d = add_vertex("D",G);
    unsigned int e = add_vertex("E",G);
    unsigned int f = add_vertex("F",G);
    unsigned int g = add_vertex("G",G);
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
    
//    print_graph(G);
    
    Isomorphism_Class* r = parallelSearch(G,8);
    Isomorphism_Class* s = r;
    int class_count = 0;
    int graph_count = 0;
    do {
        printf("Isomorphism Class of size %i\n",r->_count);
        // uncomment this line to see the example of the graphs inside this isomorphism class
//        print_graph(r->_example);
        class_count++;
        graph_count += r->_count;
        r = r->_next;
    } while(r);
    printf("%i Isomorphism Classes total.\n", class_count);
    printf("%i Subgraphs total.\n", graph_count);
    free_iso_classes(s);
}

void test_search()
{
    printf("Testing subgraph mining...\n");
    Graph* G = init_graph(1,1);
    unsigned int a = add_vertex("A",G);
    unsigned int b = add_vertex("B",G);
    unsigned int c = add_vertex("C",G);
    unsigned int d = add_vertex("D",G);
    unsigned int e = add_vertex("E",G);
    unsigned int f = add_vertex("F",G);
//    unsigned int g = add_vertex("G",G);
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
//    connect_by_idx(e,g,G);
    
//    print_graph(G);
    
    Isomorphism_Class* r = basicSearch(G);
    Isomorphism_Class* s = r;
    do {
        printf("Isomorphism Class of size %i\n",r->_count);
//        printf("Isomorphism Class of size %i defined by: \n",r->_count);
//        print_graph(r->_example);
//        printf("\n");
        r = r->_next;
    } while(r);
    free_iso_classes(s);
}

void test_subgraph_generation()
{
    printf("Testing subgraph generation...\n");
    Graph* g = init_graph(4,4);
    unsigned int a = add_vertex("A",g);
    unsigned int b = add_vertex("B",g);
    unsigned int c = add_vertex("C",g);
    unsigned int d = add_vertex("D",g);
    connect_by_idx(a,b,g);
    connect_by_idx(a,c,g);
    connect_by_idx(c,b,g);
    connect_by_idx(c,d,g);
    
    printf("Parent Graph:\n");
    print_graph(g);
    
    FSM_SearchState* r = (FSM_SearchState*)malloc(sizeof(FSM_SearchState));
    r->_size = 4;
    r->_idx = 4;
    r->_numEdges = 2;
    r->_edges = (int*)malloc(sizeof(int) * r->_size);
    r->_edges[0] = TAKE_EDGE;
    r->_edges[1] = TAKE_EDGE;
    r->_edges[2] = LEAVE_EDGE;
    r->_edges[3] = LEAVE_EDGE;
    r->_next = NULL;
    r->_prev = NULL;
    
    Graph* cg = graph_from_search_state(r,g);
    printf("Child Graph:\n");
    print_graph(cg);
}

void test_isomorphism()
{
    printf("Testing isomorphism...\n");
    Graph* g1 = init_graph(4,4);
    unsigned int a = add_vertex("A",g1);
    unsigned int b = add_vertex("B",g1);
    unsigned int c = add_vertex("C",g1);
    unsigned int d = add_vertex("D",g1);
    connect_by_idx(a,b,g1);
    connect_by_idx(a,c,g1);
    connect_by_idx(c,b,g1);
    connect_by_idx(c,d,g1);
    
    Graph* g2 = init_graph(4,4);
    unsigned int e = add_vertex("E",g2);
    unsigned int f = add_vertex("F",g2);
    unsigned int g = add_vertex("G",g2);
    unsigned int h = add_vertex("H",g2);
    connect_by_idx(e,f,g2);
    connect_by_idx(e,g,g2);
    connect_by_idx(f,h,g2);
    connect_by_idx(g,h,g2);
    
    printf("Comparing: \n");
    print_graph(g1);
    print_graph(g2);
    int i = isomorphic(g1,g2);
    if (i) printf("They are isomorphic!\n");
    else printf("They are not isomorphic!\n");
    free_graph(g1);
    free_graph(g2);
    
    printf("Testing isomorphism...\n");
    g1 = init_graph(4,4);
    a = add_vertex("A",g1);
    b = add_vertex("B",g1);
    c = add_vertex("C",g1);
    d = add_vertex("D",g1);
    connect_by_idx(a,b,g1);
    connect_by_idx(a,c,g1);
    connect_by_idx(c,b,g1);
    connect_by_idx(c,d,g1);
    
    g2 = init_graph(4,4);
    e = add_vertex("E",g2);
    f = add_vertex("F",g2);
    g = add_vertex("G",g2);
    h = add_vertex("H",g2);
    connect_by_idx(e,f,g2);
    connect_by_idx(e,h,g2);
    connect_by_idx(f,h,g2);
    connect_by_idx(g,h,g2);
    
    printf("Comparing: \n");
    print_graph(g1);
    print_graph(g2);
    i = isomorphic(g1,g2);
    if (i) printf("They are isomorphic!\n");
    else printf("They are not isomorphic!\n");
    free_graph(g1);
    free_graph(g2);
}

void test_smart_isomorphism()
{
    printf("Testing smart isomorphism...\n");
    Graph* g1 = init_graph(4,4);
    unsigned int a = add_vertex("A",g1);
    unsigned int b = add_vertex("B",g1);
    unsigned int c = add_vertex("C",g1);
    unsigned int d = add_vertex("D",g1);
    connect_by_idx(a,b,g1);
    connect_by_idx(a,c,g1);
    connect_by_idx(c,b,g1);
    connect_by_idx(c,d,g1);
    
    Graph* g2 = init_graph(4,4);
    unsigned int e = add_vertex("E",g2);
    unsigned int f = add_vertex("F",g2);
    unsigned int g = add_vertex("G",g2);
    unsigned int h = add_vertex("H",g2);
    connect_by_idx(e,f,g2);
    connect_by_idx(e,g,g2);
    connect_by_idx(f,h,g2);
    connect_by_idx(g,h,g2);
    
    printf("Comparing: \n");
    print_graph(g1);
    print_graph(g2);
    int i = smart_isomorphic(g1,g2);
    if (i) printf("They are isomorphic!\n");
    else printf("They are not isomorphic!\n");
    free_graph(g1);
    free_graph(g2);
    
    printf("Testing isomorphism...\n");
    g1 = init_graph(4,4);
    a = add_vertex("A",g1);
    b = add_vertex("B",g1);
    c = add_vertex("C",g1);
    d = add_vertex("D",g1);
    connect_by_idx(a,b,g1);
    connect_by_idx(a,c,g1);
    connect_by_idx(c,b,g1);
    connect_by_idx(c,d,g1);
    
    g2 = init_graph(4,4);
    e = add_vertex("E",g2);
    f = add_vertex("F",g2);
    g = add_vertex("G",g2);
    h = add_vertex("H",g2);
    connect_by_idx(e,f,g2);
    connect_by_idx(e,h,g2);
    connect_by_idx(f,h,g2);
    connect_by_idx(g,h,g2);
    
    printf("Comparing: \n");
    print_graph(g1);
    print_graph(g2);
    i = smart_isomorphic(g1,g2);
    if (i) printf("They are isomorphic!\n");
    else printf("They are not isomorphic!\n");
    free_graph(g1);
    free_graph(g2);
}

void test_permutations()
{
    printf("Testing permutations...\n");
    unsigned int array[] = {1,2,3,4,5};
    unsigned int** p = permutations(array,5);
    print_matrix(p,factorial(5),5);
    free_matrix(p,factorial(5));
}

void test_adding_vertices()
{
    printf("Testing graph generation...\n");
    
    Graph* g = init_graph(2,2);
    
    unsigned int a = add_vertex("A",g);
    printf("Vertex %s inserted at index %i\n",g->_vertices[a]->_label,a);
    unsigned int b = add_vertex("B",g);
    printf("Vertex %s inserted at index %i\n",g->_vertices[b]->_label,b);
    connect_by_idx(a,b,g);
    print_graph(g);
    
    unsigned int c = add_vertex("C",g);
    printf("Vertex %s inserted at index %i\n",g->_vertices[c]->_label,c);
    connect_by_idx(a,c,g);
    connect_by_idx(b,c,g);
    print_graph(g);
    
    unsigned int d = add_vertex("D",g);
    printf("Vertex %s inserted at index %i\n",g->_vertices[d]->_label,d);
    connect_by_idx(c,d,g);
    print_graph(g);
    
    unsigned int e = add_vertex("E",g);
    printf("Vertex %s inserted at index %i\n",g->_vertices[e]->_label,e);
    connect_by_idx(a,e,g);
    connect_by_idx(c,e,g);
    print_graph(g);
    
    unsigned int f = add_vertex("F",g);
    printf("Vertex %s inserted at index %i\n",g->_vertices[f]->_label,f);
    connect_by_idx(c,f,g);
    connect_by_idx(a,f,g);
    connect_by_idx(b,f,g);
    print_graph(g);
    
    unsigned int v = add_vertex("G",g);
    printf("Vertex %s inserted at index %i\n",g->_vertices[v]->_label,v);
    connect_by_idx(f,v,g);
    print_graph(g);
    
    unsigned int h = add_vertex("H",g);
    printf("Vertex %s inserted at index %i\n",g->_vertices[h]->_label,h);
    connect_by_idx(v,h,g);
    connect_by_idx(c,h,g);
    print_graph(g);
    
    free_graph(g);
}

int main(int argc, char** argv) {
//    test_adding_vertices();
//    test_permutations();
//    test_isomorphism();
//    test_smart_isomorphism();
//    test_subgraph_generation();
//    test_search();
    test_parallel_search();
    return (EXIT_SUCCESS);
}
