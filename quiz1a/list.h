#include <stddef.h>
typedef struct list_item {
    int value;
    struct list_item *next;
} list_item_t;

typedef struct {
    struct list_item *head;
} list_t;

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
