#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {

    char shoppinglist_f[] = "src/files/shopping_list.txt";
    char stores_f[] = "src/files/distances.txt";
    char normal_prices_f[] = "src/files/normal_prices.txt";
    char discounts_f[] = "src/files/discounts.txt";

    int n_stores = get_new_lines(stores_f);
    int n_items_shoppinglist = get_new_lines(shoppinglist_f);

    store_s* stores = load_distances(stores_f);
    shoppinglist_s* shoppinglist = load_shopping_list(shoppinglist_f);

    //debug normal
    load_normal_prices(stores, n_stores, normal_prices_f);

    // load discounts
    load_discounts(stores, discounts_f);

    // cheapest option
    cart_item* store_c = cheapest_onestore(stores, shoppinglist, n_stores, n_items_shoppinglist);


    // free up all items in each store
    for (int i = 0; i < 5; i++) {
        deallocate_list(&stores[i]);
    }
    // free up the store array
    free(stores);
    free(shoppinglist);
    free(store_c);
    return 0;
}
