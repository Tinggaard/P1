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
void load_discounts(store_s *stores, char filename[]) {
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

shoppinglist_s* load_shopping_list(char filename[]) {
    int numberofitems = get_new_lines(filename);

    FILE* f;
    f = fopen(filename, "r");
    if(f == NULL){
        exit(EXIT_FAILURE);
    }

     shoppinglist_s * s_list;
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


void sum_shoppinglist(store_s *stores, shoppinglist_s *shoppinglist, int n_stores, int n_shoppinglist){
    double sum[n_shoppinglist];

    for (int i = 0; i < n_stores; ++i) {
        double temp_sum = 0;
        node_t *current_item = stores[i].first_item;
        while (current_item != NULL) {
            for (int j = 0; j < n_shoppinglist; j++) { //shoppinglist
                if (strcmp(current_item->item.name, shoppinglist[j].name) == 0) {
                    //printf("%s %f\n", shoppinglist[j].name, current_item->item.price);
                    //printf("%.3f\n", current_item->item.price);
                    temp_sum = temp_sum + current_item->item.price;
                }
            }
            sum[i] = temp_sum;
            current_item = current_item->next;
        }
    }
    //for (int i = 0; i < 5; ++i) {
    //    printf("%s %f\n",stores[i].name, sum[i]);
    //}
}

void cheapest_overall_cart(void){}

void cheapest_closest_cart(void){}

void cheapest_onestore(void){}