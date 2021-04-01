#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "structures.h"
#include "tuilib.h"

// arguments: char* vertex1, char* vertex2
void tuilib_graph_add_vertex(void **callback_data, void *main_structure) {
	struct Graph *graph = (struct Graph*)main_structure;
	char *vertex_1 = (char*)(callback_data[0]);
	print_debug("tuilib vertex_1 contents: %s", vertex_1);
	char *data_copy = strnew(vertex_1);
	print_debug("tuilib copy contents: %s", data_copy);
	// TODO: status logic
	uint8_t status = graph_add_vertex(graph, data_copy);
	if (status != 2 && status != 0) {
		msg_warn("Some error occured during insertion (NOT same key)");
	}
}

void tuilib_graph_add_edge(void **callback_data, void *main_structure) {
	struct Graph *graph = (struct Graph*)main_structure;
	char *vertex_1 = (char*)(callback_data[0]);
	char *vertex_2 = (char*)(callback_data[1]);
	char *data_copy_1 = malloc(sizeof(char)*(strlen(vertex_1)+1));
	strcpy(data_copy_1, vertex_1);
	char *data_copy_2 = malloc(sizeof(char)*(strlen(vertex_2)+1));
	strcpy(data_copy_2, vertex_2);
	
	// TODO: status logic
	uint8_t status = graph_add_edge(graph, data_copy_1, data_copy_2);
}

void tuilib_graph_delete_edge(void **callback_data, void *main_structure) {
	struct Graph *graph = (struct Graph*)main_structure;
	char *vertex_1 = (char*)(callback_data[0]);
	char *vertex_2 = (char*)(callback_data[1]);
	char *data_copy_1 = malloc(sizeof(char)*(strlen(vertex_1)+1));
	strcpy(data_copy_1, vertex_1);
	char *data_copy_2 = malloc(sizeof(char)*(strlen(vertex_2)+1));
	strcpy(data_copy_2, vertex_2);
	
	// TODO: status logic
	uint8_t status = graph_delete_edge(graph, data_copy_1, data_copy_2);
}

void tuilib_graph_delete_vertex(void **callback_data, void *main_structure) {
	struct Graph *graph = (struct Graph*)main_structure;
	char *vertex_1 = (char*)(callback_data[0]);	
	char *data_copy_1 = malloc(sizeof(char)*(strlen(vertex_1)+1));
	strcpy(data_copy_1, vertex_1);
	// TODO: status logic
	uint8_t status = graph_delete_vertex(graph, data_copy_1);
}

void tuilib_graph_print(void **callback_data, void *main_structure) {
	struct Graph *graph = (struct Graph*)main_structure;
	graph_output(graph, NULL);
}

void tuilib_graph_save(void **callback_data, void *main_structure) {
	struct Graph *graph = (struct Graph*)main_structure;
	char *filename = (char*)(callback_data[0]);
	uint8_t status = graph_output(graph, filename);
	if (status != 0) {
		msg_error("Some error occured when writing to file");
	}
}

void tuilib_graph_vertex_list(void **callback_data, void *main_structure) {
	struct Graph *graph = (struct Graph*)main_structure;
	vector_print(graph->vertex_list);
}

void tuilib_graph_load(void **callback_data, void *main_structure) {
	struct Graph *graph = (struct Graph*)main_structure;
	char *filename = (char*)(callback_data[0]);
	struct Graph *new_graph = graph_load(filename);
	if (NULL == new_graph) {
		msg_warn("Error loading graph from file!");
	} else {
		hashtable_free(graph->adj_list);
		vector_free(graph->vertex_list);
		graph->vertex_list = new_graph->vertex_list;
		graph->adj_list = new_graph->adj_list;
		free_z(new_graph);
	}
}
