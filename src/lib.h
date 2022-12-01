#define MAX_NAME_SIZE 15

// An item_s struct includes a price of the item and a name
typedef struct {
    char name[MAX_NAME_SIZE];
    double price;
} item_s;

typedef struct {
    double lat;
    double lon;
} coordinates_s;

// store_s has a name of the store, a distance from the user to the store,
// and a pointer to the first item in the store (using the associative array of items to include all items)
typedef struct {
    char name[MAX_NAME_SIZE];
    coordinates_s store_coord;
    coordinates_s base_coord;
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
} cart_sum_s;

// shopping_list_s is a struct used to hold the users shopping list items
typedef struct {
    char name[MAX_NAME_SIZE];
} shopping_list_s;


// helper functions
int compare_cart(const void* ptr1, const void* ptr2);
void copy_coord(store_s* target, store_s* base);
int calc_base_to_store(store_s store);
int calc_distance(coordinates_s cord_base, coordinates_s cord_dest);

cart_item_s calc_cheapest_cart_item(cart_item_s cart[], cart_item_s current_item, int cart_index);
void shortest_path (cart_item_s cart_across[],store_s store[], int n_shopping_list, int n_stores);
// open_file function not in header as it returns FILE ptr, which is not defined here.
int get_new_lines(char filename[]);

// load and init
store_s* load_distances(char filename[], int n_stores);
void load_normal_prices(store_s stores[], int n_stores, char filename[], int n_items);
void load_discounts(store_s stores[], char filename[]);
shopping_list_s* load_shopping_list(char filename[], int n_shopping_list);
cart_item_s* create_shopping_cart(store_s stores[], shopping_list_s shopping_list[], int n_stores, int n_shopping_list, int n_items);

// calculate functions
void calc_across_stores(cart_item_s cart[], shopping_list_s shopping_list[], store_s store[], int n_stores, int n_shopping_list);
void calc_per_store(cart_item_s cart_item[], int n_shopping_list, int n_stores, store_s* stores);
