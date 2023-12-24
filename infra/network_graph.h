#ifndef __NW_GRAPH__
#define __NW_GRAPH__

#include <stdio.h>
#include <assert.h>
#include "dll.h"
#include "network_prop.h"


#define NODE_NAME_SIZE   128
#define IF_NAME_SIZE     16
#define MAX_INTF_PER_NODE   48

/*Forward Declarations*/
typedef struct node_ node_t;
typedef struct link_ link_t;


typedef struct interface_ {
    char if_name[IF_NAME_SIZE];
    struct node_ *att_node;
    struct link_ *link;
    intf_nw_props_t intf_nw_props;
} interface_t;

struct link_ {
    interface_t intf1;
    interface_t intf2;
    unsigned int cost;
}; 

struct node_ {

    char node_name[NODE_NAME_SIZE];
    interface_t *intf[MAX_INTF_PER_NODE];
    dll_t dll_unit;
    node_nw_prop_t node_nw_prop;
};

typedef struct graph_{

    char topology_name[32];
    dll_t dll_unit_list; 
} graph_t;

node_t *
create_graph_node(graph_t *graph, char *node_name);

graph_t *
create_new_graph(char *topology_name);

void
insert_link_between_two_nodes(node_t *node1, 
                             node_t *node2,
                             char *from_if_name, 
                             char *to_if_name, 
                             unsigned int cost);

/*Helper functions*/
static inline node_t *
get_nbr_node(interface_t *interface){

    assert(interface->att_node);
    assert(interface->link);
    
    link_t *link = interface->link;
    if(&link->intf1 == interface)
        return link->intf2.att_node;
    else
        return link->intf1.att_node;
}

static inline int
get_node_intf_available_slot(node_t *node){

    int i ;
    for( i = 0 ; i < MAX_INTF_PER_NODE; i++){
        if(node->intf[i])
            continue;
        return i;
    }
    return -1;
}

static inline interface_t *
get_node_if_by_name(node_t *node, char *if_name){

    int i ;
    interface_t *intf;

    for( i = 0 ; i < MAX_INTF_PER_NODE; i++){
        intf = node->intf[i];
        if(!intf) return NULL;
        if(strncmp(intf->if_name, if_name, IF_NAME_SIZE) == 0){
            return intf;
        }
    }
    return NULL;
}

static inline node_t *
get_node_by_node_name(graph_t *topo, char *node_name){
    node_t *node;
    dll_traverse_entry(node, &topo->dll_unit_list, dll_unit, node_t)
    {
        if (strncmp(node->node_name, node_name, strlen(node_name)) == 0)
            return node;
    }
    return NULL;
}



/*Display Routines*/
void dump_graph(graph_t *graph);
void dump_node(node_t *node);
void dump_interface(interface_t *interface);

graph_t *build_first_topo();
#endif /* __NW_GRAPH_ */
