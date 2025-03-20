#include <stddef.h>
typedef struct list_item {
    int value;
    struct list_item *next;
} list_item_t;

typedef struct {
    struct list_item *head;
} list_t;

#define my_assert(test, message) \
    do {                         \
        if (!(test))             \
            return message;      \
    } while (0)
#define my_run_test(test)       \
    do {                        \
        char *message = test(); \
        tests_run++;            \
        if (message)            \
            return message;     \
    } while (0)

#define N 1000



//static list_t *list_reset(void);
static inline char *list_setup(list_t *l, list_item_t *l_items, int length, int *array);

static int list_size(list_t *mylist);

/*
 * Insert a new item into the list before a specified item.
 * This function traverses the list to locate the position immediately before
 * the item pointed to by @before and inserts @item in that position.
 * the time complexity is O(n), where n is the number of steps needed to
 * reach @before from the head of the list.
 *
 * Parameters:
 * @l       : Pointer to the list.
 * @before  : Pointer to the item before which the new item should be inserted.
 *  - If @before is the head of the list, the new item is inserted
 *  at the front.
 *  - If @before is NULL, the new item appended to the end of
 *  the list.
 *  - In all other cases, behavior is undefined if @before does not
 *  belong to @l.
 *  @item   : The new list item to be inserted.
 */

static inline void list_insert_before(list_t *l, list_item_t *before, list_item_t *item);

//static char *test_list(void);
//static char *test_suite(void);

/*
* Merge two sorted lists as one list. It used to be linux style. Adopt it
* so that it is in one way linked list style.
*
* Parameters: @L1 is the first list, and @L2 is the second one. 
*
* Returen: A merged list is returned.
*/
static inline list_item_t *mergeTwoLists(list_item_t *L1, list_item_t *L2);

