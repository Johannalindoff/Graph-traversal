
/*
*
* File:                     is_connected.c
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
#include <errno.h>
#include <ctype.h>
#include <stdbool.h>

#include "array_1d.h"
#include "graph.h"
#include "dlist.h"
#include "queue.h"

/**
* Return position of first non-whitespace character or -1 if only
* white-space is found.
*/
int first_non_white_space(const char *s) {

  // Start at first char.
  int i = 0;

  // Advance until we hit EOL as long as we're loooking at white-space.
  while (s[i] && isspace(s[i])) {
    i++;
  }

  if (s[i]) {
    // Return position of found a non-white-space char.
    return i;
  } else {
    // Return fail.
    return -1;
  }
}

/**
* Return true if s only contains whitespace
*/
bool line_is_blank(const char *s) {
  // Line is blank if it only contained white-space chars.
  return first_non_white_space(s) < 0;
}

/**
* Return true if s is a comment line, i.e. first non-whitespc char is '#'
*/
bool line_is_comment(const char *s) {
  int i = first_non_white_space(s);
  return (i >= 0 && s[i] == '#');
}

/**
* Return true if s only contains300 alpanumerical symbols, otherwise false
*/
bool line_is_alphanumeric(char *s) {
  int i = 0;
  while(s[i]) {
    if(isalnum(s[i])) {
      return true;
    }
    i++;
  }
  return false;
}

/**
* build_graph() - Creates a graph with information from a given text document.
* @filename: Name of file.
*
* Creates a graph with information from the given file.
*
* Returns: The graph.
*/
graph *build_graph(const char *filename) {

  FILE *file;
  char str[300];
  char first_node[41];
  char second_node[41];
  int edge_amount = -1;
  int inserted_edges = 0;
  bool read_edges = false;
  graph *g;

  file = fopen(filename, "r");

  // Exit if the file isn't correct
  if (file == NULL) {
    fprintf(stdout, "Failed to open %s for reading.\n", filename);
    exit(EXIT_FAILURE);
  }

  // Build the graph as long as there is information left to read in the file
  while (fgets(str, sizeof(str), file) != NULL) {

    if(line_is_comment(str) || line_is_blank(str) || !line_is_alphanumeric(str)) {
      continue;
    }

    // When we haven't read any edges yet, scan the first number as
    // number of edges in the graph
    else if(line_is_alphanumeric(str) && !read_edges){
      sscanf(str, "%i", &edge_amount);
      g = graph_empty(2 * edge_amount);
      if(edge_amount == -1) {
        printf("There is no edges. \n");
        exit(EXIT_FAILURE);
      }
      read_edges = true;
    }

    // When we know the number of edges, insert the nodes and edges in the graph
    else if(line_is_alphanumeric(str) && read_edges){
      sscanf(str, "%40s %40s", first_node, second_node);
      g = graph_insert_node(g, first_node);
      g = graph_insert_node(g, second_node);
      node *n1 = graph_find_node(g, first_node);
      node *n2 = graph_find_node(g, second_node);
      g = graph_insert_edge(g, n1, n2);
      inserted_edges++;
    }
  }

  // If the number in the beginning of the file doesn't match the actual
  // number of edges, it's something wrong
  if(edge_amount != inserted_edges) {
    printf("Wrong number of edges!");
    exit(EXIT_FAILURE);
  }

  //Close files before exit.
  if (fclose(file)) {
    fprintf(stdout, "Failed to close %s: %s\n", filename, strerror(errno));
    exit(EXIT_FAILURE);
  }
  return g;
}


/**
* find_path() - Finds a path between two nodes.
* @g: Graph storing the nodes.
* @src: Source node.
* @dest: Destination node.
*
* Returns: True if there is a path between two nodes.
*/
bool find_path(graph *g,node *src,node *dest) {

  queue *q;
  dlist *neighbour_list;
  dlist_pos pos;
  node *current_node;
  node *neighbour_node;

  if (nodes_are_equal(src,dest)) {
    return true;
  }

  q = queue_empty(NULL);
  // Mark the source node as visited
  g = graph_node_set_seen(g, src, true);
  // Insert the source node into a queue
  q = queue_enqueue(q, src);

  // Look for a path as long as the queue isn't empty
  while(!queue_is_empty(q)) {

    // Take out the next node of the queue
    current_node = queue_front(q);
    q = queue_dequeue(q);
    neighbour_list = graph_neighbours(g, current_node);
    pos = dlist_first(neighbour_list);

    // Iterates through the nodes neighbours
    while(!dlist_is_end(neighbour_list, pos)) {
      neighbour_node = dlist_inspect(neighbour_list, pos);

      // If neighbour-node is unseen, set to seen and add to queue
      if(!graph_node_is_seen(g, neighbour_node)) {
        g = graph_node_set_seen(g, neighbour_node, true);
        q = queue_enqueue(q, neighbour_node);
      }

      // If neighbour-node has the same name as the destination node, we have
      // found a path.
      if(nodes_are_equal(neighbour_node, dest)) {
        queue_kill(q);
        dlist_kill(neighbour_list);
        g = graph_reset_seen(g);
        return true;
      }

      pos = dlist_next(neighbour_list, pos);
    }
    dlist_kill(neighbour_list);
  }
  queue_kill(q);
  g = graph_reset_seen(g);
  return false;
}

// ===========MAIN FUNCTION============

int main (int parameters, char const *file_name[]) {

  char buffert[300];
  char src[41];
  char dest[41];
  node *src_node;
  node *dest_node;
  int number_of_parameters = 0;

  // Build the graph from the information in the text-file
  graph *graph = build_graph(file_name[1]);

  if(parameters != 2){
    fprintf(stdout, "Wrong amount of parameters.\n");
    exit(EXIT_FAILURE);
  }

  // Run the program as long as user doen't type "quit"
  do {

    // The user writes two nodes to compare or "quit"
    printf("Enter origin and destination (quit to exit): ");
    fgets(buffert,300,stdin);
    number_of_parameters = sscanf(buffert, "%40s %40s ", src, dest);

    // If the user doesn't want to quit
    if (strncmp(buffert, "quit", 4) != 0){

      // If the user enters correct amount of nodes
      if(number_of_parameters == 2) {

        src_node = graph_find_node(graph, src);
        dest_node = graph_find_node(graph, dest);

        // If the user enters unvalid node names
        if(src_node == NULL || dest_node == NULL){
          fprintf(stdout,"Destinations do not exist, please try again. \n\n");

        }
        // If there is a path between the two nodes
        else if(find_path(graph, src_node, dest_node)){
          fprintf(stdout,"There is a path from %s to %s.\n\n", src, dest);
        }
        // If the path doesn't exist
        else{
          fprintf(stdout,"There is no path from %s to %s.\n\n", src, dest);
        }
      }
      else {
        fprintf(stdout,"Please enter two destinations \n\n");
      }
    }
    // If the user wants to quit
    else {
      fprintf(stdout,"Normal exit.\n");
    }
    // Continue if the user doesn't want to quit
  } while(strncmp(buffert, "quit", 4) != 0);

  graph_kill(graph);

  return 0;
}
