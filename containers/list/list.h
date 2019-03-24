#ifndef LIST_H
#define LIST_H

#include <stdint.h>

#include "../../tools/status.h"

#define LAST_ITEM_INDEX (-1)

typedef enum {
    LINKING_NONE = 0,
    LINKING_SINGLE,
    LINKING_DOUBLE,
    LINKING_CYCLED,
} linking_type_t;

typedef struct list_item {
    uint32_t value;
    struct list_item * prev;
    struct list_item * next;
} list_item_t;

typedef struct list {
    list_item_t *  head;
    list_item_t *  end;
    uint32_t       len;
    linking_type_t type;
} list_t;

status_code_t list_init(list_t ** list,
                        linking_type_t type);
status_code_t list_deinit(list_t * list);
status_code_t list_add_item(list_t * list,
                            uint32_t value,
                            uint32_t index);
status_code_t list_del_item(list_t * list,
                            uint32_t value);
status_code_t list_find_item(list_t *      list,
                             uint32_t      value,
                             list_item_t * list_item);
status_code_t list_print(list_t * list);

#endif /* LIST_H */