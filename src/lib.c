#include "lib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * compare function for qsort
 * @param ptr1 item 1
 * @param ptr2 item 2
 * @return returns negative if item 2 is greater, positive if item 1 is greater and 0 if they are equal
 */
int compare_cart(const void* ptr1, const void* ptr2) {
    const cart_item* item_1 = ptr1;
    const cart_item* item_2 = ptr2;
    if (item_1->item.price - item_2->item.price == 0) {
        return item_1->store.distance - item_2->store.distance;
    }
    return (int)(item_1->item.price - item_2->item.price);
}

/**
 * open_file() returns a pointer to a filestream
 * @param filename file to open
 * @return pointer to FILE
 */
FILE* open_file(char filename[]) {
    FILE *f = fopen(filename, "r");
    // Checks to see if the file was found or not
    if (f == NULL){ // if file cant be opened
        printf("Could not open file '%s'", filename);
        exit(EXIT_FAILURE);
    }
    return f;
}

/**
 * get_new_lines() counts number of lines in a given file
 * @param filename file to parse
 * @return number of lines is given file
 */
int get_new_lines(char* filename){
    int count = 1; //initializes 1 because if there's no new line, there is still one line
    char c;
    FILE *f = open_file(filename);
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

    FILE *f = open_file(filename);

    // Allocates space in the heap for all the stores that are now being collected in a dynamic store_s struct array
    store_s* store = malloc(store_count * sizeof(store_s));

    // Scans all stores and parses their values to an index in the struct array
    for (int i = 0; i < store_count; i++) {
        fscanf(f, "%[^,], %d\n", store[i].name, &store[i].distance);
        store[i].first_item = NULL;
    }
    fclose(f);
    return store;
}

/**
 * load_normal_prices() loads the normal prices into the store array.
 * @param stores array of store_s
 * @param store_count amount of stores
 */
void load_normal_prices(store_s *stores, int store_count, char filename[]) {
    FILE *f = open_file(filename);

    double price;
    char item_name[MAX_NAME_SIZE];
    // Runs over all rows in the file until it reaches the end of the file
    while (!feof(f)) {
        // Scans for the name and price of an item in given row, including spaces in the name.
        fscanf(f, "%[A-Za-z ], %lf\n", item_name, &price);
        // Adds it to all stores
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
    FILE *f = open_file(filename);

    char current_store[MAX_NAME_SIZE];
    char current_item[MAX_NAME_SIZE];
    double current_price;
    int i;
    // Runs over all rows in the file until it reaches the end of the file
    while (!feof(f)) {
        i = 0;
        // Scans for store name, item name and the price of an item. Excludes commas.
        fscanf(f, "%[^,], %[^,], %lf\n", current_store, current_item, &current_price);

        // Gets the index of the store that matches the currently scanned one
        while (strcmp(stores[i].name, current_store)) {
            i++;
        }

        node_t* item = stores[i].first_item; // Gets the first item in the store
        // Searches through all the store's items until it reaches the current item we want to add a discount on
        while (strcmp(item->item.name, current_item)) {
            item = item->next;
        }
        item->item.price = current_price; // Replaces the normal price with the discount
    }
    fclose(f);
}
/**
 * load_shopping_list() loads all shopping list items into a struct array
 * @param filename file to parse
 * @return returns a shopping_list_s array of all items from the users shopping list
 */
shopping_list_s* load_shopping_list(char filename[]) {
    int number_of_items = get_new_lines(filename);

    FILE *f = open_file(filename);

    // Allocates space in the heap for all the shopping list items
    shopping_list_s* s_list = malloc(number_of_items * sizeof(shopping_list_s));

    // Scans all item names into a shopping_list_s array
    for (int i = 0; i <= number_of_items; i++) {
        fscanf(f, "%[^\n]\n",s_list[i].name);
    }
    fclose(f);
    return s_list; // Returns the array of all the items on the shopping list
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

/**
 * create_shopping_cart() generates a shopping cart for all stores with all the items from the users shopping list.
 * @param stores Struct array of all stores.
 * @param shopping_list Struct array of the users shopping list.
 * @param n_stores Amount of stores.
 * @param n_shopping_list Amount of items on the shopping list
 * @return Returns the final cart including all shopping list items in each store.
 */
cart_item* create_shopping_cart(store_s* stores, shopping_list_s* shopping_list, int n_stores, int n_shopping_list){

    // We start by allocating space for the carts.
    cart_item* cart = malloc(n_shopping_list * n_stores * sizeof(cart_item));
    int cart_index = 0; // Index to identify where in the cart we are adding an item

    // Iterates over all stores
    for (int i = 0; i < n_stores; i++) {
        node_t* current_item = stores[i].first_item; // Initializes a current item in the store
        // Iterate every item in the store
        while (current_item != NULL) {
            // Iterate every item in the shopping list to see if it matches the current item in the store
            for (int j = 0; j < n_shopping_list; ++j) {
                // If the current item match the item from the shopping list we add it to the cart
                if (strcmp(current_item->item.name, shopping_list[j].name) == 0) {
                    strcpy(cart[cart_index].store.name, stores[i].name);
                    cart[cart_index].store.distance = stores[i].distance;
                    strcpy(cart[cart_index].item.name,current_item->item.name);
                    cart[cart_index].item.price = current_item->item.price;
                    cart_index++; // Updates the cart index to avoid overwriting already added items
                }
            }
            current_item = current_item->next; // Updates the current item to the next item in the store
        }
    }
    return cart;
}

void cheapest_overall_cart(void){}
