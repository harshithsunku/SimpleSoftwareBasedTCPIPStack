#include <stdio.h>
#include "logger.h"
#include "dll.h"
#include "network_graph.h"
#include <stdlib.h>
#include "cmdtlv.h"
#include "libcli.h"
#define CMD_SHOW_TOPO 1
#define CMD_SHOW_LOG 2
#define CMD_RUN_ARP 3

extern graph_t *topo;

int topo_callback_handler(param_t *param, ser_buff_t *tlv_buf, op_mode enable_or_disable)
{
	printf("%s() is called ...\n", __FUNCTION__);
	int CMD_CODE = -1;
	CMD_CODE = EXTRACT_CMD_CODE(tlv_buf);
	switch (CMD_CODE)
	{
	case CMD_SHOW_TOPO:
		dump_nw_graph(topo);
		break;
	default:
		printf("command code not set properly or wrong callback troggered\n");
	}
	return 0;
}

int
validate_node_topo(char *value){

    printf("%s() is called with value = %s\n", __FUNCTION__, value);
    return VALIDATION_SUCCESS; /*else return VALIDATION_FAILED*/
}


int
log_callback_handler(param_t *param, ser_buff_t *tlv_buf, op_mode enable_or_disable){
    printf("%s() is called ...\n", __FUNCTION__);
    read_log_reverse_thread_safe("log.txt");
    return 0;
}

int
validate_node_log(char *value){

    printf("%s() is called with value = %s\n", __FUNCTION__, value);
    return VALIDATION_SUCCESS; /*else return VALIDATION_FAILED*/
}

int
validate_node_extistence(char *value){

    printf("%s() is called with value = %s\n", __FUNCTION__, value);
    return VALIDATION_SUCCESS; /*else return VALIDATION_FAILED*/
}

static int
arp_handler(param_t *param, ser_buff_t *tlv_buf,
                op_mode enable_or_disable){
    char *node_name;
    char *ip_addr;
    tlv_struct_t *tlv = NULL;

    TLV_LOOP_BEGIN(tlv_buf, tlv){
		if(!tlv){
			printf("tlv null\n");
		}
        if(strncmp(tlv->leaf_id, "node-name", strlen("node-name")) ==0)
            node_name = tlv->value;
        else if(strncmp(tlv->leaf_id, "ip-address", strlen("ip-address")) ==0)
            ip_addr = tlv->value;
    } TLV_LOOP_END;

	printf("node_name : %s\n", node_name);
	printf("ip_addr : %s\n", ip_addr);
    return 0;
}


void init_app_specific_cli()
{
	param_t *show   = libcli_get_show_hook();
	// param_t *debug  = libcli_get_debug_hook();
	// param_t *config = libcli_get_config_hook();
	// param_t *clear  = libcli_get_clear_hook();
	param_t *run    = libcli_get_run_hook();
	{
		/*Implementing CMD1 : show node <node-name>*/
		{
			/*show node*/
			static param_t topo;    /*Get the param_t variable, either a static memory or heap memory, not stack memory*/
			init_param(&topo,       /*Address of the current param*/
					CMD,            /*CMD for command param, LEAF for leaf param*/
					"topo",         /*Name of the param, this string is what is displayed in command line*/
					topo_callback_handler,              /*callback : pointer to application function. Null in this case since 'show node' is not a complete command*/
					validate_node_topo,              /*Applicable only for LEAF params. Always NULL for CMD param*/
					INVALID,        /*Always INVALID for CMD params*/
					0,              /*Always NULL for CMD params*/
					"Help : show topology"); /*Help String*/

			libcli_register_param(show, &topo); /*Add node param as suboption of show param*/

			set_param_cmd_code(&topo, CMD_SHOW_TOPO);
		}
		/*Implementing CMD2*/
		{
			/*show node <node-name> loopback <loopback-address>*/
			static param_t log;   /*Get the param_t variable, either a static memory or heap memory, not stack memory*/
			init_param(&log,      /*Address of the current param*/
					CMD,                      /*CMD for command param, LEAF for leaf param. Since it is a leaf param which takes node names, hence pass LEAF*/
					"log",                         /*Always NULL for LEAF param*/
					log_callback_handler, /*Since this is complete command, it should invoke application routine. Pass the pointer to that routine here.*/
					validate_node_log,      /*Optional : can be NULL. This is also application specific routine, and perform validation test
											  to the value entered by the user for this leaf param. If validation pass, then only node_loopback_callback_handler routine is invoked*/
					INVALID,                    /*leaf param value type. loopback address is IPV4 type, hence pass IPV4*/
					0,           /*Applicable only for LEAF param. Give some name to leaf-params. It is this string that we will parse in application code to find the value passed by the user*/
					"Help : show log");  /*Help String*/
			libcli_register_param(show, &log);   /*Add node_name leaf param as suboption of <node-name> param. Note that: show --> node --> node_name --> lo-address has been chained*/
			/* The below API should be called for param at which the command is supposed to invoke application callback rouine.
			 * This CMDODE_SHOW_NODE_LOOPBACK code is sent to application using which we find which command was triggered, and accordingly what
			 * are expected leaf params we need to parse. More on this ater.*/
			set_param_cmd_code(&log, CMD_SHOW_LOG);
		}
		/*Implementing CMD3*/
		{
			static param_t node;
			init_param(&node, CMD, "node", 0, 0, INVALID, 0, "\"node\" keyword");
			libcli_register_param(run, &node);
			//libcli_register_display_callback(&node, display_graph_nodes);
			{
				/*run node <node-name>*/
				static param_t node_name;
				init_param(&node_name, LEAF, 0, 0, validate_node_extistence, STRING, "node-name", "Node Name");
				libcli_register_param(&node, &node_name);
#if 0
				{
					/*run node <node-name> ping */
					static param_t ping;
					init_param(&ping, CMD, "ping" , 0, 0, INVALID, 0, "Ping utility");
					libcli_register_param(&node_name, &ping);
					{
						/*run node <node-name> ping <ip-address>*/
						static param_t ip_addr;
						init_param(&ip_addr, LEAF, 0, ping_handler, 0, IPV4, "ip-address", "Ipv4 Address");
						libcli_register_param(&ping, &ip_addr);
						set_param_cmd_code(&ip_addr, CMDCODE_PING);
						{
							static param_t ero;
							init_param(&ero, CMD, "ero", 0, 0, INVALID, 0, "ERO(Explicit Route Object)");
							libcli_register_param(&ip_addr, &ero);
							{
								static param_t ero_ip_addr;
								init_param(&ero_ip_addr, LEAF, 0, ping_handler, 0, IPV4, "ero-ip-address", "ERO Ipv4 Address");
								libcli_register_param(&ero, &ero_ip_addr);
								set_param_cmd_code(&ero_ip_addr, CMDCODE_ERO_PING);
							}
						}
					}
				}
#endif
				{
					/*run node <node-name> resolve-arp*/
					static param_t resolve_arp;
					init_param(&resolve_arp, CMD, "resolve-arp", 0, 0, INVALID, 0, "Resolve ARP");
					libcli_register_param(&node_name, &resolve_arp);
					{
						/*run node <node-name> resolve-arp <ip-address>*/
						static param_t ip_addr;
						init_param(&ip_addr, LEAF, 0, arp_handler, 0, IPV4, "ip-address", "Nbr IPv4 Address");
						libcli_register_param(&resolve_arp, &ip_addr);
						set_param_cmd_code(&ip_addr, CMD_RUN_ARP);
					}
				}
			}

		}
	}

}



