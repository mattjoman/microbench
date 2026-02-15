#ifndef COUNTER_GROUP_H
#define COUNTER_GROUP_H

#define MAX_COUNTER_GRP_SIZE 3

typedef struct ctr_grp {
    int id;
    int size;
    int ctr_ids[MAX_COUNTER_GRP_SIZE];
} ctr_grp_t;

enum {
    COUNTER_GRP_IPC,
    NUMBER_OF_COUNTER_GRPS,
};

extern const char *counter_grp_names[NUMBER_OF_COUNTER_GRPS];

const ctr_grp_t *get_ctr_grp(int ctr_grp_id);

#endif
