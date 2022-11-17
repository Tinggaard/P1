#include "lib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//ved ikke om den behøves at komme med ind i lib.h? det kan være at den kan være nyttig i fremtiden
//men den her kan finde antal new lines i et dokument, ved igen ikke om det er relevent men nu er det her.
int get_new_lines(char* filename){
    //tæller antal linjer i filen
    int count = 0;
    char c;
    FILE *adr;
    adr = fopen(filename, "r");
    if (adr == NULL){
        printf("Could not open file '%s'", filename);
        return 0;
    }
    for (c = getc(adr); c != EOF; c = getc(adr))
        if (c == '\n') //tæller antal new lines
            count++;
    fclose(adr);
    return count+1;
}

/**
 * load_distances() loads the distances to the stores
 * @return returns the stores in a list, as structs
 */
store_s* load_distances(void) { // read from file
    char* filename = "src/files/distances.txt";
    int numstores = get_new_lines(filename);

    FILE* distances;
    distances = fopen(filename, "r");
    if (NULL == distances) {
        printf("Could not open file '%s'", filename);
        exit(EXIT_FAILURE);
    }
    store_s* store;
    store = malloc(numstores * sizeof(store_s));
    for (int i = 0; i <= numstores; i++) {
        fscanf(distances, "%[^,], %d\n", store[i].name, &store[i].distance);
        store[i].first_item = NULL;
    }
    fclose(distances);
    return store;
}

/**
 * load_normal_prices() loads the normal prices into the store array.
 * @param stores array of stores
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
