typedef struct {
    char name[20];
    int price;
} item_s;

typedef struct node_t {
    item_s item;
    struct node_t* next;
} node_t;

typedef struct {
    char name[20];
    int distance;
    node_t* first_item;
} store_s;

typedef struct {
    store_s store;
    item_s item;
} store_item;

int main(void) {
    return 0;
}
