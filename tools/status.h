#ifndef STATUS_H
#define STATUS_H

#define ENUM(ENUM) ENUM,
#define STR(ENUM) #ENUM,

#define STATUS_LIST(FUNC)     \
        FUNC(SC_OK)           \
        FUNC(SC_ERROR)        \
        FUNC(SC_PTR_NULL)     \
        FUNC(SC_PTR_NOT_NULL) \
        FUNC(SC_INV_RANGE)    \
        FUNC(SC_NOT_FOUND)    \
        FUNC(SC_RES_IN_USE)   \
        FUNC(SC_UNSUPPORTED)  \
        FUNC(SC_NONE)         \
        FUNC(SC_MIN = SC_OK)  \
        FUNC(SC_MAX = SC_UNSUPPORTED)

#define SC_CHECK_RANGE(code) \
        ((SC_MIN <= code) && (code <= SC_MAX))

#define SC_CHECK_FAIL(status) \
        (status != SC_OK)


typedef enum {
    false = 0,
    true,
} bool_t;

#define UNUSED_VAR(var) ((void) var)
#define UNUSED_PTR(ptr) ((void *) ptr)

#define ASSERT(expr) \
        { \
            if (!(expr)) { \
                LOG_ERR("Assertion failed on expression: %s.", #expr); \
                exit(1); \
            } \
        }

typedef enum {
    STATUS_LIST(ENUM)
} status_code_t;

char * status_to_str(status_code_t code);

#endif /* STATUS_H */