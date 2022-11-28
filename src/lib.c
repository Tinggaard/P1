#include "lib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * compare function for qsort
 * @param ptr1 item 1
 * @param ptr2 item 2
 * @return returns negative if item 2 is greater, positive if item 1 is greater and 0 if they are equal
 */
int compare_cart(const void* ptr1, const void* ptr2) {
    const cart_sum* item_1 = ptr1;
    const cart_sum* item_2 = ptr2;
    if (item_1->sum - item_2->sum == 0) {
        return item_1->store.distance - item_2->store.distance;
    }
    return (int)(item_1->sum - item_2->sum);
}

/**
 * open_file() returns a pointer to a filestream
 * @param filename file to open
 * @return pointer to FILE
 */
FILE* open_file(char filename[]) {
    FILE *f = fopen(filename, "r");
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
int compare_name_distance(const void* ptr1, const void* ptr2) {
    const cart_item_s* item_1 = ptr1;
    const cart_item_s* item_2 = ptr2;
    if (item_1->store.distance - item_2->store.distance == 0) { // if the distances are the same, we check the name
        return (int)(item_1->store.name - item_2->store.name);
    }
    return (int)(item_1->store.distance - item_2->store.distance); // return closest store.
}

/**
 * get_new_lines() counts number of lines in a given file
 * @param filename file to parse
 * @return number of lines is given file
 */
int get_new_lines(char* filename){
    int count = 1; //initializes 1 because if there's no new line, there is still one line
    char c;
    FILE *f = open_file(filename);
    for (c = getc(f); c != EOF; c = getc(f))
        if (c == '\n') //Everytime the char c is \n count is increased
            count++;
    fclose(f);
    return count;
}

/**
 * load_distances() loads the distances to the stores
 * @return returns stores in an array, as structs (store_s)
 */
store_s* load_distances(char filename[], int store_count) {
    FILE *f = open_file(filename);

    // Allocates space in the heap for all the stores that are now being collected in a dynamic store_s struct array
    store_s* store = malloc(store_count * sizeof(store_s));

    // Scans all stores and parses their values to an index in the struct array
    for (int i = 0; i < store_count; i++) {
        fscanf(f, "%[^,], %d\n", store[i].name, &store[i].distance);
    }
    fclose(f);
    return store;
}

/**
 * load_normal_prices() loads the normal prices into the store array.
 * @param stores array of store_s
 * @param store_count amount of stores
 */
void load_normal_prices(store_s *stores, int store_count, char filename[], int item_count) {
    FILE *f = open_file(filename);

    item_s* normal_prices = malloc(sizeof(item_s) * item_count);

    for (int i = 0; i < item_count; i++) {
        fscanf(f, "%[A-Za-z ], %lf\n", normal_prices[i].name, &normal_prices[i].price);
    }

    item_s** items = malloc(sizeof(item_s) * store_count);
    for (int i = 0; i < store_count; i++) {
        items[i] = malloc(sizeof(*items[i]) * item_count); //Allocate space for arena
        stores[i].item = items[i];
        for (int j = 0; j < item_count; j++){
            strcpy(stores[i].item[j].name, normal_prices[j].name);
            stores[i].item[j].price = normal_prices[j].price;
        }
    }

    free(normal_prices);

    fclose(f);
}

/**
 * load_discounts() overrides the discounts to their respective stores
 * @param stores array of store_s
 */

void load_discounts(store_s* stores, char filename[]) {
    FILE *f = open_file(filename);

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

shopping_list_s* load_shopping_list(char filename[]) {
    int number_of_items = get_new_lines(filename);

    FILE *f = open_file(filename);

    // Allocates space in the heap for all the shopping list items
    shopping_list_s* s_list = malloc(number_of_items * sizeof(shopping_list_s));

    // Scans all item names into a shopping_list_s array
    for (int i = 0; i <= number_of_items; i++) {
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

cart_item_s* create_shopping_cart(store_s* stores, shopping_list_s* shopping_list, int n_stores, int n_shopping_list, int n_items){
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
                    cart[cart_index].store.distance = stores[i].distance;
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
 /*
cart_item_s* sum_across_stores(cart_item_s* cart,shopping_list_s* shopping_list, int n_stores, int n_shopping_list){
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
            current_item = find_cheapest_cart_item(cart, current_item, cart_index);
        }
        // makes a copy of current_item in cart_across.
        strcpy(cart_across[shopping_list_index].item.name, current_item.item.name);
        cart_across[shopping_list_index].item.price = current_item.item.price;
        strcpy(cart_across[shopping_list_index].store.name, current_item.store.name);
        cart_across[shopping_list_index].store.distance = current_item.store.distance;
    }
    // we sort the list, so items from same shop are printed together
    qsort(cart_across,n_shopping_list, sizeof(cart_item_s),compare_name_distance);
    print_sum_across_stores(n_shopping_list, cart_across);
    return cart_across;
}
*/
/**
* find_cheapest_cart_item() compares a shopping list item with a cart item and swaps them, if they match and the cart item is cheaper.
* @param cart cart to evaluate items from
* @param current_item the item to compare with the the item in the cart
* @param cart_index index for where we are in the cart
* @return returns the cheapest item of the two compared, if the parsed cart item had the same name as the parsed current_item
*/
/*
cart_item_s find_cheapest_cart_item(cart_item_s* cart, cart_item_s current_item, int cart_index) {
    // First it check if the two parsed items have the same name, if not, it just returns the current_item that was parsed
    if (strcmp(current_item.item.name, cart[cart_index].item.name) == 0) {

        // Checks to see if the price is 0
        if (current_item.item.price == 0) {
            // if it is 0, it knows that this current_item has not yet been initialized with any values but a name and therefore transfers the first match's values
            current_item.item.price = cart[cart_index].item.price;
            strcpy(current_item.store.name, cart[cart_index].store.name);
            current_item.store.distance = cart[cart_index].store.distance;
        }

        // Checks to see if the two items have the same price. if they have the same price, it takes the closest store
        if (current_item.item.price == cart[cart_index].item.price && current_item.store.distance > cart[cart_index].store.distance) {
            current_item.item.price = cart[cart_index].item.price;
            strcpy(current_item.store.name, cart[cart_index].store.name);
            current_item.store.distance = cart[cart_index].store.distance;
        }

        //  Compare prices and transfers the cheapest option to the current item
        if (current_item.item.price > cart[cart_index].item.price) {
            current_item.item.price = cart[cart_index].item.price;
            strcpy(current_item.store.name, cart[cart_index].store.name);
            current_item.store.distance = cart[cart_index].store.distance;
        }
    }
    return current_item; // Returns the cheapest option
}
*/
/**
 * printfunction for the solution across storese
 * @param n_shopping_list amount of items in our shopping list
 * @param cart_across our cart across stores
 */

//void print_sum_across_stores(int n_shopping_list, cart_item_s* cart_across){
//    double total_sum = 0;
//    printf("Your absolute cheapest customized shopping list\n");
//    printf("| Store    | Distance | Item |           Price    |\n");
//    for (int i = 0; i < n_shopping_list; ++i) { // calculates the sum of item prices
//        printf("%-15s %*dm %-15s %7.2lf DKK \n",cart_across[i].store.name,
//               5,cart_across[i].store.distance,cart_across[i].item.name, cart_across[i].item.price);
//        total_sum += cart_across[i].item.price;
//    }
//    printf("\nTotal %40.2lf DKK\n",total_sum);
//}



cart_sum* print_cart_sum_per_store(cart_item_s* cart_item, int n_shopping_list, int n_stores, store_s* stores){
    double sum[n_stores];

    for (int i = 0; i < n_stores; i++){
        for (int j = 0; j < n_shopping_list; j++){
            sum[i] += cart_item[i*n_shopping_list+j].item.price;
        }
    }

    cart_sum* cart = malloc(n_stores * sizeof(cart_sum));

    for (int i = 0; i < n_stores; i++) {
        cart[i].sum = sum[i]; //copies the item price from the array
        strcpy(cart[i].store.name, stores[i].name); //copies the name of the stores into the new struct
        cart[i].store.distance = stores[i].distance; //copies the distances int of the new struct
    }

    qsort(cart, n_stores, sizeof(cart_sum), compare_cart);

    //print function
    for (int i = 0; i < n_stores; ++i) {
        printf("|Name > %8s : Distance > %4d : Total sum > %4.2lf|\n", cart[i].store.name, cart[i].store.distance, cart[i].sum);
    }
    return cart;
}
