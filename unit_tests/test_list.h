#ifndef TEST_LIST_H
#define TEST_LIST_H

#include "status.h"
#include "list.h"

typedef struct unit_test_item {
    status_code_t status;
    char *        test_name;
    uint16_t      test_cases;
    uint16_t      test_cases_passed;
    status_code_t (*function)(uint32_t id);
} unit_test_item_t;


status_code_t unit_test_list_init(uint32_t id);
status_code_t unit_test_list_deinit(uint32_t id);
status_code_t unit_test_list_add_item(uint32_t id);
status_code_t unit_test_list_del_item(uint32_t id);
status_code_t unit_test_list_find_item(uint32_t id);
status_code_t unit_test_list_print(uint32_t id);
status_code_t unit_test_print_report();


#endif /* TEST_LIST_H */