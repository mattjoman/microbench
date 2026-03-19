#ifndef WORKLOAD_H
#define WORKLOAD_H

#define MAX_WL_PARAMS 5

typedef struct {
    const char *key;
    const char *value;
} wl_arg_t;

typedef struct {
    int n_args;
    wl_arg_t args[MAX_WL_PARAMS];
} wl_arg_slice_t;

typedef struct {
    const char *key;
    const char *default_value;
} wl_param_t;

typedef struct workload workload_t;
typedef struct workload {
    const char* name;

    const int n_params;
    const wl_param_t *params;

    void (*init)(workload_t *wl, wl_arg_slice_t *wl_args);
    void (*clean)(void);
    void (*workload)(void);
} workload_t;

void register_workload(workload_t *wl);

#define REGISTER_WORKLOAD(wl_ptr) \
    static void __attribute((constructor)) register_wl(void) { \
        register_workload(wl_ptr); \
    }

void print_workload_guide(void);

workload_t *get_workload_by_name(const char *name);

int get_wl_param_val(workload_t *wl, wl_arg_slice_t *wl_args, const char *key);

#endif
