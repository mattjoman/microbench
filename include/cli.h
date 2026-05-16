#ifndef CLI_H
#define CLI_H

#include "../include/cyclops.h"

cyclops_cfg_t *cli_cfg_init(int argc, char *argv[]);
void cli_cfg_destroy(cyclops_cfg_t *cli_cfg);
void print_main_help_text(void);
bool cli_cfg_validate(cyclops_cfg_t *cli_cfg);

#endif
