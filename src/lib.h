#define MAX_NAME_SIZE 15

typedef struct {
    char name[MAX_NAME_SIZE];
    double price;
} item_s;

typedef struct node_t {
    item_s item;
    struct node_t* next;
} node_t;

typedef struct {
    char name[MAX_NAME_SIZE];
    int distance;
    node_t* first_item;
} store_s;

typedef struct {
    store_s store;
    item_s item;
} cart_item;

typedef struct{
    char name[MAX_NAME_SIZE];
}shoppinglist_s;



void load_normal_prices(store_s stores[], int store_count); // read from file
store_s* load_distances(void); // read from file
void load_discounts(store_s stores[]); // read from file
shoppinglist_s* load_shopping_list(void); // read from file
void add_item(store_s* store, char* name, double price); // load normal prices
void deallocate_list(store_s* store); //frees the memory used in malloc
int get_new_lines(char* filename); //gets number of new lines
item_s* cheapest_onestore(store_s* stores, shoppinglist_s* shoppinglist, int n_stores, int n_shoppinglist);
void cheapest_overall_cart(void);
void cheapest_closest_cart(void);
