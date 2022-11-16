#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_STORES 5
#define MAX_NAME_SIZE 20

typedef struct {
    char name[MAX_NAME_SIZE];
    double price;
} item_s;

typedef struct node_t {
    item_s item;
    struct node_t* next;
} node_t;

typedef struct {
    char name[MAX_NAME_SIZE];
    int distance;
    node_t* first_item;
} store_s;

typedef struct {
    store_s store;
    item_s item;
} cart_item;

void load_distances(); // read from file
void load_normal_prices(); // read from file
void load_discounts(); // read from file
void load_shopping_list(); // read from file
void add_item(); // load normal prices
void update_item(); // discounts

/**
 * add_item is used for declaring which items exist in the store
 * @param store the store to add
 * @param name name of the item
 * @param price price of said item
 */
void add_item(store_s* store, char* name, double price) {
    // we start by allocating space for a new node. The node already contains enough space for the item.
    node_t* new_node = (node_t*) malloc(sizeof(node_t));
    new_node->next = store->first_item; // we push the next element down
    new_node->item.price = price;
    // we use strcpy here because C does not support direct assignment.
    strcpy(new_node->item.name, name);
    // fix the list.
    store->first_item = new_node;
}

int main(void) {

    store_s stores[N_STORES];
    // load distances

    // load normal_prices
    // for item in normal_prices: add_item(item)

    // load discounts

    return 0;
}
