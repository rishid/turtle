# Turtle library

Turtle library is a command line interpreter to rapidly build simple shells. Its typical use can be for testing, debugging and interacting with server daemons. It allows you to call C functions with different arguments just by passing strings to the library. The library will parse the string and call the command's function with the appropriate arguments.

Its goal is provide a rapid way to create a simple and safe shell interface to daemons. It can very easily be wrapped to accept commands over a localhost-bound tcp/udp socket to remotely execute commands. It uses libffi to dynamically build a callstack and call methods with the the arguments provided.

## Example
```c
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
    /* look up connection using id and then print packets, bytes etc... */
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
```

## Command format specifiers
| Directive     | Meaning                  | Notes
| ------------- | ------------------------ | --------------
| %b            | boolean                  |
| %c            | char                     |
| %f            | double                   |
| %d            | int64_t                  |
| %I            | IPv4 address string      | Will be converted to network ordered 32-bit integer for method invocation
| %s            | char *                   |

## Building

This project requires CMake to build. libffi is required but a copy is maintained in the repository as of now. I have wrapped the CMake build calls in a Makefile for ease.

```
git clone https://github.com/rishid/turtle
cd turtle
make
make test
```

## TODO

There are a lot of features I want to add to the library over time

- [ ] Implement extended parameter validations features
- [ ] Add support for optional parameters
- [ ] Add support for switch params (e.g. '-n %d')
- [ ] Add support for key=value params
- [ ] Allow switch and key=value params to be in entered in any order


## License

MIT © Rishi Dhupar
