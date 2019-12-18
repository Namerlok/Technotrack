#ifndef LIST_H
#define LIST_H

//----------------------------------------------------------------

#ifndef BOOL
#define BOOL

	#define TRUE 1
	#define FALSE 0

#endif

#define OUT_DUMP_FILE "ListDump.txt"

#define OUT_DUMP_GRAPH_FILE "List.dv"

#ifdef NDEBUG
	#define LIST_ASSERT_OK(lst)
#else
	#define LIST_ASSERT_OK(lst) \
		if (ListOK == FALSE) { \
			ListDump ((lst), OUT_DUMP_FILE, OUT_DUMP_GRAPH_FILE); \
			assert(!"ListOK\n"); \
		}
#endif

//----------------------------------------------------------------

typedef int list_data_t;
typedef int bool;

typedef struct list {
	list_data_t *data;
	int *next;
	int *prev;
	char *name;
	size_t size;
	size_t max_size;
	int head;
	int tail;
	int free;
	bool sorted;
	int errno;
	
} list_t;

enum ListError {
	LIST_NO_ERROR,
	LIST_MEMORY_SELECTION,
	LIST_INDEX_OUT_OF_BOUNDS,
	LIST_INCORRECT_NEW_SIZE,
	LIST_SIZE_ERROR,
	LIST_DATA_ERROR,
	LIST_FREE_ERROR,
	LIST_SORTED_ERROR
};

enum ListConst {
	END_LIST = 0,
	FREE_ELEM_PREV = -1,
	MULTIPLIER_LIST = 2
};

//----------------------------------------------------------------

int ListCreat (list_t *lst, 
			   const char *lst_name, 
			   const size_t crt_size);

int ListClear (list_t *lst);

void ListDelete (list_t *lst);

int ListResize (list_t *lst, 
				const size_t new_size);

bool ListOK (list_t *lst);

void ListDump (const list_t *lst, 
			   const char *out_file, 
			   const char *out_file_graph);

char *ListErrorDefinition (const int errno);

void ListDumpGraph (const list_t *lst, 
					const char *out_file);

int ListInsert (list_t *lst, 
				const int index, 
				const list_data_t elem);

int ListRemove (list_t *lst, 
				const int index);

int ListFind (const list_t *lst,
			  list_data_t *elem);

bool ListCompareElem (list_data_t *elem1,
					  list_data_t *elem2);

list_data_t ListGetElem (list_t *lst,
						 const int index);

int ListGetNext (list_t *lst,
				 const int index);

int ListGetPrev (list_t *lst,
				 const int index);

int ListSort (list_t *lst);

int ListSwap (list_t *lst, 
			  const int i, 
			  const int j);

#endif
