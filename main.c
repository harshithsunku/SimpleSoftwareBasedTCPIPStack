#include <stdio.h>
#include "logger.h"
#include "dll.h"
#include "network_graph.h"
#include <stdlib.h>

int main()
{

    // Initialize the logger
    if (log_init("log.txt", LOG_LEVEL_INFO) != 0)
    {
        fprintf(stderr, "Failed to initialize logger\n");
        return 1;
    }

    // Write a log message indicating the application has started
    log_write(LOG_LEVEL_INFO, "Application started");

    graph_t *topo = build_first_topo();
    dump_graph(topo);

    // Write a log message indicating the application is ending
    log_write(LOG_LEVEL_INFO, "Application ending");

    // Function to print the log in reverse order.
    read_log_reverse_thread_safe("log.txt");

    // Close the logger
    log_close();

    return 0;
}