
/*
*
* File:                     graph.c
* Author:                   Johanna Lindoff and Emma Rosenlind
* CS username:              id19jlf and id19erd
* Date:                     2020-03-19
* Version:                  1.00
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "dlist.h"
#include "graph.h"
#include "array_1d.h"

// ===========INTERNAL DATA TYPES============

typedef struct node {
	bool visited;
	char *node_name;
	dlist *node_neighbours;
} node;

typedef struct graph {
	array_1d *nodes;
	int node_amount;
} graph;

// ===========INTERNAL FUNCTION IMPLEMENTATIONS============

/**
* nodes_are_equal() - Check whether two nodes are equal.
* @n1: Pointer to node 1.
* @n2: Pointer to node 2.
*
* Returns: true if the nodes are considered equal, otherwise false.
*
*/
bool nodes_are_equal(const node *n1,const node *n2){
	return (strcmp(n1->node_name, n2->node_name) == 0);
}

/**
* graph_empty() - Create an empty graph.
* @max_nodes: The maximum number of nodes the graph can hold.
*
* Returns: A pointer to the new graph.
*/
graph *graph_empty(int max_nodes){

	graph *g = malloc(1 * sizeof(struct graph));

	g->nodes = array_1d_create(0, max_nodes, NULL);
	g->node_amount = 0;
	return g;
}

/**
* graph_is_empty() - Check if a graph is empty, i.e. has no nodes.
* @g: Graph to check.
*
* Returns: True if graph is empty, otherwise false.
*/
bool graph_is_empty(const graph *g){
	return (g->node_amount == 0);
}

/**
* graph_has_edges() - Check if a graph has any edges.
* @g: Graph to check.
*
* Returns: True if graph has any edges, otherwise false.
*/
bool graph_has_edges(const graph *g){

	int current_node = array_1d_low(g->nodes);

	// Iterate the array of nodes and check if the nodes has neighbours
	while (current_node < g->node_amount) {
		node *n = array_1d_inspect_value(g->nodes, current_node);

		// If a node has a neighbour, return true
		if (dlist_is_empty(n->node_neighbours)){
			return false;
		}
		current_node++;
	}
	return true;
}

/**
* graph_insert_node() - Inserts a node with the given name into the graph.
* @g: Graph to manipulate.
* @s: Node name.
*
* Creates a new node with a copy of the given name and puts it into
* the graph.
*
* Returns: The modified graph.
*/
graph *graph_insert_node(graph *g, const char *s){

	node *n = malloc(1 * sizeof(struct node));

	n->visited = false;
	n->node_name = malloc(strlen(s) + 1);

	int str_pos = 0;

	// Copy the node name
	while (str_pos < strlen(s) + 1) {
		n->node_name[str_pos] = s[str_pos];
		str_pos++;
	}

	// Create an empty list for the node-neighbours
	n->node_neighbours = dlist_empty(NULL);
	array_1d_set_value(g->nodes, n, g->node_amount);
	g->node_amount++;
	return g;
}

/**
* graph_find_node() - Find a node stored in the graph.
* @g: Graph to manipulate.
* @s: Node identifier, e.g. a char *.
*
* Returns: A pointer to the found node, or NULL.
*/
node *graph_find_node(const graph *g, const char *s){

	int current_node = array_1d_low(g->nodes);

	// Iterate the node array
	while (current_node < g->node_amount) {
		node *n = array_1d_inspect_value(g->nodes, current_node);

		// If the current node name is equal to the searched node then node are found
		if (strcmp(n->node_name, s) == 0) {
			return n;
		}
		current_node++;
	}

	// If no match is found return NULL.
	return NULL;
}

/**
* graph_node_is_seen() - Return the seen status for a node.
* @g: Graph storing the node.
* @n: Node in the graph to return seen status for.
*
* Returns: The seen status for the node.
*/
bool graph_node_is_seen(const graph *g, const node *n){

	if (n->visited) {
		return true;
	} else {
		return false;
	}
}

/**
* graph_node_set_seen() - Set the seen status for a node.
* @g: Graph storing the node.
* @n: Node in the graph to set seen status for.
* @s: Status to set.
*
* Returns: The modified graph.
*/
graph *graph_node_set_seen(graph *g, node *n, bool seen){

	n->visited = seen;
	return g;
}

/**
* graph_reset_seen() - Reset the seen status on all nodes in the graph.
* @g: Graph to modify.
*
* Returns: The modified graph.
*/
graph *graph_reset_seen(graph *g){

	int current_node = array_1d_low(g->nodes);

	// Iterate the node array and make all the nodes un-visited
	while (current_node < g->node_amount) {
		struct node *n = array_1d_inspect_value(g->nodes, current_node);
		n->visited = false;
		current_node++;
	}

	return g;
}

/**
* graph_insert_edge() - Insert an edge into the graph.
* @g: Graph to manipulate.
* @n1: Source node (pointer) for the edge.
* @n2: Destination node (pointer) for the edge.
*
* NOTE: Undefined unless both nodes are already in the graph.
*
* Returns: The modified graph.
*/
graph *graph_insert_edge(graph *g, node *n1, node *n2){

	// Insert a node in the neighbour-list that belongs to the other node.
	dlist_pos position = dlist_first(n1->node_neighbours);
	position = dlist_insert(n1->node_neighbours, n2, position);
	return g;
}


/**
* graph_delete_node() - Remove a node from the graph.
* @g: Graph to manipulate.
* @n: Node to remove from the graph.
*
* Returns: The modified graph.
*
* NOTE: Undefined if the node is not in the graph.
*/
graph *graph_delete_node(graph *g, node *n){
	return g;
}

/**
* graph_delete_edge() - Remove an edge from the graph.
* @g: Graph to manipulate.
* @n1: Source node (pointer) for the edge.
* @n2: Destination node (pointer) for the edge.
*
* Returns: The modified graph.
*
* NOTE: Undefined if the edge is not in the graph.
*/
graph *graph_delete_edge(graph *g, node *n1, node *n2){
	return g;
}

/**
* graph_choose_node() - Return an arbitrary node from the graph.
* @g: Graph to inspect.
*
* Returns: A pointer to an arbitrayry node.
*
* NOTE: The return value is undefined for an empty graph.
*/
node *graph_choose_node(const graph *g){
	return NULL;
}

/**
* graph_neighbours() - Return a list of neighbour nodes.
* @g: Graph to inspect.
* @n: Node to get neighbours for.
*
* Returns: A pointer to a list of nodes. Note: The list must be
* dlist_kill()-ed after use.
*/
dlist *graph_neighbours(const graph *g,const node *n){

	// Make an empty list
	dlist *dlist_new = dlist_empty(NULL);
	dlist_pos new_pos = dlist_first(dlist_new);
	dlist_pos neighbours_pos = dlist_first(n->node_neighbours);

	// Iterate the neighbour list and copy the node neighbours to the new list
	while(!dlist_is_end(n->node_neighbours, neighbours_pos)) {
		node *list_value = dlist_inspect(n->node_neighbours, neighbours_pos);
		new_pos = dlist_insert(dlist_new, list_value, new_pos);
		neighbours_pos = dlist_next(n->node_neighbours, neighbours_pos);
	}
	return dlist_new;
}

/**
* graph_kill() - Destroy a given graph.
* @g: Graph to destroy.
*
* Return all dynamic memory used by the graph.
*
* Returns: Nothing.
*/
void graph_kill(graph *g){

	int current_node = array_1d_low(g->nodes);

	while (current_node < g->node_amount) {
		node *node_kill = array_1d_inspect_value(g->nodes, current_node);

		if(array_1d_has_value(g->nodes, current_node)){
			free(node_kill->node_name);
			dlist_kill(node_kill->node_neighbours);
			free(node_kill);
		}
		current_node++;
	}

	array_1d_kill(g->nodes);
	free(g);
}
