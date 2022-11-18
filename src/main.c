#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

#define N_STORES 5

int main(void) {
    char* filename = "src/files/distances.txt";
    int n_stores = get_new_lines(filename);
    //printf("%d", n_stores);
    store_s* stores = load_distances();
    shoppinglist* list = load_shopping_list();
    //debug shoppinglist
    //for (int i = 0; i < N_STORES; ++i) {
    //    printf("%s\n", list[i].item);
    //}

//    // debug
//    for (int i = 0; i < n_stores; ++i) {
//        printf("%s %d\n", stores[i].name, stores[i].distance);
//    }

    load_normal_prices(stores, n_stores);
    //debug normal prices
    //for (int i = 0; i < 5; i++) {
    //    node_t* current_item = stores[i].first_item;
    //    while (current_item != NULL){
    //        printf("%s: %s %lf\n", stores[i].name, current_item->item.name, current_item->item.price);
    //        current_item = current_item->next;
    //    }
    //}

    // load discounts
    load_discounts(stores);

    // debug
    //for (int i = 0; i < 5; i++) {
    //    node_t* current_item = stores[i].first_item;
    //    while (current_item != NULL){
    //        printf("%s: %s %lf\n", stores[i].name, current_item->item.name, current_item->item.price);
    //        current_item = current_item->next;
    //    }
   // }


    // free up all items in each store
    for (int i = 0; i < 5; i++) {
        deallocate_list(&stores[i]);
    }
    // free up the store array
    free(stores);
    free(list);
    return 0;
}
