#include "../src/lib.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void shoppinglist_test(void);
void distances_test(void);
void normal_prices_test(void);
void discounts_test(void);

int main (int argc, char* argv[]) {

    if (argc == 1){
        printf("No arguments called\n");
        return 0;
    }

    if (strcmp(argv[1], "shoppinglist") == 0) {
        printf("Testing shoppinglist\n");
        shoppinglist_test();
        return 0;
    }

    if (strcmp(argv[1], "distances") == 0) {
        printf("Testing distances\n");
        distances_test();
        return 0;
    }

    if (strcmp(argv[1], "normal_prices") == 0) {
        printf("Testing normal prices\n");
        normal_prices_test();
        return 0;
    }

    if (strcmp(argv[1], "discounts") == 0) {
        printf("Testing discounts\n");
        discounts_test();
        return 0;
    }

    return 0;
}

void shoppinglist_test(void) {
    shopping_list_s* shoppinglist = load_shopping_list("../../test/files/shopping_list.csv");
    assert(strcmp("potatoes", shoppinglist[0].name) == 0);
    assert(strcmp("skimmed milk", shoppinglist[1].name) == 0);
    free(shoppinglist);
}

void distances_test(void) {
    store_s* stores = load_distances("../../test/files/distances.csv");
    assert(strcmp("Foetex", stores[1].name) == 0); // Foetex
    assert(stores[0].distance == 1); // Rema
    assert(stores[0].first_item == NULL); // Rema
    free(stores);
}

void normal_prices_test(void) {
    store_s* stores = load_distances("../../test/files/distances.csv");
    load_normal_prices(stores, 2, "../../test/files/normal_prices.csv");
    assert(strcmp(stores[1].first_item -> item.name, "skimmed milk") == 0); // Foetex, skimmed milk
    assert(stores[0].first_item -> next -> item.price == 1); // Rema, vegetables
    for (int i = 0; i < 2; i++) {
        deallocate_list(&stores[i]);
    }
    free(stores);
}

void discounts_test(void) {
    store_s* stores = load_distances("../../test/files/distances.csv");
    load_normal_prices(stores, 2, "../../test/files/normal_prices.csv");
    load_discounts(stores, "../../test/files/discounts.csv");
    assert(stores[1].first_item -> next -> item.price == 0.1); // Foetex, skimmed milk
    for (int i = 0; i < 2; i++) {
        deallocate_list(&stores[i]);
    }
    free(stores);
}
