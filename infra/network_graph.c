#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dll.h"
#include "network_graph.h"
#include "logger.h"
#include "communication.h"


void
insert_link_between_two_nodes(node_t *node1,
        node_t *node2,
        char *from_if_name,
        char *to_if_name,
        unsigned int cost){

    link_t *link = calloc(1, sizeof(link_t));

    /*Set interface properties*/
    strncpy(link->intf1.if_name, from_if_name, IF_NAME_SIZE);
    link->intf1.if_name[IF_NAME_SIZE - 1] = '\0';
    strncpy(link->intf2.if_name, to_if_name, IF_NAME_SIZE);
    link->intf2.if_name[IF_NAME_SIZE - 1] = '\0';
    
    link->intf1.link= link; /*set back pointer to link*/
    link->intf2.link= link; /*set back pointer to link*/

    link->intf1.att_node = node1;
    link->intf2.att_node = node2;
    link->cost = cost;

    int empty_intf_slot;

    /*Plugin interface ends into Node*/
    empty_intf_slot = get_node_intf_available_slot(node1);
    node1->intf[empty_intf_slot] = &link->intf1;

    empty_intf_slot = get_node_intf_available_slot(node2);
    node2->intf[empty_intf_slot] = &link->intf2;

    /*Now Assign Random generated Mac address to the Interfaces*/
    interface_assign_mac_address(&link->intf1);                                                                                                                                                          
    interface_assign_mac_address(&link->intf2);

}

graph_t *
create_new_graph(char *topology_name){

    graph_t *graph = calloc(1, sizeof(graph_t));
    strncpy(graph->topology_name, topology_name, 32);
    graph->topology_name[31] = '\0';

    dll_init(&graph->dll_unit_list);
    return graph;
}

node_t *
create_graph_node(graph_t *graph, char *node_name){

    node_t *node = calloc(1, sizeof(node_t));
    strncpy(node->node_name, node_name, NODE_NAME_SIZE);
    node->node_name[NODE_NAME_SIZE - 1] = '\0';
    
    init_udp_socket(node);
    init_node_nw_prop(&node->node_nw_prop);
    dll_init(&node->dll_unit);
    dll_insert_after(&graph->dll_unit_list, &node->dll_unit);
    return node;
}

void dump_graph(graph_t *graph){
    node_t *node = NULL;
	dll_t *curr = NULL;

    printf("========================================\n");
    printf("  Topology Name = %s\n", graph->topology_name);
    printf("========================================\n");

	dll_traverse(&graph->dll_unit_list, curr){
		node = dll_to_node(curr);
		dump_node(node);
	}

    printf("========================================\n");
}

void dump_node(node_t *node){
    unsigned int i = 0;
    interface_t *intf;

    printf("  Node Name = %s :\n", node->node_name);
    printf("  UDP Port Number = %d :\n", node->udp_port_number);
    printf("  UDP Socket FD = %d :\n", node->udp_sock_fd);
    printf("  --------------------------------------\n");

    for( ; i < MAX_INTF_PER_NODE; i++){
        intf = node->intf[i];
        if(!intf) break;
        dump_interface(intf);
    }
    printf("  --------------------------------------\n");
}

void dump_interface(interface_t *interface){
    link_t *link = interface->link;
    node_t *nbr_node = get_nbr_node(interface);

    printf("    |-> Local Node : %s\n", interface->att_node->node_name);
    printf("        Interface Name = %s\n", interface->if_name);
    printf("        Nbr Node %s, cost = %u\n", nbr_node->node_name, link->cost); 
    // printf("    ----------------------------------\n");
}
