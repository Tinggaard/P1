#define MAX_NAME_SIZE 15

// An item_s struct includes a price of the item and a name
typedef struct {
    char name[MAX_NAME_SIZE];
    double price;
} item_s;

// node_t struct is used to create an associative array of items in a store.
typedef struct node_t {
    item_s item;
    struct node_t* next;
} node_t;

// store_s has a name of the store, a distance from the user to the store,
// and a pointer to the first item in the store (using the associative array of items to include all items)
typedef struct {
    char name[MAX_NAME_SIZE];
    int distance;
    node_t* first_item;
} store_s;

// cart_item is used to combine a store with a specific item from the users shopping list
typedef struct {
    store_s store;
    item_s item;
} cart_item;

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
cart_item find_cheapest_cart_item(cart_item* cart, cart_item current_item, int cart_index);
// Load functions
void load_normal_prices(store_s stores[], int store_count, char filename[]);
store_s* load_distances(char filename[]);
void load_discounts(store_s stores[], char filename[]);
shopping_list_s* load_shopping_list(char filename[]);
cart_item* create_shopping_cart(store_s* stores, shopping_list_s* shopping_list, int n_stores, int n_shopping_list);

// Calculate cheapest options
cart_item* sum_across_stores(cart_item* cart,shopping_list_s* shopping_list, int n_stores, int n_shopping_list);

// Calculate printfunctions
void print_sum_across_stores(int n_shopping_list, cart_item* cart_across);

