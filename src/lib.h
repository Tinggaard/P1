#define MAX_NAME_SIZE 15

// An item_s struct includes a price of the item and a name
typedef struct {
    char name[MAX_NAME_SIZE];
    double price;
} item_s;

// node_s struct is used to create an associative array of items in a store.
typedef struct node_s {
    item_s item;
    struct node_s* next;
} node_s;

// store_s has a name of the store, a distance from the user to the store,
// and a pointer to the first item in the store (using the associative array of items to include all items)
typedef struct {
    char name[MAX_NAME_SIZE];
    int distance;
    item_s* item;
} store_s;

// cart_item_s is used to combine a store with a specific item from the users shopping list
typedef struct {
    store_s store;
    item_s item;
} cart_item_s;

//cart is ued to combine all the items of a store and have a total sum of the store
typedef struct {
    store_s store;
    double sum;
} cart_sum;

// shopping_list_s is a struct used to hold the users shopping list items
typedef struct {
    char name[MAX_NAME_SIZE];
} shopping_list_s;

// Helper functions
int get_new_lines(char filename[]); // Gets number of new lines
void add_item(store_s* store, char* name, double price); // Adds an item to the associative array of store items
void deallocate_list(store_s* store); // Frees the memory used in malloc
int compare_cart(const void* ptr1, const void* ptr2); // Compares items by price and then distance to the user
int compare_name_distance(const void* ptr1, const void* ptr2);
cart_item_s find_cheapest_cart_item(cart_item_s* cart, cart_item_s current_item, int cart_index);
// Load functions
void load_normal_prices(store_s stores[], int store_count, char filename[]);
store_s* load_distances(char filename[]);
void load_discounts(store_s stores[], char filename[]);
shopping_list_s* load_shopping_list(char filename[]);
cart_item_s* create_shopping_cart(store_s* stores, shopping_list_s* shopping_list, int n_stores, int n_shopping_list);

// Calculate cheapest options
cart_item_s* sum_across_stores(cart_item_s* cart, shopping_list_s* shopping_list, int n_stores, int n_shopping_list);

// Calculate printfunctions
void print_sum_across_stores(int n_shopping_list, cart_item_s* cart_across);

cart_sum* print_cart_sum_per_store(cart_item_s* cart_item, int n_shopping_list, int n_stores, store_s* stores);
void cheapest_overall_cart(void);
