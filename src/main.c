#include <string.h>
#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

#define N_STORES 5

int main(void) {
    store_s*  store= load_distances();

    for (int i = 0; i < 5; ++i) {
        printf("%s  ", store[i].name);
        printf("%d\n", store[i].distance);
    }

/*
    store_s stores[N_STORES];
    stores[0].distance = 10;
    strcpy(stores[0].name, "rema");
    //add_item(&stores[0], "ost", 10);
    // load distances
    load_normal_prices(stores, N_STORES);

    node_t* current_item = stores[0].first_item;
    // printf("'%s' %lf", current_item->item.name, current_item->item.price);

    int k = 0; //current_item->next != NULL
    while (current_item->next != NULL){
        printf("'%s' %lf", current_item->item.name, current_item->item.price);

        current_item = current_item->next;
        k++;
    }

    // load normal_prices
    // for item in normal_prices: add_item(item)

    // load discounts
*/
    free(store);
    printf("store´> %d", store[2].distance);
    return 0;
}
