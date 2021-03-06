#include <stdio.h>
#include <stdlib.h>

#include "tuilib.h"
#include "structures_interface.h"
#include "structures.h"
#include "time.h"
#include "stdio.h"

#define N_OPS 10

int main() {
	time_t t;
   	srand((unsigned) time(&t));
	int save_args[1] = {1};
	int *print_args = NULL;
	int delete_vertex_args[1] = {1};
	int delete_edge_args[2] = {1, 1};
	int add_edge_args[2] = {1, 1};
	int add_vertex_args[1] = {1};
	int load_args[1] = {1};
	int path_args[2] = {1, 1};

	struct Op save_op = {"save", 1, save_args, &tuilib_graph_save, 0, "Saves graph to a file"};
	struct Op print_op = {"print", 0, print_args, &tuilib_graph_print, 0, "Prints graph to a stdout"};
	struct Op delete_vertex_op = {"delete_vertex", 1, delete_vertex_args, &tuilib_graph_delete_vertex, 0, "Deletes a vertex by name"};
	struct Op delete_edge_op = {"delete_edge", 2, delete_edge_args, &tuilib_graph_delete_edge, 0, "Deletes edge by two vertecies"};
	struct Op add_edge_op = {"add_edge", 2, add_edge_args, &tuilib_graph_add_edge, 0, "Deletes edge by two vertecies"};
	struct Op add_vertex_op = {"add_vertex", 1, add_vertex_args, &tuilib_graph_add_vertex, 0, "Adds vertex by name"};
	struct Op vertex_list_op = {"vertex_list", 0, NULL, &tuilib_graph_vertex_list, 0, "Prints all the vertecies of the graph"};
	struct Op load_op = {"load", 1, load_args, &tuilib_graph_load, 0, "Loads graph from file, effectively overwriting existing graph"};
	struct Op generate_op = {"gen", 0, NULL, &tuilib_graph_generate, 0, "Generates random graph, overwriting existing"};
	struct Op path_op = {"path", 2, path_args, &tuilib_graph_path, 0, "Finds shortest path between two vertecies"};

	struct Op operations[N_OPS] = {save_op, print_op, delete_vertex_op, delete_edge_op, add_edge_op, add_vertex_op, vertex_list_op, load_op, generate_op, path_op};
	struct Graph *graph = graph_create(NULL);
	void* structure = (void*)graph;
	int flag = 1;

	while (flag) {
		flag = parse_input(structure, operations, N_OPS);

	}
	graph_free(graph);
	return 0;
}
