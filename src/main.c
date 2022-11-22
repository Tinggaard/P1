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
    //debug shoppinglist
    //for (int i = 0; i < N_STORES; ++i) {
    //    printf("%s\n", list[i].item);
    //}

//    // debug
//    for (int i = 0; i < n_stores; ++i) {
//        printf("%s %d\n", stores[i].name, stores[i].distance);
//    }

    load_normal_prices(stores, n_stores, normal_prices_f);
    //debug normal prices
    //for (int i = 0; i < 5; i++) {
    //    node_t* current_item = stores[i].first_item;
    //    while (current_item != NULL){
    //        printf("%s: %s %lf\n", stores[i].name, current_item->item.name, current_item->item.price);
    //        current_item = current_item->next;
    //    }
    //}

    // load discounts
    load_discounts(stores, discounts_f);

    // debug
    //for (int i = 0; i < 5; i++) {
    //    node_t* current_item = stores[i].first_item;
    //    while (current_item != NULL){
    //        printf("%s: %s %lf\n", stores[i].name, current_item->item.name, current_item->item.price);
    //        current_item = current_item->next;
    //    }
   // }


   //cheapest option
    cart_item* store_c = cheapest_onestore(stores, shoppinglist, n_stores, n_items_shoppinglist);
    //debug
    for (int i = 0; i < 5; ++i) {
        printf("The cheapest store is: %f %s, %d distance\n", store_c[i].item.price, store_c[i].store.name, store_c[i].store.distance);
    }
    int temp_max_distance = 1000;
    cheapest_closest_cart(store_c, n_stores, temp_max_distance);



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
