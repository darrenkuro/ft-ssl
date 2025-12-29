#include "command.h"
#include <stdlib.h>

static const t_command g_commands[] = {
    {"md5", cmd_md5}, {"sha256", cmd_sha256}, {"help", cmd_help}, {NULL, NULL, NULL}};

int run_cmd(int ac, char **av) {}

int cmd_md5(int ac, char **av) {}

int cmd_sha256(int ac, char **av) {}

int cmd_help(int ac, char **av) {}
