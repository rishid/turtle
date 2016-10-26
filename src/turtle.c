/**
 * Copyright (c) 2016 Rishi Dhupar
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 * @brief main file for turtle library
 *
 */

#include "turtle.h"
#include "turtle_private.h"
#include "parser.h"
#include "ffi.h"
#include "art.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>

/**
 * List of commands, set after initialization.
 */
struct turtle_table
{
    turtle_cmd_t *commands;
    int command_count;
};

static struct turtle_table _table;
/// contains the adaptive radix tree uses for command lookup
static art_tree _tree;

static void _turtle_help(void);
static turtle_cmd_t _help_cmd = TURTLE_CMD_NP("help", "the help command", _turtle_help);

/**
 * Counts the number of specifiers in a string
 *
 * @param p a null terminated string with specifiers
 *
 * @return number of specifiers
 */
static int _count_specifiers(const char *p)
{
    int rc = 0;

    if (!p)
    {
        return 0;
    }

    while (*p != '\0')
    {
        if (*p == '%')
        {
            rc++;
        }
        p++;
    }
    return rc;
}

/**
 * Find the command in the list of commands
 *
 * @param cmd_name name of command
 *
 * @return cmd structure else NULL if not found
 */
static const turtle_cmd_t * _find_command(const char *cmd_name)
{
    return art_search(&_tree, (const unsigned char *)cmd_name, strlen(cmd_name));
#if 0
    for (const turtle_cmd_t *p = _table.commands; p && p->name != NULL; p++)
    {
        // Command name match?
        if (strcmp(p->name, cmd_name) == 0)
        {
            return p;
        }
    }

    return NULL;
#endif
}

/**
 * Parse specifier format provided in @param p and set properties in @param arg
 * Sets p to character after specifier
 *
 * @param p Specifier format, updated to character after specifier on return
 * @param arg argument structure to store specifier meta data
 *
 * @return true if valid specifier was found
 */
static bool _parse_specifier(const char **p, struct arg_t *arg)
{
    const char *s = *p;

    if (*s != '%')
    {
        return false;
    }

    s++;

    switch (*s)
    {
        case 'b':
            arg->type = ARG_BOOL;
            break;
        case 'c':
            arg->type = ARG_CHAR;
            break;
        case 'f':
            arg->type = ARG_FLOAT;
            break;
        case 'd':
        case 'i':
            arg->type = ARG_INTEGER;
            break;
        case 'I':
            arg->type = ARG_IP_ADDR;
            break;
        case 's':
            arg->type = ARG_STRING;
            break;
        default:
            fprintf(stderr, "turtle: invalid format specifier (%c)\n", *s);
            return false;
    }
    
    s++;
    *p = s;

    return true;
}

/**
 * Takes the shell format string as input and fills in the
 * appropriate arg_t struct for each parameter
 *
 * @param format format string
 * @param args list of arguments to be filled in
 *
 * @return true if no errors are encountered while parsing format string
 */
static bool _parse_specifiers(const char *format, struct arg_t args[TURTLE_MAX_CMD_PARAMS])
{
    const char *p = format;
    int cnt = 0;

    while (*p != '\0')
    {
        if (isspace(*p))
        {
            p++;
            continue;
        }

        switch (*p)
        {
            case '%': /* specifier */
                if (!_parse_specifier(&p, &args[cnt]))
                {
                  return false;
                }
                cnt++;
                break;
            case '-': /* -n <specifier> */
            {
                char switch_arg = *(p+1);
                if (!isalpha(switch_arg) || (*(p+2) != ' ')) // after '-' require alpha character and then space
                {
                    return false;
                }
                p += 2; // move to format specifier
                if (!_parse_specifier(&p, &args[cnt]))
                {
                  return false;
                }
                cnt++;
                break;
            }
            default:
                /* check for key=value */
                for (;;)
                {
                    if (*p == '\0')
                    {
                        return false;
                    }
                    if (!isalpha(*p)) // keys are alpha only
                    {
                        return false;
                    }
                    if (*p == '=')
                    {
                        p++;
                        _parse_specifier(&p, &args[cnt]);
                        cnt++;
                        break;
                    }
                    /* this is not a key=value */
                    return false;
                }
        }
    }

    return 0;
}

static bool _create_arg_metadata(turtle_cmd_t *cmd)
{
    cmd->arg_count = _count_specifiers(cmd->parameters);

    if (cmd->arg_count > TURTLE_MAX_CMD_PARAMS)
    {
        fprintf(stderr, "turtle: command has too many parameters (%s)\n", cmd->name);
        return false;
    }

    if (cmd->arg_count)
    {
        cmd->args = malloc(sizeof(struct arg_t) * cmd->arg_count);

        if (cmd->args == NULL)
        {
            perror("malloc");
            return false;
        }

        _parse_specifiers(cmd->parameters, cmd->args);
    }

    return true;
}

static void _turtle_help(void)
{
    fprintf(stdout, "\nList of commands (cnt %d):\n", _table.command_count);
    for (const turtle_cmd_t *p = _table.commands; p && p->name != NULL; p++)
    {
        fprintf(stdout, "  - %s %s\n", p->name, p->parameters);
        if (p->help)
        {
          fprintf(stdout, "      %s\n", p->help);
        }
    }
    fprintf(stdout, "\n");
}

bool turtle_init(turtle_cmd_list_t list)
{
    // save a pointer to the list of commands
    _table.commands = list;
    _table.command_count = 0;

    if (art_tree_init(&_tree))
    {
      return false;
    }
    
    turtle_cmd_t *cmd = &list[0];

    // for each command, build the meta data about each argument based
    // on the parameters string
    while (cmd && cmd->name != NULL)
    {
        _table.command_count++;

        // check if command matched a reserved command name
        if (strcmp("help", cmd->name) == 0)
        {
            fprintf(stderr, "turtle: reserved command name: '%s'\n", cmd->name);
            return false;
        }

        if (art_insert(&_tree, (uint8_t *)cmd->name, strlen(cmd->name), cmd))
        {
            fprintf(stderr, "turtle: duplicate command name: '%s'\n", cmd->name);
            return false;
        }

        if (!_create_arg_metadata(cmd))
        {
            fprintf(stderr, "turtle: failed to create arg metadata for '%s'\n", cmd->name);
            return false;
        }

        cmd++;
    }

    // Insert help command
    art_insert(&_tree, (uint8_t *)"help", 4, &_help_cmd);
    _create_arg_metadata(&_help_cmd);

    return true;
}

bool turtle_deinit(void)
{    
    int atd = art_tree_destroy(&_tree);
    
    for (turtle_cmd_t *p = _table.commands; p && p->name != NULL; p++)
    {
        if (p->arg_count)
        {
          free(p->args);
        }
    }
    
    return (atd == 0);
}

/**
 * Split string by space tokens while taking into account strings
 * enclosed in single or double quotes
 *
 * @param param_str the string to be split
 * @param params array of pointers where start of each token will be stored
 *
 * @return number of parameters found
 *
 * @todo this is very rudimentary tokenizer and needs to be more robust and safer against bad
 * bad inputs
 */
static size_t _split_params(char *param_str, char * params[TURTLE_MAX_CMD_PARAMS])
{
    char *start_of_word = NULL;
    enum states { IDLE, IN_WORD, IN_QUOTES, IN_SINGLE_QUOTES } state = IDLE;
    size_t n = 0;

    for (char *p = param_str; n < TURTLE_MAX_CMD_PARAMS && *p != '\0'; p++)
    {
        char c = *p;
        switch (state)
        {
            case IDLE:
                if (isspace(c))
                {
                    continue;
                }

                if (c == '"')
                {
                    state = IN_QUOTES;
                    start_of_word = p + 1;
                    continue;
                }
                else if (c == '\'')
                {
                    state = IN_SINGLE_QUOTES;
                    start_of_word = p + 1;
                    continue;
                }
                state = IN_WORD;
                start_of_word = p;
                continue;

            case IN_WORD:
                if (isspace(c))
                {
                    *p = '\0'; // terminate parameter
                    params[n] = start_of_word;
                    n++;
                    state = IDLE;
                }
                continue;

            case IN_QUOTES:
                if (c == '"')
                {
                    *p = '\0'; // terminate parameter
                    params[n] = start_of_word;
                    n++;
                    state = IDLE;
                }
                continue;

            case IN_SINGLE_QUOTES:
                if (c == '\'')
                {
                    *p = '\0'; // terminate parameter
                    params[n] = start_of_word;
                    n++;
                    state = IDLE;
                }
                continue;
        }
    }

    // catch last param
    if (state != IDLE && n < TURTLE_MAX_CMD_PARAMS)
        params[n++] = start_of_word;

    return n;
}

/**
 * Execute a command with variable number of arguments
 *
 * 1. go through param input and tokenize, output is an array of
 *    char * pointers to beginning of each parameter
 * 2. go through arg definitions and validate/convert
 * 3. execute command
 *
 * @param cmd command structure that will be executed
 * @param param_str string of parameters
 *
 * @return true if command is executed successfully
 */
static bool _execute_cmd_with_args(const turtle_cmd_t *cmd, char *param_str)
{
    /* Tokenize the parameter string and make sure user passed in
     * correct number of parameters required for this commmand
     */
    char *params[TURTLE_MAX_CMD_PARAMS];
    int arg_count = _split_params(param_str, params);

    if (arg_count != cmd->arg_count)
    {
        // TODO: optionals will break this
        fprintf(stderr, "Received %d parameters but commands expects %ld\n",
                arg_count, cmd->arg_count);
        return false;
    }

    union arg_value arg_values[TURTLE_MAX_CMD_PARAMS];
    ffi_type *args[TURTLE_MAX_CMD_PARAMS];
    void *values[TURTLE_MAX_CMD_PARAMS];

    /* Iterate through the command arguments and parse each parameter accordingly.
     * @note this will need to be re-worked when we support optionals
     *       and out of order parameters
     */
    for (int i = 0; i < cmd->arg_count; ++i)
    {
        switch (cmd->args[i].type)
        {             
            case ARG_BOOL:
            {
                args[i] = &ffi_type_uint8;
                arg_values[i].boolarg = parse_bool(params[i]);
                values[i] = &arg_values[i].boolarg;
                break;
            }
            case ARG_CHAR:
            {
                int ch = (int)(*params[i]);
                if (isprint(ch))
                {
                    args[i] = &ffi_type_sint8;
                    arg_values[i].chararg = ch;
                    values[i] = &arg_values[i].chararg;
                }
                else
                {
                    fprintf(stderr,
                            "Parse error: tried to parse param #%d (0x%02X) as character but failed\n",
                            i, ch);
                    return false;
                }

                break;
            }
            case ARG_FLOAT:
            {
                bool rc = parse_float(params[i], &arg_values[i].doublearg);

                if (!rc)
                {
                    return false;
                }

                args[i] = &ffi_type_double;
                values[i] = &arg_values[i].doublearg;
                break;
            }
            case ARG_INTEGER:
            {
                bool rc = parse_integer(params[i], &arg_values[i].longlongarg);

                if (!rc)
                {
                    return false;
                }

                args[i] = &ffi_type_sint64;
                values[i] = &arg_values[i].longlongarg;
                break;
            }
            case ARG_IP_ADDR:
            {
                bool rc = parse_ip_address(params[i], &arg_values[i].ipaddrarg);

                if (!rc)
                {
                    return false;
                }

                args[i] = &ffi_type_uint32;
                values[i] = &arg_values[i].ipaddrarg;
                break;
            }
            case ARG_STRING:
            {
                args[i] = &ffi_type_pointer;
                values[i] = &params[i];
                break;
            }
            default:
                fprintf(stderr, "Invalid argument type %d\n", cmd->args[i].type);
                return false;
        }
    }

    ffi_cif cif;

    // Have libffi prep the callstack
    if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, cmd->arg_count, &ffi_type_void, args) == FFI_OK)
    {
        // Call the method with the args
        ffi_call(&cif, cmd->callback, NULL, values);
    }

    return true;
}

bool turtle_execute(const char *line)
{
    bool rc = false;

    if (NULL == line || strlen(line) == 0)
    {
        return false;
    }
    
    printf("execute %s\n", line);

    // Copy input line to be used for strtok
    char *line_dup = strdup(line);
    char *line_save = NULL;

    // command name is first word
    const char *name = strtok_r(line_dup, " ", &line_save);

    // Search for command in table
    const turtle_cmd_t *cmd = _find_command(name);
    
    if (cmd)
    {
        //printf("%s found cmd\n", cmd->name);
        /* If the command has arguments, we need to build a call
         * stack prior to calling the function.
         */
        // TODO: will this work for commands with all optionals?
        if (cmd->arg_count)
        {
            rc = _execute_cmd_with_args(cmd, line_save);
        }
        else
        {
            ffi_cif cif;

            // Prep call stack for a method with no parameters
            if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 0,
                             &ffi_type_void, NULL) == FFI_OK)
            {
                ffi_call(&cif, cmd->callback, NULL, NULL);
                rc = true;
            }
        }
    }

    free(line_dup);

    return rc;
}
