#include "network_graph.h"
#include "communication.h"

graph_t *build_first_topo(){
#if 0

            40.1.1.1      +----------+20.1.1.1
            ethernet 1/1/5|122.1.1.0 |etherbet 1/1/1
         +----------------+   DUT1   +---------------------------+
         |                |          |                           |
         |                +----------+                           |
         |                                                       |
         |                                                       |
         |                                                       |
         |40.1.1.2                                               |20.1.1.2
         |ethernet 1/1/6                                         |ethernet 1/1/2
    +-----+---+ 30.1.1.2                       30.1.1.1      +----+-----+
    |122.1.1.2|ethernet 1/1/4                  ethernet 1/1/3|122.1.1.1 |
    | DUT3    +----------------------------------------------+    DUT2  |
    |         |                                              |          |
    +---------+                                              +----------+

#endif

 graph_t *topo = create_new_graph("Ring 3 DUT Topo");
    node_t *DUT1 = create_graph_node(topo, "DUT1");
    node_t *DUT2 = create_graph_node(topo, "DUT2");
    node_t *DUT3 = create_graph_node(topo, "DUT3");

    insert_link_between_two_nodes(DUT1, DUT2, "ethernet 1/1/1", "ethernet 1/1/2", 1);
    insert_link_between_two_nodes(DUT2, DUT3, "ethernet 1/1/3", "ethernet 1/1/4", 1);
    insert_link_between_two_nodes(DUT1, DUT3, "ethernet 1/1/5", "ethernet 1/1/6", 1);

    node_set_loopback_address(DUT1, "122.1.1.0");
    node_set_intf_ip_address(DUT1, "ethernet 1/1/5", "40.1.1.1", 24);
    node_set_intf_ip_address(DUT1, "ethernet 1/1/1", "20.1.1.1", 24);

    node_set_loopback_address(DUT2, "122.1.1.1");
    node_set_intf_ip_address(DUT2, "ethernet 1/1/2", "20.1.1.2", 24);
    node_set_intf_ip_address(DUT2, "ethernet 1/1/3", "30.1.1.1", 24);

    node_set_loopback_address(DUT3, "122.1.1.2");
    node_set_intf_ip_address(DUT3, "ethernet 1/1/4", "30.1.1.2", 24);
    node_set_intf_ip_address(DUT3, "ethernet 1/1/6", "40.1.1.2", 24);

    network_start_pkt_receiver_thread(topo);

return topo;
}