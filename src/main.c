#include <string.h>
#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

#define N_STORES 5

int main(void) {
    store_s* stores = load_distances();

//    // debug
//    for (int i = 0; i < N_STORES; ++i) {
//        printf("%s %d\n", stores[i].name, stores[i].distance);
//    }

    load_normal_prices(stores, N_STORES);

//    // debug
//    for (int i = 0; i < 5; i++) {
//        node_t* current_item = stores[i].first_item;
//        while (current_item != NULL){
//            printf("%s: %s %lf\n", stores[i].name, current_item->item.name, current_item->item.price);
//            current_item = current_item->next;
//        }
//    }

    // load normal_prices
    // for item in normal_prices: add_item(item)

    // load discounts

    free(stores);

    return 0;
}
