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


/**
 * compare_store_name() used to sort cart_item_s based on store names
 * @param ptr1 first cart_item_s
 * @param ptr2 second cart_item_s
 * @return -1, 0 or 1 based on difference
 */
int compare_store_name(const void* ptr1, const void* ptr2) {
    const cart_item_s* store_1 = ptr1;
    const cart_item_s* store_2 = ptr2;
    return strcmp(store_1->store.name, store_2->store.name);
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

int calc_distance(coordinates_s cord_base, coordinates_s cord_dest){
    double EARTH_RADIUS = 6372797.56085; // in meters
    double DEGREES_RADIANS = M_PI / 180; // from degrees to radians

    double haversine, arcsin, distance, sin_lat, cos_lat, lon, minimum;

    // we convert degrees to radians
    double lat1 = cord_dest.lat * DEGREES_RADIANS;
    double lon1 = cord_dest.lon * DEGREES_RADIANS;
    double lat2 = cord_base.lat * DEGREES_RADIANS;
    double lon2 = cord_base.lon * DEGREES_RADIANS;

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
            // if it is 0, it knows that this current_item has not yet been initialized with any values but a name
            // and therefore transfers the first match's values
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
 * a function that finds the cheapest possible comb of a cart across all stores.
 * @param cart contain all shopping list items in every store
 * @param shopping_list array of item names
 * @param n_stores amount of stores
 * @param n_shopping_list amount of items in our list
 * @return returns the final cart for which items are cheapest in which stores
 */
void calc_across_stores(cart_item_s cart[], shopping_list_s shopping_list[], store_s store[], int n_stores, int n_shopping_list){
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
    // Calculates the shortest path and prints the new shopping list
    shortest_path(cart_across, n_shopping_list, n_stores);
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
    for (int i = 0; i < n_stores ; ++i) { //initializere til 0
        sum[i] = 0;
    }

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


/**
 * shortest_path calculates the shortest path and outputs the new shopping list
 * @param cart_across The cart with the cheapest option across stores
 * @param store Array of stores
 * @param n_shopping_list amount of items in shopping list
 * @param n_stores amount of stores
 */
void shortest_path(cart_item_s cart_across[], int n_shopping_list, int n_stores) {
    // sort the array, according to store names
    qsort(cart_across, n_shopping_list, sizeof(cart_item_s), compare_store_name);

    int n_locations = 1; // minimum 1 store
    // need to make this n_stores long, as we might only have 1 item in each store
    cart_item_s stores_to_visit[n_stores];
    cart_item_s* index_of_first[n_stores];
    int items_per_store[n_stores];

    // create first index manually
    strcpy(stores_to_visit[0].store.name, cart_across[0].store.name);
    copy_coord(&stores_to_visit[0].store, &cart_across[0].store);
    index_of_first[0] = &cart_across[0];
    items_per_store[0] = 0;
    // Creates the chars in the array with only of the stores you have to visit
    for (int i = 1; i < n_shopping_list; i++) {
        // if this store is not seen before we add it
        if (strcmp(cart_across[i].store.name, stores_to_visit[n_locations-1].store.name) != 0){
            strcpy(stores_to_visit[n_locations].store.name, cart_across[i].store.name);
            copy_coord(&stores_to_visit[n_locations].store, &cart_across[i].store);
            index_of_first[n_locations] = &cart_across[i];
            items_per_store[n_locations] = 0;
            n_locations++;
        }
        // add another item to this store (index -1 because indexes start at 0)
        items_per_store[n_locations-1]++;
    }


    // Values for finding all possible combinations
    int comb[n_locations];
    int temp;
    coordinates_s current_dest;
    coordinates_s current_base;
    // Creates numbers for combinations starting from 0 to number of locations
    for (int i = 0; i < n_locations; ++i) {
        comb[i] = i;
    }
    int fastest_comb[n_locations];
    int temp_dist;
    int total_dist = 0;
    for (int j = 1; j <= n_locations; j++) {
        for (int i = 0; i < n_locations-1; i++) {
            // Permutates the list of combination numbers
            temp_dist = 0;
            temp = comb[i];
            comb[i] = comb[i + 1];
            comb[i + 1] = temp;

            // Calculates the distance between every store
            for (int h = 0 ; h < n_locations-1 ; h++) {
                current_dest = stores_to_visit[comb[h+1]].store.store_coord;
                current_base = stores_to_visit[comb[h]].store.store_coord;
                temp_dist += calc_distance(current_dest, current_base);
            }
            // Adds the distance to and from home
            temp_dist += calc_distance(stores_to_visit[comb[0]].store.base_coord,
                                       stores_to_visit[comb[0]].store.store_coord);
            temp_dist += calc_distance(stores_to_visit[comb[n_locations-1]].store.base_coord,
                                       stores_to_visit[comb[n_locations-1]].store.store_coord);
            // If the calculated distance is less than the current minimum, we save it.
            if (total_dist > temp_dist || total_dist == 0){
                total_dist = temp_dist;
                for (int k = 0; k < n_locations; ++k) {
                    fastest_comb[k] = comb[k];
                }
            }
        }
    }

    double total_sum = 0;
    int index;
    cart_item_s* first_item;
    coordinates_s base = cart_across[0].store.base_coord;
    // Distance to first store
    int distance = calc_distance(stores_to_visit[fastest_comb[0]].store.store_coord, base);

    printf("Your absolute cheapest customized shopping list\n");
    printf("| Store    | Distance | Item |           Price    |\n");

    // Calculates and prints the sum of prices and the distances between store a to store b.
    for (int i = 0; i < n_locations; ++i) {
        index = fastest_comb[i];
        first_item = index_of_first[index];
        printf("%-15s %.4dm %-15s %7.2lf DKK \n", stores_to_visit[index].store.name,
               distance, first_item->item.name, first_item->item.price);
        total_sum += first_item->item.price;

        // if there is more than 1 item in this store, we print it here
        for (int j = 1; j < items_per_store[index]; j++) {
            first_item++;
            printf("%28s %16.2lf DKK \n", first_item->item.name, first_item->item.price);
            total_sum += first_item->item.price;
        }
        // Prevent indexing error
        if (i < n_locations-1) {
            current_dest = stores_to_visit[fastest_comb[i+1]].store.store_coord;
            current_base = stores_to_visit[index].store.store_coord;
            distance = calc_distance(current_dest, current_base);
        } else {
            distance = calc_distance(stores_to_visit[index].store.store_coord, base);
        }
    }
    //Prints the final distance between the last store and home
    printf("Home %15dm \n",distance);

    printf("\nTotal %14.4dm %23.2lf DKK\n",total_dist,total_sum);


}
