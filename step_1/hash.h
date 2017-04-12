
typedef struct hash_node_struck
{
	int type;
	char *text;
	struct hash_node_struck *next;
	
} HASH_NODE;

void hashInit(void);
int hashAddress(char *text);
HASH_NODE *hashFind(char *text);
HASH_NODE *hashInsert(char *text, int type);
void hashPrint(void);
