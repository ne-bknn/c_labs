#ifndef TUILIB_STRUCTURES_INTERFACE_H
#define TUILIB_STRUCTURES_INTERFACE_H

void tuilib_graph_save(void **callback_data,void *main_structure);
void tuilib_graph_print(void **callback_data,void *main_structure);
void tuilib_graph_delete_vertex(void **callback_data,void *main_structure);
void tuilib_graph_delete_edge(void **callback_data,void *main_structure);
void tuilib_graph_add_edge(void **callback_data,void *main_structure);
void tuilib_graph_add_vertex(void **callback_data,void *main_structure);
void tuilib_graph_vertex_list(void **callback_data,void *main_structure);
void tuilib_graph_load(void **callback_data,void *main_structure);

#endif
