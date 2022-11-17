#define MAX_NAME_SIZE 20

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

store_s* load_distances(void); // read from file
void load_normal_prices(void); // read from file
void load_discounts(void); // read from file
void load_shopping_list(void); // read from file
void add_item(store_s* store, char* name, double price); // load normal prices
void update_item(void); // discounts
