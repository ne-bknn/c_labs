#include <inttypes.h>

#include "structures.h"

// arguments: char* vertex1, char* vertex2
void tuilib_graph_add_vertex(void **callback_data, void *main_structure) {
	struct Graph *graph = (struct Graph*)main_structure;
	char *vertex_1 = (char*)(callback_data[0]);	
	// TODO: status logic
	uint8_t status = graph_add_vertex(graph, vertex_1);
}

void tuilib_graph_add_edge(void **callback_data, void *main_structure) {
	struct Graph *graph = (struct Graph*)main_structure;
	char *vertex_1 = (char*)(callback_data[0]);
	char *vertex_2 = (char*)(callback_data[1]);
	// TODO: status logic
	uint8_t status = graph_add_edge(graph, vertex_1, vertex_2);
}

void tuilib_graph_delete_edge(void **callback_data, void *main_structure) {
	struct Graph *graph = (struct Graph*)main_structure;
	char *vertex_1 = (char*)(callback_data[0]);
	char *vertex_2 = (char*)(callback_data[1]);
	// TODO: status logic
	uint8_t status = graph_delete_edge(graph, vertex_1, vertex_2);
}

void tuilib_graph_delete_vertex(void **callback_data, void *main_structure) {
	struct Graph *graph = (struct Graph*)main_structure;
	char *vertex_1 = (char*)(callback_data[0]);	
	// TODO: status logic
	uint8_t status = graph_delete_vertex(graph, vertex_1);
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
