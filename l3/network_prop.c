#include "network_graph.h"
#include "utils.h"
#include <memory.h>
#include <stdio.h>

/*Random number generator*/
static unsigned int 
hash_code(void *ptr, unsigned int size){
    unsigned int value=0, i =0; 
    char *str = (char*)ptr;
    while(i < size)
    {   
        value += *str;
        value*=97;
        str++;
        i++;
    }   
    return value;
}


/*Assign a unique mac address to interface*/
void
interface_assign_mac_address(interface_t *interface){

    node_t *node = interface->att_node;
        
    if(!node)
        return;

    unsigned int hash_code_val = 0;
    hash_code_val = hash_code(node->node_name, NODE_NAME_SIZE);
    hash_code_val *= hash_code(interface->if_name, IF_NAME_SIZE);
    memset(IF_MAC(interface), 0, sizeof(IF_MAC(interface)));
    memcpy(IF_MAC(interface), (char *)&hash_code_val, sizeof(unsigned int));
}    

bool_t node_set_device_type(node_t *node, unsigned int F){

    SET_BIT(node->node_nw_prop.flags, F);
    return TRUE;
}

bool_t node_set_loopback_address(node_t *node, char *ip_addr){

    assert(ip_addr);

    node->node_nw_prop.is_lb_addr_config = TRUE;
    strncpy(NODE_LO_ADDR(node), ip_addr, 16);
    NODE_LO_ADDR(node)[15] = '\0';
    
    return TRUE;
}

bool_t node_set_intf_ip_address(node_t *node, char *local_if, 
                                char *ip_addr, char mask) {

    interface_t *interface = get_node_if_by_name(node, local_if);
    if(!interface) assert(0);

    strncpy(IF_IP(interface), ip_addr, 16);
    IF_IP(interface)[15] = '\0';
    interface->intf_nw_props.mask = mask; 
    interface->intf_nw_props.is_ipadd_config = TRUE;
    return TRUE;
}

bool_t node_unset_intf_ip_address(node_t *node, char *local_if){

    return TRUE;
}

void dump_node_nw_props(node_t *node){
    printf("\n");
    printf("  ========================================\n");
    printf("  Node Name = %s :\n\n", node->node_name);
    printf("  UDP Port Number = %d \n", node->udp_port_number);
    printf("  UDP Socket FD = %d \n", node->udp_sock_fd);
    printf("  --------------------------------------\n");
    printf("\tnode flags : %u", node->node_nw_prop.flags);
    if(node->node_nw_prop.is_lb_addr_config){
        printf("\t  lo addr : %s/32\n\n", NODE_LO_ADDR(node));
    }
}

void dump_intf_props(interface_t *interface){

    dump_interface(interface);

    if(interface->intf_nw_props.is_ipadd_config){
        printf("\tIP Addr = %s/%u", IF_IP(interface), interface->intf_nw_props.mask);
    }
    else{
         printf("\tIP Addr = %s/%u", "Nil", 0);
    }

    printf("\t MAC : %x:%x:%x:%x:%x:%x\n", 
        IF_MAC(interface)[0], IF_MAC(interface)[1],
        IF_MAC(interface)[2], IF_MAC(interface)[3],
        IF_MAC(interface)[4], IF_MAC(interface)[5]);
}

void dump_nw_graph(graph_t *graph){

	node_t *node = NULL;
	dll_t *curr = NULL;
	interface_t *interface;
	unsigned int i;

	printf("========================================\n");
	printf("  Topology Name = %s\n", graph->topology_name);
	printf("========================================\n");

	dll_traverse(&graph->dll_unit_list, curr){
		node = dll_to_node(curr);
		dump_node_nw_props(node);
		for (i = 0; i < MAX_INTF_PER_NODE; i++)
		{
			interface = node->intf[i];
			if (!interface)
				break;
			dump_intf_props(interface);
			printf("    ----------------------------------\n");
		}
	}

	printf("========================================\n");
}

interface_t *
node_get_matching_subnet_interface(node_t *node, char *ip_addr){

    unsigned int i = 0;
    interface_t *intf;

    char *intf_addr = NULL;
    char mask;
    char intf_subnet[16];
    char subnet2[16];

    for( ; i < MAX_INTF_PER_NODE; i++){
        
        intf = node->intf[i];
        if(!intf) return NULL;

        if(intf->intf_nw_props.is_ipadd_config == FALSE)
            continue;
            
        intf_addr = IF_IP(intf);
        mask = intf->intf_nw_props.mask;

        memset(intf_subnet, 0 , 16);
        memset(subnet2, 0 , 16);
        apply_mask(intf_addr, mask, intf_subnet);
        apply_mask(ip_addr, mask, subnet2);
            
        if(strncmp(intf_subnet, subnet2, 16) == 0){ 
            return intf;
        }   
    }   
    return NULL;
} 
