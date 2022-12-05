#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // Initializing and declaring strings with our txt file paths
    char shopping_list_f[] = "src/files/shopping_list.csv";
    char stores_f[] = "src/files/coordinates.csv";
    char normal_prices_f[] = "src/files/normal_prices.csv";
    char discounts_f[] = "src/files/discounts.csv";

    // Counting amount of rows from coordinates.csv and shopping_list.csv, then stores it in two variables
    int n_items = get_new_lines(normal_prices_f); // Amount of rows = amount of items in total
    int n_stores = get_new_lines(stores_f); // Amount of rows = amount of stores
    int n_shopping_list = get_new_lines(shopping_list_f); // Amount of rows = amount of shopping list items

    // Loading stores and shopping list items from txt files into struct arrays.
    store_s* stores = load_coordinates(stores_f, n_stores);
    shopping_list_s* shopping_list = load_shopping_list(shopping_list_f, n_shopping_list);

    // Connecting all items available to the different stores using associative arrays. (Normal prices are used here)
    load_normal_prices(stores, n_stores, normal_prices_f, n_items);

    // Overwrites the normal prices of items with a discount to their respective store
    load_discounts(stores, discounts_f, n_items);

    // Finds the sum of the shopping list in each store and returns it in a price sorted array of cart_item_s structs
    cart_item_s* cart_item = create_shopping_cart(stores, shopping_list, n_stores, n_shopping_list, n_items);

    calc_across_stores(cart_item, shopping_list, stores, n_stores, n_shopping_list);
    calc_per_store(cart_item, n_shopping_list, n_stores, stores);


    // Free up the memory of each array
    free(cart_item);
    for (int i = 0; i < n_stores; i++) {
        free(stores[i].item);
    }
    free(stores);
    free(shopping_list);
    return 0;
}
