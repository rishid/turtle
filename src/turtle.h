/**
 * Copyright (c) 2016 Rishi Dhupar
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 * @brief
 *
 * @note this is not thread-safe
 */
#ifndef TURTLE_H_
#define TURTLE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/// Maximum number of parameters allowed in a single command
#define TURTLE_MAX_CMD_PARAMS (32)

struct arg_t;

typedef struct turtle_cmd_t_
{
  char *name;             ///< command name
  char *parameters;       ///< parameter string using format specifiers
  char *help;             ///< short help text of what the command does
  void (*callback)(void); ///< the function to be executed

  /* private fields - internal library use */
  long arg_count;         ///< number of arguments for this command
  struct arg_t *args;     ///< internal list of meta data about each argument
} turtle_cmd_t;

// Useful for eliminating compiler warnings
#define TURTLE_FN(f) ((void (*)(void))f)

/**
 * Macros to help build initial command list.
 */
#define TURTLE_CMD(name, params, help, cb) { (name), (params), (help), (TURTLE_FN(cb)), 0, NULL }
#define TURTLE_CMD_NP(name, help, cb) { (name), "", (help), (TURTLE_FN(cb)), 0, NULL }

typedef turtle_cmd_t turtle_cmd_list_t[];

/**
 * Initialize the turtle library with a set of commands
 *
 * @param list an array of commands, NULL terminated, that the can be executed
 *
 * @return true if library is initialized successfully and all
 *         commands are valid
 */
bool turtle_init(turtle_cmd_list_t list);

/**
 * De-initalize the turtle library
 */
bool turtle_deinit(void);

/**
 * Lookup and execute a command based on the passed in string
 *
 * @param cmd the command line to be interperted
 *
 * @return true if command is executed successfully
 */
bool turtle_execute(const char *cmd);

#ifdef __cplusplus
}
#endif

#endif /* TURTLE_H_ */

/*
  %specifier[{attributes}]

  %specifier{[default]:[1,50]}

%d
%d{32}
%d{32:4-40}
%s{abc}
%s{“abc”}
%b{True}
foo=%d
foo=%d{32}
foo=%d{:1-50}
foo=%d{32:1-50}
foo=%b{True}
-n %d
-n %d{32}
-n %d{32:1-50}
-n %b{True}       %b means a switch arg which is a boolean argument and only defines a flag that indicates True/False, if no default is provided it is set to false
*/
