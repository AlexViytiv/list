#include "test_list.h"

#include <stdio.h>
#include <stdlib.h>

#include "logger.h"
#include "status.h"
#include "list.h"


unit_test_item_t g_test_items[] = {
    { SC_NONE, "List init",      0, 0, unit_test_list_init },
    { SC_NONE, "List deinit",    0, 0, unit_test_list_deinit },
    { SC_NONE, "List add item",  0, 0, unit_test_list_add_item },
    { SC_NONE, "List del item",  0, 0, unit_test_list_del_item },
    { SC_NONE, "List find item", 0, 0, unit_test_list_find_item },
    { SC_NONE, "List print",     0, 0, unit_test_list_print },
};

#define UNIT_TEST_REQUISITE \
        status_code_t status = SC_OK; \
        uint16_t      test_case_id = 1;

#define UNIT_TEST_FINAL \
        out: \
            return status;

#define SET_TOTAL_CASES(id, value) \
        { g_test_items[id].test_cases = value; }

#define CASE_PASSED(id) \
        { g_test_items[id].test_cases_passed++; }

#define TEST_CASE_EXEC(id, func, exp_code, ...) \
        { \
            status = func(__VA_ARGS__); \
            if (status != exp_code) { \
                LOG_ERR( #func "() test case %u failed. Error code: %u [%s].", \
                        test_case_id, status, status_to_str(status)); \
                status = SC_ERROR; \
                goto out; \
            } \
            CASE_PASSED(id); \
            status = SC_OK; \
            test_case_id++; \
        }

status_code_t unit_test_list_init(uint32_t id)
{
    UNIT_TEST_REQUISITE
    list_t * stub_list;

    SET_TOTAL_CASES(id, 4);

    /* Case 1. Pass list double pointer NULL */
    TEST_CASE_EXEC(id, list_init, SC_PTR_NULL, NULL, LINKING_NONE);

    /* Case 2. Pass list pointer not NULL */
    stub_list = (list_t *) 0x1;
    TEST_CASE_EXEC(id, list_init, SC_PTR_NOT_NULL, &stub_list, LINKING_NONE);
    stub_list = NULL;

    /* Case 3. Pass list unsupported type */
    TEST_CASE_EXEC(id, list_init, SC_UNSUPPORTED, &stub_list, LINKING_NONE);

    /* Case 4. Normal flow */
    TEST_CASE_EXEC(id, list_init, SC_OK, &stub_list, LINKING_DOUBLE);
    free(stub_list);

    UNIT_TEST_FINAL
}

status_code_t unit_test_list_deinit(uint32_t id)
{
    UNIT_TEST_REQUISITE
    list_t *  stub_list;

    SET_TOTAL_CASES(id, 3);

    /* Case 1. Pass list pointer NULL */
    TEST_CASE_EXEC(id, list_deinit, SC_PTR_NULL, NULL);

    /* Case 2. List in use */
    stub_list = (list_t *) malloc(sizeof(list_t));
    stub_list->len = 2;
    TEST_CASE_EXEC(id, list_deinit, SC_RES_IN_USE, stub_list);

    /* Case 3. Normal flow */
    stub_list->len = 0;
    TEST_CASE_EXEC(id, list_deinit, SC_OK, stub_list);

    UNIT_TEST_FINAL
}

status_code_t unit_test_list_add_item(uint32_t id)
{
    UNIT_TEST_REQUISITE
    list_t stub_list = { NULL, NULL, 0, LINKING_DOUBLE };

    SET_TOTAL_CASES(id, 6);

    /* Case 1. Pass list pointer NULL */
    TEST_CASE_EXEC(id, list_add_item, SC_PTR_NULL, NULL, 1, -1);

    /* Case 2. Index is more than valid range */
    TEST_CASE_EXEC(id, list_add_item, SC_INV_RANGE, &stub_list, 1, 10);

    /* Case 3. List len is 0 */
    TEST_CASE_EXEC(id, list_add_item, SC_OK, &stub_list, 1, -1);

    /* Case 4. Index is 0 */
    TEST_CASE_EXEC(id, list_add_item, SC_OK, &stub_list, 2, 0);

    /* Case 5. Index is -1 */
    TEST_CASE_EXEC(id, list_add_item, SC_OK, &stub_list, 3, -1);

    /* Case 6. Index is 2 */
    TEST_CASE_EXEC(id, list_add_item, SC_OK, &stub_list, 4, 2);

    UNIT_TEST_FINAL
}

status_code_t unit_test_list_del_item(uint32_t id)
{
    UNIT_TEST_REQUISITE
    list_t   stub_list = { NULL, NULL, 0, LINKING_DOUBLE };
    uint32_t i = 0;

    SET_TOTAL_CASES(id, 5);

    /* Case 1. Pass list pointer NULL */
    TEST_CASE_EXEC(id, list_del_item, SC_PTR_NULL, NULL, 2);

    /* Case 2. Delete entry, that doesn`t exist */
    TEST_CASE_EXEC(id, list_del_item, SC_NOT_FOUND, &stub_list, 2);

    for (i = 1; i <= 5; i++) {
        status = list_add_item(&stub_list, i, -1);
        if (SC_CHECK_FAIL(status)) {
            LOG_ERR("Failed to prepare testing enviroment. Error code %d [%s].",
                    status, status_to_str(status));
            goto out;
        }
    }

    /* Case 3. Delete head entry */
    TEST_CASE_EXEC(id, list_del_item, SC_OK, &stub_list, 1);

    /* Case 4. Delete end entry */
    TEST_CASE_EXEC(id, list_del_item, SC_OK, &stub_list, 5);

    /* Case 5. Delete middle entry */
    TEST_CASE_EXEC(id, list_del_item, SC_OK, &stub_list, 3);

    /* Enviroment clear */
    status = list_del_item(&stub_list, 2);
    if (SC_CHECK_FAIL(status)) {
        LOG_ERR("Failed to clear testing enviroment. Error code %d [%s].",
                status, status_to_str(status));
    }
    status = list_del_item(&stub_list, 4);
    if (SC_CHECK_FAIL(status)) {
        LOG_ERR("Failed to clear testing enviroment. Error code %d [%s].",
                status, status_to_str(status));
    }

    UNIT_TEST_FINAL
}

status_code_t unit_test_list_find_item(uint32_t id)
{
    UNIT_TEST_REQUISITE
    list_t      stub_list = { NULL, NULL, 0, LINKING_DOUBLE };
    list_item_t curr_item = { 0, NULL, NULL };
    uint32_t    i = 0;

    SET_TOTAL_CASES(id, 6);

    /* Case 1. Pass list pointer NULL */
    TEST_CASE_EXEC(id, list_find_item, SC_PTR_NULL, NULL, 2, NULL);

    /* Case 2. Pass list item pointer NULL */
    TEST_CASE_EXEC(id, list_find_item, SC_PTR_NULL, &stub_list, 2, NULL);

    /* Case 3. Find not existing value */
    TEST_CASE_EXEC(id, list_find_item, SC_NOT_FOUND, &stub_list, 2, &curr_item);

    for (i = 1; i <= 5; i++) {
        status = list_add_item(&stub_list, i, -1);
        if (SC_CHECK_FAIL(status)) {
            LOG_ERR("Failed to prepare testing enviroment. Error code %d [%s].",
                    status, status_to_str(status));
            goto out;
        }
    }

    /* Case 4. Normal case - find at begin */
    TEST_CASE_EXEC(id, list_find_item, SC_OK, &stub_list, 1, &curr_item);
    ASSERT(curr_item.value == 1);

    /* Case 5. Normal case - find at end */
    TEST_CASE_EXEC(id, list_find_item, SC_OK, &stub_list, 5, &curr_item);
    ASSERT(curr_item.value == 5);

    /* Case 6. Normal case - find at middle */
    TEST_CASE_EXEC(id, list_find_item, SC_OK, &stub_list, 3, &curr_item);
    ASSERT(curr_item.value == 3);

    UNIT_TEST_FINAL
}

status_code_t unit_test_list_print(uint32_t id)
{
    UNIT_TEST_REQUISITE
    SET_TOTAL_CASES(id, 1);

    /* Case 1. Pass list pointer NULL */
    TEST_CASE_EXEC(id, list_print, SC_PTR_NULL, NULL);

    UNIT_TEST_FINAL
}

int main(int argc, char * argv[])
{
    uint32_t i = 0;
    uint32_t tests_num = (int) (sizeof(g_test_items) / sizeof(unit_test_item_t));
    status_code_t status = SC_OK;

    /* Run list unit tests */
    for (i = 0; i < tests_num; i++) {
        g_test_items[i].status = g_test_items[i].function(i);
    }

    status = unit_test_print_report();
    if (SC_CHECK_FAIL(status)) {
        LOG_ERR("Unit test failed to print report.");
        goto out;
    }

out:
    return status;
}

status_code_t unit_test_print_report()
{
    status_code_t status = SC_OK;
    uint32_t      i = 0;
    uint32_t      tests_num = (int) (sizeof(g_test_items) / sizeof(unit_test_item_t));
    char          cases_rate_str[6] = "";
    char          cases_passed_str[6] = "";
    const char *  format = " | %-20s | %-6s | %-6s | %-6s | %-20s |\n";

    printf(" + -------------------- + ------ + ------ + ------ + -------------------- +\n");
    printf(format, "Test", "Status", "Rate", "Passed", "Result");
    printf(" + -------------------- + ------ + ------ + ------ + -------------------- +\n");
    /* Go through unit tests */
    for (i = 0; i < tests_num; i++) {
        sprintf(cases_rate_str, "%u%c",
                (int) (g_test_items[i].test_cases_passed * 100 / g_test_items[i].test_cases), '%');
        sprintf(cases_passed_str, "%u", g_test_items[i].test_cases_passed);

        printf(format, g_test_items[i].test_name,
               (g_test_items[i].status == SC_OK) ? GRN("Passed") : RED("Failed"),
               cases_rate_str, cases_passed_str,
               status_to_str(g_test_items[i].status));
    }
    printf(" + -------------------- + ------ + ------ + ------ + -------------------- +\n");

    goto out;
out:
    return status;
}
