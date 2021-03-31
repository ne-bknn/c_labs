#include <stdio.h>

#include "tuilib.h"
#include "structures_interface.h"
#include "structures.h"

#define N_OPS 7

int main() {
	int save_args[1] = {1};
	int *print_args = NULL;
	int delete_vertex_args[1] = {1};
	int delete_edge_args[2] = {1, 1};
	int add_edge_args[2] = {1, 1};
	int add_vertex_args[1] = {1};

	struct Op save_op = {"save", 1, save_args, &tuilib_graph_save, 0, "Saves graph to a file"};
	struct Op print_op = {"print", 0, print_args, &tuilib_graph_print, 0, "Prints graph to a stdout"};
	struct Op delete_vertex_op = {"delete_vertex", 1, delete_vertex_args, &tuilib_graph_delete_vertex, 0, "Deletes a vertex by name"};
	struct Op delete_edge_op = {"delete_edge", 2, delete_edge_args, &tuilib_graph_delete_edge, 0, "Deletes edge by two vertecies"};
	struct Op add_edge_op = {"add_edge", 2, add_edge_args, &tuilib_graph_add_edge, 0, "Deletes edge by two vertecies"};
	struct Op add_vertex_op = {"add_vertex", 1, add_vertex_args, &tuilib_graph_add_vertex, 0, "Adds vertex by name"};
	struct Op vertex_list_op = {"vertex_list", 0, NULL, &tuilib_graph_vertex_list, 0, "Prints all the vertecies of the graph"};

	struct Op operations[N_OPS] = {save_op, print_op, delete_vertex_op, delete_edge_op, add_edge_op, add_vertex_op, vertex_list_op};
	struct Graph *graph = graph_create(NULL);
	void* structure = (void*)graph;
	int flag = 1;

	while (flag) {
		flag = parse_input(structure, operations, N_OPS);

	}
	graph_free(graph);
	return 0;
}
