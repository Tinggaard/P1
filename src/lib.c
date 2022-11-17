#include "lib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//ved ikke om den behøves at komme med ind i lib.h? det kan være at den kan være nyttig i fremtiden
//men den her kan finde antal new lines i et dokument, ved igen ikke om det er relevent men nu er det her.
int get_file_size(char* filename){
    //tæller antal linjer i filen
    int count = 0;
    char c;
    FILE * adr;
    adr = fopen(filename, "r");
    if (adr == NULL){
        printf("Could not open file");
        return 0;
    }
    for (c = getc(adr); c != EOF; c = getc(adr))
        if (c == '\n') //tæller antal new lines
            count++;
    fclose(adr);
    return count;
}

store_s* load_distances(void) { // read from file
    char* filename = "C:\\Users\\Maxnm\\CLionProjects\\P1\\src\\files\\distances.txt";
    int numstores =get_file_size(filename);

    FILE* distances;
    distances = fopen(filename, "r");
    if (NULL == distances){
        exit(EXIT_FAILURE);
    }
    store_s static store[5] ;
    for (int i = 0; i <= numstores ; i++){
        fscanf(distances, "%[^,], %d\n", store[i].name, &store[i].distance);
    }
    fclose(distances);
return store;
}

void load_normal_prices(void) { // read from file

}
void load_discounts(void) { // read from file

}
void load_shopping_list(void) { // read from file

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

void update_item(void) { // discounts

}
