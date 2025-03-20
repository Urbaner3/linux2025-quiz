#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "alist.h"

static list_item_t items[N];
static list_t l;

static list_t lsa, lsb;
static list_item_t a_items[5], b_items[5];

#define setup_run_test(test, l, l_items, length, array) \
    do {                                               \
        char *message = test(l, l_items, length, array); \
        tests_run++;                                   \
        if (message)                                   \
            return message;                            \
    } while (0)


static char *list_setup(list_t *l, list_item_t *l_items, int length,
                        int *array) {
  // Construction.
  int check_board;
  for (size_t i = 0; i < length; i++) {
    l_items[i].value = array[i];
    l_items[i].next = NULL;
  }
  l->head = NULL;

  // test and insert
  // to catch return of my_assert, use a message variable.
  // return messages here
  char *message;
  // my_assert(list_size(l) == 0, "Initial list size is expected to be zero.");
    if (list_size(l) != 0) {
      message = "Initial list size is expected to be zero.";
      return message;
    }
  for (size_t i = 0; i < length; i++)
      list_insert_before(l, NULL, &l_items[i]);
  // my_assert(list_size(l) == length, "list size should be N");
    check_board = list_size(l);
    if (list_size(l) != length) {
      message = "Final list size should be N";
      fprintf(stdout, "List size is %d\n", list_size(l));
      return message;
    }
    else
    	return NULL;
}

static list_t *list_reset(void) {
  for (size_t i = 0; i < N; i++) {
    items[i].value = i;
    items[i].next = NULL;
  }
  l.head = NULL;
  return &l;
}

static char *list_merge(list_t *head, list_item_t *L1, list_item_t *L2){
  list_item_t *merged_head;
  merged_head = mergeTwoLists(L1, L2);
//check size and sorted with comparison.
  //merge into new list, maybe use list_setup
}

static bool *list_is_sorted(list_t *head);


static char *test_list(void) {
  /* Test inserting at the beginning */
  list_reset();
  my_assert(list_size(&l) == 0, "Initial list size is expected to be zero.");
  for (size_t i = 0; i < N; i++)
    list_insert_before(&l, l.head, &items[i]);
  my_assert(list_size(&l) == N, "Final list size should be N");
  size_t k = N - 1;
  list_item_t *cur = l.head;
  while (cur) {
    my_assert(cur->value == k, "Unexpected list item value");
    k--;
    cur = cur->next;
  }

  /* Test inserting at the end */
  list_reset();
  my_assert(list_size(&l) == 0, "Initial list size is expected to be zero.");
  for (size_t i = 0; i < N; i++)
    list_insert_before(&l, NULL, &items[i]);
  my_assert(list_size(&l) == N, "Final list size should be N");
  k = 0;
  cur = l.head;
  while (cur) {
    my_assert(cur->value == k, "Unexpected list item value");
    k++;
    cur = cur->next;
  }

  /* Reset the list and insert elements in order (i.e. at the end) */
  list_reset();
  my_assert(list_size(&l) == 0, "Initial list size is expected to be zero.");
  for (size_t i = 0; i < N; i++)
    list_insert_before(&l, NULL, &items[i]);
  my_assert(list_size(&l) == N, "list size should be N");

  return NULL;
}

int tests_run = 0;

static char *test_suite(void) {
  
  static list_t *alh = &lsa, *blh = &lsb;
  int arr_a[5] = {1, 3, 5, 7, 9}; 
  int arr_b[5] = {2, 4, 6, 8, 10};
  
  static list_t *mlh = &l;
  //my_run_test(test_list);
  //setup_run_test(list_setup);
  setup_run_test(list_setup, alh, a_items, 5, arr_a);
  setup_run_test(list_setup, blh, b_items, 5, arr_b);
  return NULL;
}

/* Count the size of the list */
static int list_size(list_t *mylist) {
  list_item_t *curr;
  curr = mylist->head;
  int count = 0;
  while (curr) {
    count++;
    curr = curr->next;
  }
  /*if(count>0)
  	count--;*/
  return count;
}

/* Insert a node before target. */
static inline void list_insert_before(list_t *l, list_item_t *before,
                                      list_item_t *item) {
  list_item_t **p;
  for (p = &l->head; *p != before; p = &(*p)->next)
    ;
  *p = item;
  item->next = before;
}

static list_item_t *mergeTwoLists(list_item_t *L1, list_item_t *L2) {
  list_item_t *head = malloc(sizeof(list_item_t));
  list_item_t *ptr = head;
  while (L1 && L2) {
    if (L1->value < L2->value) {
      ptr->next = L1;
      L1 = L1->next;
    } else {
      ptr->next = L2;
      L2 = L2->next;
    }
    ptr = ptr->next;
  }
  ptr->next = L1 ? L1 : L2;

  // free the malloc
  list_item_t *t_head = head;
  free(head);
  head = NULL;
  return t_head->next;
}

int main(void) {
  

  // Open a file to store error message
  /*FILE *errorFile = fopen("error_log.txt", "w");
  if (errorFile == NULL) {
    fprintf(stderr, "Error: Could not open file for writing error messages.\n");
    return 1;
  }

  printf("---=[ Merge tests\n");
  
  // result = list_setup(alh, a_items, 5, &arr_a[0]);
  if (result) {

    fprintf(errorFile, "ERROR: %s\n", result);

    printf("ERROR: %s\n", result);
    fprintf(errorFile, "ERROR after prf: %s\n", result);
  } else
    printf("List A tests are passed!!\n");

  result = list_setup(blh, b_items, 5, &arr_b[0]);
  if (result)
    printf("ERROR: %s\n", result);
  else
    printf("List B tests are passed!!\n");
*/
  printf("---=[ List tests\n");
  char *result;
  result = test_suite();
  if (result)
    printf("ERROR: %s\n", result);
  else
    printf("ALL TESTS of List PASSED\n");
  printf("Tests run: %d\n", tests_run);

  // Close the error log file
  //fclose(errorFile);

  //printf("Program completed. Check 'error_log.txt' for error messages.\n");
  return !!result;
}
