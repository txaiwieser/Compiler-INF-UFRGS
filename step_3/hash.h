
typedef struct hash_node
{
	int type;
	char *text;
	struct hash_node *next;
	
} hash_node_t;

void hash_init(void);
int hash_address(char *text);
hash_node_t *hash_find(char *text);
hash_node_t *hash_insert(char *text, int type);
void hash_print(void);