#include <string.h>
#include "lib.h"
#include <stdio.h>

#define N_STORES 5

int main(void) {
    store_s*  store= load_distances();

    printf("%s", store[0].name);
    printf("%d", store[0].distance);

/*
    store_s stores[N_STORES];
    stores[0].distance = 10;
    strcpy(stores[0].name, "rema");
    add_item(&stores[0], "ost", 10);
    // load distances

    // load normal_prices
    // for item in normal_prices: add_item(item)

    // load discounts
*/
    return 0;
}
