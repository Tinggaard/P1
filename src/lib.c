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
    if (item_1->total_sum - item_2->total_sum == 0) {
        return (int)(calc_distance(item_1->store.base_coord, item_1->store.store_coord) - calc_distance(item_2->store.base_coord, item_2->store.store_coord));
    }
    return (int)(item_1->total_sum - item_2->total_sum);
}

/**
 * compare function for qsort, to sort in alphabetical order
 * @param ptr1 item 1
 * @param ptr2 item 2
 * @return 0 if they are equal, >0 if item1 appears before item2 in lexicographical order and opposite for <0.
 */
int compare_item_names(const void* ptr1, const void* ptr2){
    const item_s* item_1 = ptr1;
    const item_s* item_2 = ptr2;
    return strcmp(item_1->name, item_2->name);
}


/**
 * binary_search() is used to search the item array, for a specific item
 * @param itemlist list of item_s
 * @param x name of item to look for
 * @param n_items number of items in total
 * @return index of item name in itemlist
 */
int binary_search(item_s itemlist[], const char x[], int n_items) {
    int l, r, m; // left, right, middle
    l = 0;
    r = n_items - 1;
    while (l < r){
        // set middle correctly
        m = (r + l) / 2;

        // if we are sill not there
        if (0 > strcmp(itemlist[m].name, x)) {
            l = m + 1;
        } else {
            // if we have gone too far
            r = m;
        }
    }
    // the item was actually found
    if (strcmp(x, itemlist[l].name) == 0){
        return l;
    } else {
        // otherwise we throw an error
        printf("Item \"%s\" not found\n", x);
        exit(EXIT_FAILURE);
    }
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


void swap_stores(store_s stores_to_visit[], int i, int j){
    store_s temp = stores_to_visit[i];
    stores_to_visit[i] = stores_to_visit[j];
    stores_to_visit[j] = temp;
}

void swap_int(int arr[], int i, int j){
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

double calc_gas_price(double km_price, int dist) {
    return (km_price * dist) / 1000;
}


/**
 * distance_user_to_stares() calculates the distance between base and store coordinates
 * @param location1 starting point
 * @param location2 destination
 * @return the distance in meters (int)
 */
// courtesy of https://stackoverflow.com/questions/27126714/c-latitude-and-longitude-distance-calculator
int calc_distance(coordinates_s location1, coordinates_s location2){
    double EARTH_RADIUS = 6372797.56085; // in meters
    double DEGREES_RADIANS = M_PI / 180; // from degrees to radians

    double haversine, arcsin, distance, sin_lat, cos_lat, lon, minimum;

    // we convert degrees to radians
    double lat1 = location1.lat * DEGREES_RADIANS;
    double lon1 = location1.lon * DEGREES_RADIANS;
    double lat2 = location2.lat * DEGREES_RADIANS;
    double lon2 = location2.lon * DEGREES_RADIANS;

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

    // Checks to see if the price is 0
    if (current_item.item.price == 0) {
        // if it is 0, it knows that this current_item has not yet been initialized with any values but a name
        // and therefore transfers the first match's values
        current_item.item.price = cart[cart_index].item.price;
        strcpy(current_item.store.name, cart[cart_index].store.name);
        copy_coord(&current_item.store, &cart[cart_index].store);
    }

    // Checks to see if the two items have the same price. if they have the same price, it takes the closest store
    else if (current_item.item.price == cart[cart_index].item.price &&
        calc_distance(current_item.store.base_coord, current_item.store.store_coord) >
        calc_distance(cart[cart_index].store.base_coord, cart[cart_index].store.store_coord)) {
        current_item.item.price = cart[cart_index].item.price;
        strcpy(current_item.store.name, cart[cart_index].store.name);
        copy_coord(&current_item.store, &cart[cart_index].store);
    }

    //  Compare prices and transfers the cheapest option to the current item
    else if (current_item.item.price > cart[cart_index].item.price) {
        current_item.item.price = cart[cart_index].item.price;
        strcpy(current_item.store.name, cart[cart_index].store.name);
        copy_coord(&current_item.store, &cart[cart_index].store);
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
store_s* load_coordinates(char filename[], int* n_stores, coordinates_s user_location, int radius) {
    FILE* f = open_file(filename);
    int n_store_counter = 0;
    int store_index = 0;

    // Allocates space in the heap for all the stores that are now being collected in a dynamic store_s struct array
    store_s* store_placeholder = malloc(*n_stores * sizeof(store_s));

    // Scans all stores and parses their values to an index in the struct array
    for (int i = 0; i < *n_stores; i++) {
        fscanf(f, "%[^,], %lf, %lf\n", store_placeholder[i].name, &store_placeholder[i].store_coord.lat, &store_placeholder[i].store_coord.lon);
        store_placeholder[i].base_coord.lat = user_location.lat;
        store_placeholder[i].base_coord.lon = user_location.lon;
        if(calc_distance(store_placeholder[i].base_coord, store_placeholder[i].store_coord) <= radius){
            n_store_counter++;
        }
    }

    store_s* stores = malloc(n_store_counter * sizeof(store_s));
    for (int i = 0; i < *n_stores; i++) {
        if(calc_distance(store_placeholder[i].base_coord, store_placeholder[i].store_coord) <= radius){
            strcpy(stores[store_index].name, store_placeholder[i].name);
            stores[store_index].store_coord.lat = store_placeholder[i].store_coord.lat;
            stores[store_index].store_coord.lon = store_placeholder[i].store_coord.lon;
            stores[store_index].base_coord.lat = user_location.lat;
            stores[store_index].base_coord.lon = user_location.lon;
            store_index++;
        }
    }
    free(store_placeholder);

    *n_stores = n_store_counter;
    //qsort(store, *n_stores, sizeof(store_s), compare_store_name);
    fclose(f);
    return stores;
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

    // We use qsort to sort the items in alphabetical order
    qsort(normal_prices, n_items, sizeof(item_s), compare_item_names);

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
void load_discounts(store_s stores[], char filename[], int n_items, int n_stores) {
    FILE* f = open_file(filename);
    char current_store[MAX_NAME_SIZE];
    char current_item[MAX_NAME_SIZE];
    double current_price;
    int item_index;
    // Runs over all rows in the file until it reaches the end of the file
    while (!feof(f)) {
        // Scans for store name, item name and the price of an item. Excludes commas.
        fscanf(f, "%[^,], %[^,], %lf\n", current_store, current_item, &current_price);

        // Gets the index of the store that matches the currently scanned one
        for (int i = 0; i < n_stores; ++i) {
            if(strcmp(stores[i].name, current_store) == 0){
                // Binary_search finds the index of the item in the item array
                item_index = binary_search(stores[i].item, current_item, n_items);
                stores[i].item[item_index].price = current_price; // Replaces the normal price with the discount
                break;
            }
        }



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
    int item_index = 0;
    // Iterates over all stores

    for (int i = 0; i < n_stores; i++) {
        // Iterate every item in the shopping list
        for (int j = 0; j < n_shopping_list; j++) {
            // Binary search finds the index the item in the item array
            item_index = binary_search(stores[i].item, shopping_list[j].name, n_items);

            // All the necessary information is copied from store and item into cart.
            strcpy(cart[cart_index].store.name, stores[i].name);
            copy_coord(&cart[cart_index].store, &stores[i]); // Copy coordinates into cart_item_s
            strcpy(cart[cart_index].item.name, stores[i].item[item_index].name);
            cart[cart_index].item.price = stores[i].item[item_index].price;
            cart_index++; // Updates the cart index to avoid overwriting already added items
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
void calc_across_stores(cart_item_s cart[], shopping_list_s shopping_list[], store_s store[], int n_stores, int n_shopping_list, double km_price){
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
            // First it check if the two items have the same name, if they have then calc_cheapest_cart_item
            if (strcmp(current_item.item.name, cart[cart_index].item.name) == 0) {
                current_item = calc_cheapest_cart_item(cart, current_item, cart_index);
            }
        }
        // makes a copy of current_item in cart_across.
        strcpy(cart_across[shopping_list_index].item.name, current_item.item.name);
        cart_across[shopping_list_index].item.price = current_item.item.price;
        strcpy(cart_across[shopping_list_index].store.name, current_item.store.name);
        copy_coord(&cart_across[shopping_list_index].store, &current_item.store);

    }
    // Calculates the shortest path and prints the new shopping list
    shortest_path(cart_across, n_shopping_list, n_stores, km_price);
}


/**
 * print_cart_sum_per_store() prints a sorted list for the cheapest option to shop in
 * @param cart_item an array of cart_item
 * @param n_shopping_list number of items in shopping list
 * @param n_stores number of stores
 * @param stores array of stores
 * @return returns an array of cart_sum, one for each store
 */
void calc_per_store(cart_item_s cart_item[], int n_shopping_list, int n_stores, store_s* stores, double km_price){
    double sum[n_stores];
    // initialize list to 0, to avoid garbage
    for (int i = 0; i < n_stores; i++) {
        sum[i] = 0;
    }

    for (int i = 0; i < n_stores; i++) {
        for (int j = 0; j < n_shopping_list; j++) {
            sum[i] += cart_item[i*n_shopping_list+j].item.price;
        }
    }

    cart_sum_s* cart = malloc(n_stores * sizeof(cart_sum_s));

    for (int i = 0; i < n_stores; i++) {
        cart[i].item_sum = sum[i]; //copies the item price from the array
        strcpy(cart[i].store.name, stores[i].name); //copies the name of the stores into the new struct
        copy_coord(&cart[i].store, &stores[i]);
        cart[i].total_sum = calc_gas_price(km_price, calc_distance(cart[i].store.base_coord, cart[i].store.store_coord)) + cart[i].item_sum;
    }

    qsort(cart, n_stores, sizeof(cart_sum_s), compare_cart);


    //print function, prints the store name,
    // prints the distance to the store, prisen for distance,
    //prints the item expenesses and the sum of item expenses and travel exprenses
    int base_to_store = 0;
    double travel_expense = 0;
    double total_price = 0;
    printf("\n\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");

    if(km_price != 0) {
        printf("  ------------------------------------------------------------------------------------\n");
        printf("  |                        If you want to shop in one store only,                    |\n");
        printf("  |                  we have sorted a list of cheapest stores for you.               |\n");
        printf("  ------------------------------------------------------------------------------------\n");
        printf("  |Store          |Distance     |Travel expenses    |Item expenses    |Total sum     |\n");
        for (int i = 0; i < n_stores; i++) {
            base_to_store = calc_distance(cart[i].store.base_coord, cart[i].store.store_coord);
            travel_expense = calc_gas_price(km_price, base_to_store);
            total_price = travel_expense + cart[i].item_sum;
            printf("  |%-15s%5.4dm      %8.2lf DKK         %8.2lf DKK      %8.2lf DKK    |\n",cart[i].store.name,base_to_store,
                   travel_expense, cart[i].item_sum, cart[i].total_sum);
        }
        printf("  ------------------------------------------------------------------------------------\n");

    }
    else{
        printf("            ---------------------------------------------------------------\n");
        printf("            |            If you want to shop in one store only,           |\n");
        printf("            |      we have sorted a list of cheapest stores for you.      |\n");
        printf("            ---------------------------------------------------------------\n");
        printf("            |Store          |Distance                        |Total sum   |\n");
        for (int i = 0; i < n_stores; i++) {
            base_to_store = calc_distance(cart[i].store.base_coord, cart[i].store.store_coord);
            total_price = calc_gas_price(km_price, base_to_store) + cart[i].item_sum;
            printf("            |%-15s%5.4dm                            %6.2lf DKK  |\n",
                   cart[i].store.name,
                   base_to_store, total_price);
        }
        printf("            ---------------------------------------------------------------\n");
    }
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
}

coordinates_s user_input(char user_location_f[], double* km_price, int* radius){

    FILE* f = open_file(user_location_f);
    //select from preivously loaded locations
    int current_location;
    char by_car = 0;
    coordinates_s user_location;

    while(!feof(f)){
        fscanf(f,"%lf, %lf\n",&user_location.lat, &user_location.lon);
    }

    // radius input
    printf("Please enter the radius that you want to shop within (in meters) >\n");
    scanf(" %d",radius);

    while (by_car != 'y' && by_car != 'Y' && by_car != 'n' && by_car != 'N'){//checks if the user is driving or not
        printf("Do you travel by car (y/n)? \n");
        scanf(" %c",&by_car);
    }
    //if the user is drivin the km price is entered
    if(by_car == 'y' || by_car == 'Y'){
        printf("Enter price per. kilometer > \n");
        scanf(" %lf", km_price);
    }
    return user_location;
}


/**
 * shortest_path calculates the shortest path and outputs the new shopping list
 * @param cart_across The cart with the cheapest option across stores
 * @param store Array of stores
 * @param n_shopping_list amount of items in shopping list
 * @param n_stores amount of stores
 */
void shortest_path(cart_item_s cart_across[], int n_shopping_list, int n_stores, double km_price) {
    // sort the array, according to store names
    qsort(cart_across, n_shopping_list, sizeof(cart_item_s), compare_store_name);

    int n_locations = 1; // minimum 1 store
    // need to make this n_stores long, as we might only have 1 item in each store
    store_s stores_to_visit[n_stores];

    //index of first is the index of the first item in cart_across for each store in stores_to_visit.
    int index_of_first[n_stores];

    // create first store manually
    strcpy(stores_to_visit[0].name, cart_across[0].store.name);
    copy_coord(&stores_to_visit[0], &cart_across[0].store);
    index_of_first[0] = 0;
    // Creates an array with only of the stores you have to visit with no repeating stores.
    for (int i = 1; i < n_shopping_list; i++) {
        // if this store is not seen before we add it
        if (strcmp(cart_across[i].store.name, stores_to_visit[n_locations-1].name) != 0) {
            strcpy(stores_to_visit[n_locations].name, cart_across[i].store.name);
            copy_coord(&stores_to_visit[n_locations], &cart_across[i].store);
            index_of_first[n_locations] = i; //index of where the store's first item can be found in cart_across
            n_locations++;
        }
    }

    coordinates_s current_location = stores_to_visit[0].base_coord;
    int index_of_nearest;  // the index of the nearest store in stores_to_visit[]
    double distance_to_nearest;
    double temp_distance;
    int n_stores_visited = 0;  // number of stores already visited
    int distances[n_stores];   // array of distances between each location, which is stored for later use in printf()
    int total_distance = 0;
    // while loop runs until every store has been visited, in each iteration of the while loop we find the nearest store
    while (n_stores_visited < n_locations) {
        distance_to_nearest = calc_distance(current_location, stores_to_visit[n_stores_visited].store_coord);
        index_of_nearest = n_stores_visited;
        // Iterates through every store. i = n_stores_visited so that we won't go to the same store twice
        for (int i = n_stores_visited; i < n_locations; ++i) {
            //Distance from current location to current store is calculated
            temp_distance = calc_distance(current_location, stores_to_visit[i].store_coord);
            if (temp_distance < distance_to_nearest) { // If the distance is shorter than the currently shortest,
                distance_to_nearest = temp_distance;   // then the distance and the index of the store is saved
                index_of_nearest = i;
            }
        }
        current_location = stores_to_visit[index_of_nearest].store_coord; // location is changed to the nearest store
        total_distance += distance_to_nearest;
        // the stores in stores_to_visit is swapped around so that they are in the same order as we visit them
        swap_stores(stores_to_visit, n_stores_visited, index_of_nearest);
        swap_int(index_of_first, n_stores_visited, index_of_nearest);
        distances[n_stores_visited] = distance_to_nearest;
        n_stores_visited++;
    }

    // Calculates the distance from the last store to home and adds it to the total distance.
    int dist_last_store_to_home = calc_distance(stores_to_visit[n_locations-1].base_coord, stores_to_visit[n_locations-1].store_coord);
    total_distance += dist_last_store_to_home;

    double item_price_sum = 0;
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    printf("             ---------------------------------------------------------------\n");
    printf("             |           Your absolute cheapest shopping list              |\n");
    printf("             |   Follow the order of stores to achieve the shortest rute   |\n");
    printf("             ---------------------------------------------------------------\n");

    printf("             |Store          |Distance     |Item           |Price          |\n");
    for (int i = 0; i < n_locations; ++i) { // Iterates through every store that was visited
        // index_of_first[i] is used to get the index of the current store's first item in cart_across[]
        printf("             |%-15s%5.4dm         %-15s%6.2lf DKK      |\n", stores_to_visit[i].name,
               distances[i], cart_across[index_of_first[i]].item.name,cart_across[index_of_first[i]].item.price);
        item_price_sum += cart_across[index_of_first[i]].item.price;

        // The while-loop prints the rest of the items from the current store
        while(strcmp(cart_across[index_of_first[i]].store.name, cart_across[index_of_first[i]+1].store.name) == 0){
            printf("             |                              %-15s%6.2lf DKK      |\n", cart_across[index_of_first[i]+1].item.name, cart_across[index_of_first[i]+1].item.price);
            item_price_sum += cart_across[index_of_first[i]+1].item.price;
            index_of_first[i]++;
        }
    }

    printf("             |Home           %5.4dm                                        |\n", dist_last_store_to_home);

    // Prints the total distance, travel expenses and total price
    if(km_price != 0){
        double travel_expenses = calc_gas_price(km_price, total_distance);
        double total_price = item_price_sum + travel_expenses;

        printf("             ---------------------------------------------------------------\n");
        printf("             |   Travel expenses: %6.2lf DKK | Item expenses: %6.2lf DKK   |\n",travel_expenses, item_price_sum);
        printf("             ---------------------------------------------------------------\n");
        printf("             ---------------------------------------------------------------\n");
        printf("             |     Total distance: %6.4dm | Total price: %7.2lf DKK      |\n",total_distance, total_price);
        printf("             ---------------------------------------------------------------\n");
        printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");

    }
    else{
        printf("             ---------------------------------------------------------------\n");
        printf("             |      Total distance: %6.4dm | Total price: %6.2lf DKK      |\n",total_distance, item_price_sum);
        printf("             ---------------------------------------------------------------\n");
        printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");


    }
}
