#ifndef COMMAND_H
#define COMMAND_H

typedef struct s_command {
    const char *name;
    int (*handler)(int ac, char **av);
} t_command;

#endif
