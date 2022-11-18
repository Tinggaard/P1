#include "lib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * get_new_lines() counts number of lines in a given file
 * @param filename file to parse
 * @return number of lines is given file
 */
int get_new_lines(char* filename){
    int count = 0;
    char c;
    FILE *f;
    f = fopen(filename, "r");
    if (f == NULL){
        printf("Could not open file '%s'", filename);
        exit(EXIT_FAILURE);
    }
    for (c = getc(f); c != EOF; c = getc(f))
        if (c == '\n') // counts new lines
            count++;
    fclose(f);
    return count+1;
}

/**
 * load_distances() loads the distances to the stores
 * @return returns stores in an array, as structs (store_s)
 */
store_s* load_distances(void) { // read from file
    char* filename = "src/files/distances.txt";
    int store_count = get_new_lines(filename);

    FILE* distances;
    distances = fopen(filename, "r");
    if (NULL == distances) {
        printf("Could not open file '%s'", filename);
        exit(EXIT_FAILURE);
    }
    store_s* store;
    store = malloc(store_count * sizeof(store_s));
    for (int i = 0; i <= store_count; i++) {
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

void load_normal_prices(store_s stores[], int store_count) { // read from file
    char filename[] = "src/files/normal_prices.txt";
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
void load_discounts(store_s stores[]) { // read from file
    char filename[] = "src/files/discounts.txt";
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
        node_t *item = stores[i].first_item;
        while (strcmp(item->item.name, current_item)) {
            item = item->next;
        }
        item->item.price = current_price;
    }
    fclose(f);

}

shoppinglist* load_shopping_list(void) { // read from file
    char* filename = "src/files/shopping_list.txt";
    int numberofitems = get_new_lines(filename);

    FILE* f;
    f = fopen(filename, "r");
    if(f == NULL){
        exit(EXIT_FAILURE);
    }

     shoppinglist * s_list;
     s_list = malloc(numberofitems * sizeof(shoppinglist));

     for (int i = 0; i <= numberofitems; i++) {
         fscanf(f, "%s\n",s_list[i].item);
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
