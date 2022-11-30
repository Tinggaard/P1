#include "lib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


/**
 * compare function for qsort
 * @param ptr1 item 1
 * @param ptr2 item 2
 * @return returns negative if item 2 is greater, positive if item 1 is greater and 0 if they are equal
 */
int compare_cart(const void* ptr1, const void* ptr2) {
    const cart_sum_s* item_1 = ptr1;
    const cart_sum_s* item_2 = ptr2;
    if (item_1->sum - item_2->sum == 0) {
        return (int)(calc_base_to_store(item_1->store) - calc_base_to_store(item_2->store));
    }
    return (int)(item_1->sum - item_2->sum);
}
int compare_item_names(const void* ptr1, const void* ptr2){
    const item_s* item_1 = ptr1;
    const item_s* item_2 = ptr2;
    return strcmp(item_1->name, item_2->name);
}

int binary_search(item_s itemlist[], const char x[], int n_items){
    int l, r, m;
    l=0;
    r=n_items-1;
    while (l<r){
        m = (r+l)/2;

        if (0>strcmp(itemlist[m].name, x)){
            l=m+1;
        } else r=m;
    }
    if (strcmp(x, itemlist[l].name) == 0){
        return l;
    } else
        return -1;
}


/**
 * copy_coord() is a helper function to copy the coordinates from one store_s to another
 * @param target the store_s to copy into
 * @param base the store_s to copy from
 */
void copy_coord(store_s* target, store_s* base){
    // Copy coordinates into cart_item_s
    target->store_coord.lat = base->store_coord.lat;
    target->store_coord.lon = base->store_coord.lon;
    target->base_coord.lat = base->base_coord.lat;
    target->base_coord.lon = base->base_coord.lon;
}


/**
 * distance_user_to_stares() calculates the distance between base and store coordinates
 * @param store the store to calculate on
 * @return the distance in meters (double)
 */
// courtesy of https://stackoverflow.com/questions/27126714/c-latitude-and-longitude-distance-calculator
int calc_base_to_store(store_s store){
    double EARTH_RADIUS = 6372797.56085; // in meters
    double DEGREES_RADIANS = M_PI / 180; // from degrees to radians

    double haversine, arcsin, distance, sin_lat, cos_lat, lon, minimum;

    // we convert degrees to radians
    double lat1 = store.store_coord.lat * DEGREES_RADIANS;
    double lon1 = store.store_coord.lon * DEGREES_RADIANS;
    double lat2 = store.base_coord.lat * DEGREES_RADIANS;
    double lon2 = store.base_coord.lon * DEGREES_RADIANS;

    // haversine formula
    sin_lat = pow(sin(0.5 * (lat2 - lat1)), 2);
    cos_lat = cos(lat1) * cos(lat2);
    lon = pow(sin(0.5 * (lon2 - lon1)), 2);
    haversine = sin_lat+ (cos_lat* lon);

    minimum = sqrt(haversine) < 1.0 ? sqrt(haversine) : 1.0;
    arcsin = 2 * asin(minimum);
    distance = EARTH_RADIUS * arcsin;

    return (int) distance;
}


/**
* calc_cheapest_cart_item() compares a shopping list item with a cart item and swaps them, if they match and the cart item is cheaper.
* @param cart cart to evaluate items from
* @param current_item the item to compare with the the item in the cart
* @param cart_index index for where we are in the cart
* @return returns the cheapest item of the two compared, if the parsed cart item had the same name as the parsed current_item
*/
cart_item_s calc_cheapest_cart_item(cart_item_s cart[], cart_item_s current_item, int cart_index) {
    // First it check if the two parsed items have the same name, if not, it just returns the current_item that was parsed
    if (strcmp(current_item.item.name, cart[cart_index].item.name) == 0) {

        // Checks to see if the price is 0
        if (current_item.item.price == 0) {
            // if it is 0, it knows that this current_item has not yet been initialized with any values but a name and therefore transfers the first match's values
            current_item.item.price = cart[cart_index].item.price;
            strcpy(current_item.store.name, cart[cart_index].store.name);
            copy_coord(&current_item.store, &cart[cart_index].store);
        }

        // Checks to see if the two items have the same price. if they have the same price, it takes the closest store
        if (current_item.item.price == cart[cart_index].item.price &&
            calc_base_to_store(current_item.store) > calc_base_to_store(cart[cart_index].store)) {
            current_item.item.price = cart[cart_index].item.price;
            strcpy(current_item.store.name, cart[cart_index].store.name);
            copy_coord(&current_item.store, &cart[cart_index].store);
        }

        //  Compare prices and transfers the cheapest option to the current item
        if (current_item.item.price > cart[cart_index].item.price) {
            current_item.item.price = cart[cart_index].item.price;
            strcpy(current_item.store.name, cart[cart_index].store.name);
            copy_coord(&current_item.store, &cart[cart_index].store);
        }
    }
    return current_item; // Returns the cheapest option
}


/**
 * open_file() returns a pointer to a filestream
 * @param filename file to open
 * @return pointer to FILE
 */
FILE* open_file(char filename[]) {
    FILE* f = fopen(filename, "r");
    // Checks to see if the file was found or not
    if (f == NULL){ // if file cant be opened
        printf("Could not open file '%s'", filename);
        exit(EXIT_FAILURE);
    }
    return f;
}


/**
 * compares distance and name, so items from the same store are grouped
 * @param ptr1 pointer to first item
 * @param ptr2 pointer to second item
 * @return returns negative if item 2 is greater, positive if item 1 is greater and 0 if they are equal
 */
/*
int compare_name_distance(const void* ptr1, const void* ptr2) {
   const cart_item_s* item_1 = ptr1;
   const cart_item_s* item_2 = ptr2;
   if (item_1->store.distance - item_2->store.distance == 0) { // if the distances are the same, we check the name
       return (int)(item_1->store.name - item_2->store.name);
   }
   return (int)(item_1->store.distance - item_2->store.distance); // return closest store.
}
*/


/**
 * get_new_lines() counts number of lines in a given file
 * @param filename file to parse
 * @return number of lines is given file
 */
int get_new_lines(char filename[]){
    int count = 1; // initializes 1 because if there's no new line, there is still one line
    char c;
    FILE* f = open_file(filename);
    for (c = getc(f); c != EOF; c = getc(f))
        if (c == '\n') // Every time the char c is \n count is increased
            count++;
    fclose(f);
    return count;
}


/**
 * load_distances() loads the distances to the stores
 * @param filename path to distances.txt file
 * @param n_stores number of stores
 * @return returns stores in an array, as structs (store_s)
 */
store_s* load_distances(char filename[], int n_stores) {
    FILE* f = open_file(filename);

    // Allocates space in the heap for all the stores that are now being collected in a dynamic store_s struct array
    store_s* store = malloc(n_stores * sizeof(store_s));

    // Scans all stores and parses their values to an index in the struct array
    for (int i = 0; i < n_stores; i++) {
        fscanf(f, "%[^,], %lf, %lf\n", store[i].name, &store[i].store_coord.lat, &store[i].store_coord.lon);
        store[i].item = NULL;
        store[i].base_coord.lat = 57.0139045715332;
        store[i].base_coord.lon = 9.986823081970215;
    }
    fclose(f);
    return store;
}


/**
 * load_normal_prices() loads the normal prices into the store array.
 * @param stores array of store_s
 * @param n_stores amount of stores
 * @param filename path to normal_prices.txt
 * @param n_items number of items
 */
void load_normal_prices(store_s stores[], int n_stores, char filename[], int n_items) {
    FILE* f = open_file(filename);

    // allocate space enough for the amount of items
    item_s* normal_prices = malloc(sizeof(item_s) * n_items);

    for (int i = 0; i < n_items; i++) {
        // get each of the items
        fscanf(f, "%[^,], %lf\n", normal_prices[i].name, &normal_prices[i].price);
    }
    fclose(f); // remember to close the file

    qsort(normal_prices, n_items, sizeof(item_s), compare_item_names);
    binary_search(normal_prices, "cheese", n_items);

    // allocate out layer of 2d array, the size of n_stores
    item_s** items = malloc(sizeof(item_s) * n_stores);
    for (int i = 0; i < n_stores; i++) {
        // allocate inner layer, for each store, with the size of items
        items[i] = malloc(sizeof(*items[i]) * n_items);
        // set the pointer of store_s.items to this array
        stores[i].item = items[i];
        for (int j = 0; j < n_items; j++) {
            // here we copy the values over (name and price)
            strcpy(stores[i].item[j].name, normal_prices[j].name);
            stores[i].item[j].price = normal_prices[j].price;
        }
    }

    // the first array can now be freed, as the data is in the store_s array
    free(normal_prices);
}


/**
 * load_discounts() overrides the discounts to their respective stores
 * @param stores array of store_s
 * @param filename path to discounts.txt file
 */
void load_discounts(store_s stores[], char filename[]) {
    FILE* f = open_file(filename);

    char current_store[MAX_NAME_SIZE];
    char current_item[MAX_NAME_SIZE];
    double current_price;
    int i;
    int j;
    // Runs over all rows in the file until it reaches the end of the file
    while (!feof(f)) {
        i = 0;
        j = 0;
        // Scans for store name, item name and the price of an item. Excludes commas.
        fscanf(f, "%[^,], %[^,], %lf\n", current_store, current_item, &current_price);

        // Gets the index of the store that matches the currently scanned one
        while (strcmp(stores[i].name, current_store)) {
            i++;
        }

        // Searches through all the store's items until it reaches the current item we want to add a discount on
        while (strcmp(stores[i].item[j].name, current_item)) {
            j++;
        }
        stores[i].item[j].price = current_price; // Replaces the normal price with the discount
    }
    fclose(f);
}


/**
 * load_shopping_list() loads all shopping list items into a struct array
 * @param filename file to parse
 * @return returns a shopping_list_s array of all items from the users shopping list
 */
shopping_list_s* load_shopping_list(char filename[], int n_shopping_list) {
    FILE* f = open_file(filename);

    // Allocates space in the heap for all the shopping list items
    shopping_list_s* s_list = malloc(n_shopping_list * sizeof(shopping_list_s));

    // Scans all item names into a shopping_list_s array
    for (int i = 0; i <= n_shopping_list; i++) {
        fscanf(f, "%[^\n]\n",s_list[i].name);
    }
    fclose(f);
    return s_list; // Returns the array of all the items on the shopping list
}


/**
 * create_shopping_cart() generates a shopping cart for all stores with all the items from the users shopping list.
 * @param stores Struct array of all stores.
 * @param shopping_list Struct array of the users shopping list.
 * @param n_stores Amount of stores.
 * @param n_shopping_list Amount of items on the shopping list
 * @return Returns the final cart including all shopping list items in each store.
 */
cart_item_s* create_shopping_cart(store_s stores[], shopping_list_s shopping_list[], int n_stores, int n_shopping_list, int n_items){
    // We start by allocating space for the carts.
    cart_item_s* cart = malloc(n_shopping_list * n_stores * sizeof(cart_item_s));
    int cart_index = 0; // Index to identify where in the cart we are adding an item
    // Iterates over all stores
    item_s* current_item; // Initializes a current item in the store
    for (int i = 0; i < n_stores; i++) {
        for (int j = 0; j < n_items; j++) {
            // Iterate every item in the shopping list to see if it matches the current item in the store
            for (int k = 0; k < n_shopping_list; k++) {
                // If the current item match the item from the shopping list we add it to the cart
                if (strcmp(stores[i].item[j].name, shopping_list[k].name) == 0) {
                    strcpy(cart[cart_index].store.name, stores[i].name);

                    // Copy coordinates into cart_item_s
                    copy_coord(&cart[cart_index].store, &stores[i]);

                    strcpy(cart[cart_index].item.name, stores[i].item[j].name);
                    cart[cart_index].item.price = stores[i].item[j].price;
                    cart_index++; // Updates the cart index to avoid overwriting already added items
                    break; // Breaks out of the for loop when the item has been found, so we don't iterate over unnecessary items
                }
            }
        }
    }
    return cart;
}


/**
 * a function that finds the cheapest possible combination of a cart across all stores.
 * @param cart contain all shopping list items in every store
 * @param shopping_list array of item names
 * @param n_stores amount of stores
 * @param n_shopping_list amount of items in our list
 * @return returns the final cart for which items are cheapest in which stores
 */
void calc_across_stores(cart_item_s cart[], shopping_list_s shopping_list[], int n_stores, int n_shopping_list){
    // Allocates space in the heap for each cart per item in the shopping list
    cart_item_s* cart_across = malloc(n_shopping_list * sizeof(cart_item_s));
    cart_item_s current_item;
    // iterate all the items in the shopping list
    for (int shopping_list_index = 0; shopping_list_index < n_shopping_list; ++shopping_list_index) {
        // initialize the name of our current item to the indexed item in our shopping list
        strcpy(current_item.item.name, shopping_list[shopping_list_index].name);
        current_item.item.price = 0;
        // iterates over all cart items
        for (int cart_index = 0; cart_index < n_shopping_list * n_stores; ++cart_index) {
            current_item = calc_cheapest_cart_item(cart, current_item, cart_index);
        }
        // makes a copy of current_item in cart_across.
        strcpy(cart_across[shopping_list_index].item.name, current_item.item.name);
        cart_across[shopping_list_index].item.price = current_item.item.price;
        strcpy(cart_across[shopping_list_index].store.name, current_item.store.name);
        copy_coord(&cart_across[shopping_list_index].store, &current_item.store);

    }
    // we sort the list, so items from same shop are printed together
    // TODO: IMPLEMENT THIS
//    qsort(cart_across,n_shopping_list, sizeof(cart_item_s),compare_name_distance);

    double total_sum = 0;
    printf("Your absolute cheapest customized shopping list\n");
    printf("| Store    | Distance | Item |           Price    |\n");
    for (int i = 0; i < n_shopping_list; ++i) { // calculates the sum of item prices
        printf("%-15s %*dm %-15s %7.2lf DKK \n",cart_across[i].store.name,
               5, calc_base_to_store(cart_across[i].store), cart_across[i].item.name, cart_across[i].item.price);
        total_sum += cart_across[i].item.price;
    }
    printf("\nTotal %40.2lf DKK\n",total_sum);
}


/**
 * print_cart_sum_per_store() prints a sorted list for the cheapest option to shop in
 * @param cart_item an array of cart_item
 * @param n_shopping_list number of items in shopping list
 * @param n_stores number of stores
 * @param stores array of stores
 * @return returns an array of cart_sum, one for each store
 */
void calc_per_store(cart_item_s cart_item[], int n_shopping_list, int n_stores, store_s* stores) {
    double sum[n_stores];

    for (int i = 0; i < n_stores; i++) {
        for (int j = 0; j < n_shopping_list; j++) {
            sum[i] += cart_item[i*n_shopping_list+j].item.price;
        }
    }

    cart_sum_s* cart = malloc(n_stores * sizeof(cart_sum_s));

    for (int i = 0; i < n_stores; i++) {
        cart[i].sum = sum[i]; //copies the item price from the array
        strcpy(cart[i].store.name, stores[i].name); //copies the name of the stores into the new struct
        copy_coord(&cart[i].store, &stores[i]);
    }

    qsort(cart, n_stores, sizeof(cart_sum_s), compare_cart);

    //print function
    for (int i = 0; i < n_stores; ++i) {
        printf("|Name > %8s : Distance > %4d : Total sum > %4.2lf|\n", cart[i].store.name,
               calc_base_to_store(cart[i].store), cart[i].sum);
    }
}
