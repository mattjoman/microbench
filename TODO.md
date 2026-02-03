# TODO

- Make a stuct for `test`
    ```c
    struct test {
        void (*func)(void);
        void (*init)(void);
        void (*clean)(void);
    };
    ```
- Make separate structs for `batch_config` and `batch_results`
- Make a function `batch_config_init`
    - Enforce invariants
