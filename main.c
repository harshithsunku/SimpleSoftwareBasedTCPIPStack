#include <stdio.h>
#include "logger.h"
//#include "dll.h"
#include "network_graph.h"
#include "dll.h"
#include <stdlib.h>
#include "cmdtlv.h"
#include "libcli.h"
#include "communication.h"

graph_t *topo;
extern void init_app_specific_cli();
int main()
{

    // Initialize the logger
    if (log_init("log.txt", LOG_LEVEL_INFO) != 0)
    {
        fprintf(stderr, "Failed to initialize logger\n");
        return 1;
    }

    init_libcli();
    // param_t *show   = libcli_get_show_hook();
    // param_t *debug  = libcli_get_debug_hook();
    // param_t *config = libcli_get_config_hook();
    // param_t *clear  = libcli_get_clear_hook();
    // param_t *run    = libcli_get_run_hook();

    init_app_specific_cli();


    // Write a log message indicating the application has started
    log_write(LOG_LEVEL_INFO, "Application started");

    topo = build_first_topo();
    //dump_graph(topo);
    // dump_nw_graph(topo);

    // Write a log message indicating the application is ending
    log_write(LOG_LEVEL_INFO, "Application ending");

    // Function to print the log in reverse order.
    // read_log_reverse_thread_safe("log.txt");

    // // Close the logger
    // 

    // support_cmd_negation(config);

    sleep(2);

    node_t *node = get_node_by_node_name(topo, "DUT1");
    interface_t *oif = get_node_if_by_name(node, "ethernet 1/1/1");

    char msg[] = "Original Working\0";
    send_pkt_out(msg,strlen(msg),oif);

    /*Do not add any param in config command tree after above line*/
    start_shell();

    log_close();

    return 0;
}
