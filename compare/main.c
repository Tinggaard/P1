#include "lib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(void) {
    // Initializing and declaring strings with our txt file paths
    char stores_f[] = "compare/files/coordinates.csv";
    char normal_prices_f[] = "compare/files/normal_prices.csv";
    char discounts_f[] = "compare/files/discounts.csv";
    char shopping_list_f[] = "compare/files/shopping_list.csv";

    // Counting amount of rows from distances.csv and shopping_list.csv, then stores it in two variables
    int n_items = get_new_lines(normal_prices_f); // Amount of rows = amount of items in total
    int n_stores = get_new_lines(stores_f); // Amount of rows = amount of stores
    int n_shopping_list = get_new_lines(shopping_list_f); // Amount of rows = amount of shopping list items



    int radius = 100000000;
    coordinates_s user_location;
    user_location.lat = 57.0139045715332;
    user_location.lon = 9.986823081970215;


    // Loading stores and shopping list items from txt files into struct arrays.
    store_s* stores = load_coordinates(stores_f, &n_stores, user_location, radius);

    shopping_list_s* shopping_list = load_shopping_list(shopping_list_f, n_shopping_list);

    // Connecting all items available to the different stores using associative arrays. (Normal prices are used here)
    load_normal_prices(normal_prices_f, stores, n_stores, n_items);

    // Overwrites the normal prices of items with a discount to their respective store
    load_discounts(discounts_f, stores, n_stores, n_items);

    // Finds the sum of the shopping list in each store and returns it in a price sorted array of cart_item_s structs
    cart_item_s* cart_item = create_shopping_cart(stores, shopping_list, n_stores, n_shopping_list, n_items);

    cart_item_s* cart = calc_across_stores(shopping_list, cart_item, n_stores, n_shopping_list);

    // begin timing
    clock_t t1, t2, t3;
    t1 = clock();
    brute_force(cart, n_shopping_list, n_stores);
    t2 = clock();
    nearest_neighbor(cart, n_shopping_list, n_stores);
    t3 = clock();
    printf("Time taken by brute force: %lfs\n", (double)(t2-t1) / CLOCKS_PER_SEC);
    printf("Time taken by nearest neighbor: %lfs\n", (double)(t3-t2) / CLOCKS_PER_SEC);


    // Free up the memory of each array
    free(cart_item);
    for (int i = 0; i < n_stores; i++) {
        free(stores[i].item);
    }
    free(stores);
    free(shopping_list);
    return 0;
}
