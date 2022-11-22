#include "lib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int compare_prices(const void* a, const void* b) {
    const cart_item* c = a;
    const cart_item* d = b;
    return (c->item.price- d->item.price);
}

int compare_distances(const void* a, const void* b) {
    const cart_item* c = a;
    const cart_item* d = b;
    return (c->store.distance- d->store.distance);
}
/**
 * get_new_lines() counts number of lines in a given file
 * @param filename file to parse
 * @return number of lines is given file
 */
int get_new_lines(char* filename){
    int count = 1; //initializes 1 because if there's no new line, there is still one line
    char c;
    FILE *f;
    f = fopen(filename, "r");
    if (f == NULL){ //if file cant be opened
        printf("Could not open file '%s'", filename);
        exit(EXIT_FAILURE);
    }
    for (c = getc(f); c != EOF; c = getc(f))
        if (c == '\n') //Everytime the char c is \n count is increased
            count++;
    fclose(f);
    return count;
}

/**
 * load_distances() loads the distances to the stores
 * @return returns stores in an array, as structs (store_s)
 */
store_s* load_distances(char filename[]) {
    int store_count = get_new_lines(filename);

    FILE* distances;
    distances = fopen(filename, "r");
    if (NULL == distances) {
        printf("Could not open file '%s'", filename);
        exit(EXIT_FAILURE);
    }
    store_s* store = malloc(store_count * sizeof(store_s));
    for (int i = 0; i < store_count; i++) {
        fscanf(distances, "%[^,], %d\n", store[i].name, &store[i].distance);
        store[i].first_item = NULL;
    }
    fclose(distances);
    return store;
}

/**
 * load_normal_prices() loads the normal prices into the store array.
 * @param stores array of store_s
 * @param store_count amount of stores
 */

void load_normal_prices(store_s *stores, int store_count, char filename[]) {
    FILE* f = fopen(filename, "r");

    if (f == NULL){
        printf("Could not open file '%s'", filename);
        exit(EXIT_FAILURE);
    }

    double price;
    char item_name[MAX_NAME_SIZE];
    while (!feof(f)) {
        fscanf(f, "%[A-Za-z ], %lf\n", item_name, &price);
        // add it to all stores
        for (int i = 0; i < store_count; i++){
            add_item(&stores[i], item_name, price);
        }
    }
    fclose(f);
}

/**
 * load_discounts() overrides the discounts to their respective stores
 * @param stores array of store_s
 */
void load_discounts(store_s* stores, char filename[]) {
    FILE* f = fopen(filename, "r");

    if (f == NULL){
        printf("Could not open file '%s'", filename);
        exit(EXIT_FAILURE);
    }

    char current_store[MAX_NAME_SIZE];
    char current_item[MAX_NAME_SIZE];
    double current_price;
    int i;

    while (!feof(f)) {
        i = 0;
        fscanf(f, "%[^,], %[^,], %lf\n", current_store, current_item, &current_price);

        // get index of store
        while (strcmp(stores[i].name, current_store)) {
            i++;
        }
        // get item in list
        node_t* item = stores[i].first_item;
        while (strcmp(item->item.name, current_item)) {
            item = item->next;
        }
        item->item.price = current_price;
    }
    fclose(f);

}

shoppinglist_s* load_shopping_list(char filename[]) {
    int numberofitems = get_new_lines(filename);

    FILE* f;
    f = fopen(filename, "r");
    if(f == NULL){
        exit(EXIT_FAILURE);
    }

     shoppinglist_s* s_list;
     s_list = malloc(numberofitems * sizeof(shoppinglist_s));

     for (int i = 0; i <= numberofitems; i++) {
         fscanf(f, "%[^\n]\n",s_list[i].name);
     }
     fclose(f);
     return s_list;
}

/**
 * add_item is used for declaring which items exist in the store
 * @param store the store_s to add
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

/**
 * deallocate_list() deallocates the memory used by items
 * @param store store_s to deallocate
 */
void deallocate_list(store_s* store) {
    node_t* current = store->first_item;
    while(current != NULL) // we know that the NULL pointer signals the end of the list
    {
        node_t* next = current->next; // we need to copy out the value "next" before we deallocate!
        free(current);
        current = next; // move to the next
    }
    // remember to reset the list pointer
    store->first_item = NULL;
}


cart_item* cheapest_onestore(store_s* stores, shoppinglist_s* shoppinglist, int n_stores, int n_shoppinglist){
    double sum[n_stores];

    // iterate the stores
    for (int i = 0; i < n_stores; ++i) {
        sum[i] = 0;
        node_t* current_item = stores[i].first_item; // initialize item

        while (current_item != NULL) { // iterate every item in the store
            for (int j = 0; j < n_shoppinglist; j++) { // iterate items in shoppinglist

                // if the item is in the shoppinglist
                if (strcmp(current_item->item.name, shoppinglist[j].name) == 0) {
                    sum[i] += current_item->item.price; // add price of the item
                }
            }
            current_item = current_item->next;
        }
    }

    cart_item* store_c = malloc(n_stores * sizeof(cart_item));

    for (int i = 0; i < n_stores; i++) {
        store_c[i].item.price = sum[i]; //copies the item price from the array
        strcpy(store_c[i].store.name, stores[i].name); //copies the name of the stores into the new struct
        store_c[i].store.distance = stores[i].distance; //copies the distances int of the new struct
    }

    qsort(store_c, n_shoppinglist, sizeof(cart_item), compare_prices);

    return store_c;
}

void cheapest_overall_cart(void){}

void cheapest_closest_cart(cart_item* cartitems, int n_stores, int max_distance){
    //get cartiems from cheapest_onestore, sort after distance, make ranking
    qsort(cartitems, n_stores, sizeof(cart_item), compare_prices);
    qsort(cartitems, n_stores, sizeof(cart_item), compare_distances);
//    for (int i = 0; i < 5; ++i) {
//        printf("The cheapest store is: %f %s, %d distance\n", cartitems[i].item.price, cartitems[i].item.name, cartitems[i].store.distance);
//    }

}

