#include "turtle.h"

#include <stdio.h>
#include <stdint.h>

static void _list_connections(void)
{
    printf("list clients connected...\n");
}

static void _show_connection(uint32_t id)
{
    printf("connection details for conn id %u...\n", id);
}

static void _start_statsd_connection(uint32_t ip, uint16_t port)
{    
    printf("initializing statsd connection...\n");
    
    // open tcp socket to ip and port
    //statsd_init(ip, port);    
}

turtle_cmd_list_t cmds = {
    TURTLE_CMD_NP("listconn", "show list of connections", _list_connections),
    TURTLE_CMD("showconn", "%d", "show details of connection", _show_connection),
    TURTLE_CMD("statsd_connect", "%I %d", "initialize the statsd connection", _start_statsd_connection),
    { NULL, },
};

int main()
{
    turtle_init(cmds);

    turtle_execute("statsd_connect 192.168.1.2 8125");
    turtle_execute("listconn");
    turtle_execute("showconn 4");
    turtle_execute("showconn 12");
    
    return 0;
}