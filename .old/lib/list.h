#ifndef LIST_H
#define LIST_H


/*******************************************************************************
  Datatype definitions                       
*******************************************************************************/

/* Forward declaration */
struct lnode;

/* The linked list struct.  Has a head & tail pointer. */
typedef struct llist
{
  struct lnode* head; /* Head pointer either points to a node with data or NULL */
  struct lnode* tail; /* Tail pointer either points to a node with data or NULL */
  unsigned int size; /* Size of the linked list */
} list;

/* A function pointer type that points to a function that takes in a void* and returns nothing call it list_op */
typedef void (*list_op)(void*);
/* A function pointer type that points to a function that takes in a const void* and returns an int call it list_pred */
typedef int (*list_pred)(const void*);


/**************************************************
** Prototypes for linked list library functions. **
**                                               **
** For more details on their functionality,      **
** check list.c.                                 **
***************************************************/

/* Creating */
list* create_list(void);

/* Adding */
void push_front(list* llist, void* data);
void push_back(list* llist, void* data);

/* Removing */
int remove_front(list* llist, list_op free_func);
int remove_back(list* llist, list_op free_func);
int remove_if(list* llist, list_pred pred_func, list_op free_func);

/* Querying List */
void* front(list* llist);
void* back(list* llist);
int is_empty(list* llist);
int size(list* llist);

/* Freeing */
void empty_list(list* llist, list_op free_func);

/* Traversal */
void traverse(list* llist, list_op do_func);

/* Debugging Support */
#ifdef LLDEBUG
    /*
       Does the following if compiled in debug mode
       When compiled in release mode does absolutely nothing.
    */
    #define IF_DEBUG(call) (call)
    /* Prints text (in red) if in debug mode */
    #define DEBUG_PRINT(string) fprintf(stderr, "\033[31m%s:%d %s\n\033[0m", __FILE__, __LINE__, (string))
    /* Asserts if the expression given is true (!0) */
    /* If this fails it prints a message and terminates */
    #define DEBUG_ASSERT(expr)   \
    do                           \
    {                            \
        if (!(expr))             \
        {                        \
            fprintf(stderr, "ASSERTION FAILED %s != TRUE (%d) IN %s ON line %d\n", #expr, (expr), __FILE__, __LINE__); \
            exit(0);             \
        }                        \
    } while(0)
#else
    #define IF_DEBUG(call)
    #define DEBUG_PRINT(string)
    #define DEBUG_ASSERT(expr)
#endif

#endif

