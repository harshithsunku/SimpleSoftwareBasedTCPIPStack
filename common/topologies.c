#include "network_graph.h"

graph_t *build_first_topo(){
#if 0

                          +----------+
            ethernet 1/1/5|          |etherbet 1/1/1
         +----------------+   DUT1   +---------------------------+
         |                |          |                           |
         |                +----------+                           |
         |                                                       |
         |                                                       |
         |                                                       |
         |                                                       |
         |ethernet 1/1/6                                         |ethernet 1/1/2
     +---+---+                                              +----+-----+
     |       |ethernet 1/1/4                  ethernet 1/1/3|          |
     | DUT3  +----------------------------------------------+    DUT2  |
     |       |                                              |          |
     +-------+                                              +----------+

#endif

 graph_t *topo = create_new_graph("Ring 3 DUT Topo");
    node_t *DUT1 = create_graph_node(topo, "DUT1");
    node_t *DUT2 = create_graph_node(topo, "DUT2");
    node_t *DUT3 = create_graph_node(topo, "DUT3");

    insert_link_between_two_nodes(DUT1, DUT2, "etherbet 1/1/1", "ethernet 1/1/2", 1);
    insert_link_between_two_nodes(DUT2, DUT3, "ethernet 1/1/3", "ethernet 1/1/4", 1);
    insert_link_between_two_nodes(DUT1, DUT3, "ethernet 1/1/5", "ethernet 1/1/6", 1);

return topo;
}