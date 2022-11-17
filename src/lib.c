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
    return count+1;
}

store_s* load_distances(void) { // read from file
    char* filename = "src/files/distances.txt";
    int numstores =get_new_lines(filename);

    FILE* distances;
    distances = fopen(filename, "r");
    if (NULL == distances){
        exit(EXIT_FAILURE);
    }
    store_s* store;
    store = malloc(numstores * sizeof(store_s));
    for (int i = 0; i <= numstores ; i++){
        fscanf(distances, "%[^,], %d\n", store[i].name, &store[i].distance);
    }
    fclose(distances);
return store;
}


void load_normal_prices(store_s stores[], int store_count) { // read from file
    FILE* f = fopen("files/normal_prices.txt", "r");

    if (f == NULL){
        printf("File not found");
        exit(EXIT_FAILURE);
    }

    double price;
    char item_name[MAX_NAME_SIZE];
    while (1){
        if (feof(f)){
            break;
        }

        fscanf(f, "%[^,], %lf\n", item_name, &price);
        //printf("'%s' %lf\n", item_name, price);
        for (int i = 0; i < 1; i++){
            add_item(&stores[i], item_name, price);
        }
    }


    fclose(f);
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
