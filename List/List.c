/*!
 * \mainpage
 * \author Загребельный Аркадий
 * \version 0.1
 * \date Декабрь 2019 года
 *
 * \brief реализация структуры данных "Список" ("List")
 */

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "List.h"

int ListCreat (list_t *lst, 
			   const char *lst_name, 
			   const size_t crt_size) {
	assert (lst);
	assert (lst_name);
	
	lst->data = (list_data_t *) calloc (crt_size + 1, sizeof (*(lst->data)));
	lst->next = (int *) calloc (crt_size + 1, sizeof (*(lst->next)));
	lst->prev = (int *) calloc (crt_size + 1, sizeof (*(lst->prev)));
	lst->name = (char *) calloc (strlen(lst_name) + 1, sizeof (*(lst->name)));
	strcpy (lst->name, lst_name);
	
	if (lst->data == NULL || lst->next == NULL || lst->prev == NULL) {
		lst->errno = LIST_MEMORY_SELECTION;
		return lst->errno;
	}
	
	// нулевой элемент не используется
	lst->data[0] = 0;
	lst->next[0] = 0;
	lst->prev[0] = 0;
	
	// заполнение последовательности свободных ячеек 
	for (int i = 1; i <= crt_size; i++) {
		lst->next[i] = i + 1;
		lst->prev[i] = FREE_ELEM_PREV;
	}
	lst->next[crt_size] = END_LIST;
	lst->free = 1;
	
	lst->head = END_LIST;
	lst->tail = END_LIST;
	lst->size = 0;
	lst->max_size = crt_size;
	lst->errno = LIST_NO_ERROR;
	lst->sorted = TRUE;
	
	return lst->errno;
}

int ListClear (list_t *lst) {
	assert (lst);
	
	// нулевой элемент не используется
	lst->data[0] = 0;
	lst->next[0] = 0;
	lst->prev[0] = 0;
	
	// заполнение последовательности свободных ячеек 
	for (int i = 1; i <= lst->max_size; i++) {
		lst->data[i] = 0;
		lst->next[i] = i + 1;
		lst->prev[i] = FREE_ELEM_PREV;
	}
	lst->next[lst->max_size] = END_LIST;
	lst->free = 1;
	
	lst->head = END_LIST;
	lst->tail = END_LIST;
	lst->size = 0;
	lst->errno = LIST_NO_ERROR;
	lst->sorted = TRUE;
	
	return lst->errno;
}

void ListDelete (list_t *lst) {
	assert (lst);
	
	if (lst->data != NULL) {
		free (lst->data);
		lst->data = NULL;
	}
	if (lst->next != NULL) {
		free (lst->next);
		lst->next = NULL;
	}
	if (lst->prev != NULL) {
		free (lst->prev);
		lst->prev = NULL;
	}
	if (lst->name != NULL) {
		free (lst->name);
		lst->name = NULL;
	}
	
	lst->size = 0;
	lst->max_size = 0;
	lst->head = 0;
	lst->tail = 0;
	lst->free = 0;
	lst->sorted = FALSE;
	lst->errno = LIST_NO_ERROR;
}

int ListResize (list_t *lst, 
				const size_t new_size) {
	assert (lst);
	assert (new_size > 0);
	
	LIST_ASSERT_OK (lst);
	
	if (new_size < lst->size ) {
		lst->errno = LIST_INCORRECT_NEW_SIZE;
		return lst->errno;
	}
	
	list_data_t *new_data = (list_data_t *) realloc (lst->data, 
								(new_size + 1) * sizeof (*(lst->data)));
	int *new_next = (int *) realloc (lst->next, 
								(new_size + 1) * sizeof (*(lst->next)));
	int *new_prev = (int *) realloc (lst->prev, 
								(new_size + 1) * sizeof (*(lst->prev)));
	if (new_data == NULL || new_next == NULL || new_prev == NULL) {
		lst->errno = LIST_MEMORY_SELECTION;
		return lst->errno;
	}
	
	lst->data = new_data;
	lst->next = new_next;
	lst->prev = new_prev;
	
	int new_free = lst->max_size + 1;
	if (lst->free == END_LIST) {
		lst->free = new_free;
	} else {
		int i = lst->free;
		for (; lst->next[i] != END_LIST; i = lst->next[i])
			;
		lst->next[i] = new_free;
	}
	for (int i = new_free; i <= new_size; i++) {
		lst->data[i] = 0;
		lst->next[i] = i + 1;
		lst->prev[i] = FREE_ELEM_PREV;
	}
	lst->next[new_size] = END_LIST;
	
	lst->max_size = new_size;
	
	LIST_ASSERT_OK (lst);
	
	return lst->errno;
}

bool ListOK (list_t *lst) {
	assert (lst);
	
	if (lst->errno != LIST_NO_ERROR)
		return FALSE;
	
	if (lst->size > lst->max_size) {
		lst->errno = LIST_SIZE_ERROR;
		return FALSE;
	}
	
	if ((lst->head < 0) || (lst->tail < 0) ||
		(lst->next[lst->tail] != END_LIST) || 
		(lst->prev[lst->head] != END_LIST)) {
		
		lst->errno = LIST_DATA_ERROR;
		return FALSE;
	}
	
	int fr = lst->free;
	for (int i = 0; i < lst->max_size - lst->size - 1; i++, fr = lst->next[fr]) {
		if (lst->prev[fr] != FREE_ELEM_PREV || lst->next[fr] <= 0) {
			lst->errno = LIST_FREE_ERROR;
			return FALSE;
		}
	}
	if (lst->next[fr] != END_LIST) {
		lst->errno = LIST_FREE_ERROR;
		return FALSE;
	}
	
	if (lst->sorted) {
		for (int i = lst->head; lst->next[i] != 0; i = lst->next[i]) {
			if (lst->next[i] != i + 1) {
				lst->errno = LIST_SORTED_ERROR;
				return FALSE;
			}
		}
		if (lst->tail + 1 != lst->free) {
			lst->errno = LIST_SORTED_ERROR;
			return FALSE;
		}
		for (int i = lst->free; lst->next[i] != 0; i = lst->next[i]) {
			if (lst->next[i] != i + 1) {
				lst->errno = LIST_SORTED_ERROR;
				return FALSE;
			}
		}
	}
	
	return TRUE;
}

void ListDump (const list_t *lst, 
			   const char *out_file, 
			   const char *out_file_graph) {
	assert (lst);
	assert (out_file);
	assert (out_file_graph);
	
	FILE* out = fopen (out_file, "wb");
	if (out == NULL)
		perror ("Открытие выходного файла ListDump\n");
	
	fprintf (out, "list_t \"%s\" [%p] {\n", lst->name, lst);
	fprintf (out, "\tdata [%p]:\n", lst->data);
	for (int i = 0; i <= lst->max_size; i++) {
		fprintf (out, "\t\t[%2i]: %i\n", i, lst->data[i]);
	}
	fprintf (out, "\tnext [%p]:\n", lst->next);
	for (int i = 0; i <= lst->max_size; i++) {
		fprintf (out, "\t\t[%2i]: %2i\n", i, lst->next[i]);
	}
	fprintf (out, "\tprev [%p]:\n", lst->prev);
	for (int i = 0; i <= lst->max_size; i++) {
		fprintf (out, "\t\t[%2i]: %2i\n", i, lst->prev[i]);
	}
	fprintf (out, "\tsize [%p] = %li\n", &(lst->size), lst->size);
	fprintf (out, "\tmax size [%p] = %li\n", &(lst->max_size), lst->max_size);
	fprintf (out, "\thead [%p] = %i\n", &(lst->head), lst->head);
	fprintf (out, "\ttail [%p] = %i\n", &(lst->tail), lst->tail);
	fprintf (out, "\tfree [%p] = %i\n", &(lst->free), lst->free);
	fprintf (out, "\tsorted [%p] = %i\n", &(lst->sorted), lst->sorted);
	fprintf (out, "\terrno [%p] = %i (%s)\n", &(lst->errno), lst->errno,
			 (lst->errno) ? ListErrorDefinition (lst->errno) : "OK");
	fprintf (out, "}\n");
	
	fclose (out);
	
	ListDumpGraph (lst, out_file_graph);
}

char *ListErrorDefinition (const int errno) {
	switch (errno) {
	case LIST_MEMORY_SELECTION:
		return "Ошибка выделения памяти";
	case LIST_INDEX_OUT_OF_BOUNDS:
		return "Индекс за пределами допустимых значений";
	case LIST_INCORRECT_NEW_SIZE:
		return "Некорректный новый размер list";
	case LIST_SIZE_ERROR:
		return "Ошибка размера";
	case LIST_DATA_ERROR:
		return "Ошибка значений list";
	case LIST_FREE_ERROR:
		return "Ошибка свободных ячеек";
	case LIST_SORTED_ERROR:
		return "Ошибка сортировки";
	default: 
		return "неопределенный код ошибки";
	}
}

void ListDumpGraph (const list_t *lst, 
					const char *out_file) {
	assert (lst);
	assert (out_file);
	
	FILE* out = fopen (out_file, "wb");
	if (out == NULL)
		perror ("Открытие выходного файла ListDump\n");
	
	
	
	fclose (out);
}

int ListInsert (list_t *lst, 
				const int index, 
				const list_data_t elem) {
	assert (lst);
	
	LIST_ASSERT_OK (lst);
	
	if (lst->size == lst->max_size) {
		int errno = ListResize (lst, MULTIPLIER_LIST * lst->max_size);
		if (errno != LIST_NO_ERROR) {
			fprintf (stderr, "ERROR: memory allocation during relocation!\n");
			return errno;
		}
	}
	
	if ((lst->prev[index] == FREE_ELEM_PREV) ||
		(index > lst->max_size) || 
		(index < 0)) {
			
		lst->errno = LIST_INDEX_OUT_OF_BOUNDS;
	} else if (lst->head == END_LIST) { // пустой список
		lst->head = lst->free;
		lst->tail = lst->free;
		lst->free = lst->next[lst->free];
		
		lst->data[lst->head] = elem;
		lst->next[lst->head] = END_LIST;
		lst->prev[lst->tail] = END_LIST;
		
		lst->size++;
	} else if (index == lst->tail) { // вставка после хвоста
		int new_tail = lst->free;
		lst->free = lst->next[lst->free];
		
		lst->data[new_tail] = elem;
		lst->next[lst->tail] = new_tail;
		lst->next[new_tail] = END_LIST;
		lst->prev[new_tail] = lst->tail;
		lst->tail = new_tail;
		
		lst->size++;
	} else if (index == 0) { // вставка перед головой
		int new_head = lst->free;
		lst->free = lst->next[lst->free];
		
		lst->data[new_head] = elem;
		lst->prev[lst->head] = new_head;
		lst->prev[new_head] = END_LIST;
		lst->next[new_head] = lst->head;
		lst->head = new_head;
		
		lst->size++;
		lst->sorted = FALSE;
	} else {
		int new_pos = lst->free;
		lst->free = lst->next[lst->free];
		
		lst->data[new_pos] = elem;
		lst->next[new_pos] = lst->next[index];
		lst->next[index] = new_pos;
		
		lst->prev[new_pos] = index;
		lst->prev[lst->next[new_pos]] = new_pos;
		
		lst->size++;
		lst->sorted = FALSE;
	}
	
	LIST_ASSERT_OK (lst);
	
	return lst->errno;
}

int ListRemove (list_t *lst, 
				const int index) {
	assert (lst);
	
	LIST_ASSERT_OK (lst);
	
	if ((lst->prev[index] == FREE_ELEM_PREV) ||
		(index > lst->max_size) || 
		(index <= 0)) {
			
		lst->errno = LIST_INDEX_OUT_OF_BOUNDS;
	} else if (lst->size == 1) {
		lst->data[index] = 0;
		lst->next[lst->head] = lst->free;
		lst->prev[lst->tail] = -1;
		
		lst->free = lst->head;
		lst->head = END_LIST;
		lst->tail = END_LIST;
		lst->size--;
		lst->sorted = TRUE;
	} else if (index == lst->tail) { // удаление хвоста
		lst->data[index] = 0;
		lst->tail = lst->prev[lst->tail];
		lst->prev[index] = -1;
		
		lst->next[index] = lst->free;
		lst->free = index;
		lst->next[lst->tail] = END_LIST;
		
		lst->size--;
	} else if (index == lst->head) { // удаление головы
		lst->data[index] = 0;
		lst->head = lst->next[lst->head];
		lst->next[index] = lst->free;
		lst->free = index;
		
		lst->prev[index] = FREE_ELEM_PREV;
		lst->prev[lst->head] = END_LIST;
		
		lst->sorted = FALSE;
		lst->size--;
	} else {
		lst->data[index] = 0;
		lst->next[lst->prev[index]] = lst->next[index];
		lst->prev[lst->next[index]] = lst->prev[index];
		lst->prev[index] = FREE_ELEM_PREV;
		lst->next[index] = lst->free;
		lst->free = index;
		
		lst->size--;
		lst->sorted = FALSE;
	}
	
	LIST_ASSERT_OK (lst);
	
	return lst->errno;
}

int ListFind (const list_t *lst,
			  list_data_t *elem) {
	assert (lst);
	assert (elem);
	
	LIST_ASSERT_OK (lst);
	
	for (int i = lst->head; i != END_LIST; i = lst->next[i])
		if (ListCompareElem (&(lst->data[i]), elem))
			return i;
	
	return 0;
}

bool ListCompareElem (list_data_t *elem1,
					  list_data_t *elem2) {
	if (*elem1 == *elem2) {
		return TRUE;
	} else {
		return FALSE;
	}
}

list_data_t ListGetElem (list_t *lst,
						 const int index) {
	assert (lst);
	
	if ((index > lst->max_size) || (index <= 0)) {
		lst->errno = LIST_INDEX_OUT_OF_BOUNDS;
		return 0;
	}
	
	return lst->data[index];
}

int ListGetNext (list_t *lst,
				 const int index) {
	assert (lst);
	
	if ((index > lst->max_size) || (index <= 0)) {
		lst->errno = LIST_INDEX_OUT_OF_BOUNDS;
		return -1;
	}
	
	return lst->next[index];
}

int ListGetPrev (list_t *lst,
				 const int index) {
	assert (lst);
	
	if ((index > lst->max_size) || (index <= 0)) {
		lst->errno = LIST_INDEX_OUT_OF_BOUNDS;
		return -1;
	}
	
	return lst->prev[index];
}

int ListSort (list_t *lst) {
	assert (lst);
	
	int i = lst->head;
	int j = 1;
	while (i != END_LIST) {
		if (i != j)
			ListSwap (lst, i, j);
		i = lst->next[j++];
	}
	
	lst->free = j;
	for (; j <= lst->max_size; j++) {
		lst->data[j] = 0;
		lst->next[j] = j + 1;
		lst->prev[j] = FREE_ELEM_PREV;
	}
	lst->next[j] = END_LIST;
	
	lst->sorted = TRUE;
	
	return lst->errno;
}

int ListSwap (list_t *lst, 
			  const int i, 
			  const int j) {
	assert (lst);
	
	if (i > lst->max_size || i <= 0 || j > lst->max_size || j <= 0) {
		lst->errno = LIST_INDEX_OUT_OF_BOUNDS;
		return lst->errno;
	}
	
	if (lst->prev[i] == FREE_ELEM_PREV &&
		lst->prev[j] == FREE_ELEM_PREV) {
		return lst->errno;
	}
	
	if (lst->prev[j] == FREE_ELEM_PREV && 
		lst->prev[i] != FREE_ELEM_PREV) {
		
		int t = i;
		i = j;
		j = t;	
	}
	
	if (lst->prev[i] == FREE_ELEM_PREV &&
		lst->prev[j] != FREE_ELEM_PREV) {
		
		int inext = lst->next[i];
		int jnext = lst->next[j];
		int jprev = lst->prev[j];
		
		lst->data[i] = lst->data[j];
		
		if (j == lst->tail) {
			lst->tail = i;
		} else {
			lst->[jnext] = i;
		}
		
		if (j == lst->head) {
			lst->head = i;
		} else {
			lst->next[jprev] = i;
		}
		lst->next[i] = jnext;
		lst->prev[i] = jprev;
		
		lst->data[j] = END_LIST;
		lst->next[j] = lst->free;
		lst->prev[j] = FREE_ELEM_PREV;
		
		int it = lst->free;
		while (lst->next[it] != i) {
			it = lst->next[it];
		}
		lst->next[it] = inext;
		lst->free = j;
		
		lst->sorted = FALSE;
		
		return lst->errno;
	}
	
	if (lst->prev[i] != FREE_ELEM_PREV && 
		lst->prev[j] != FREE_ELEM_PREV) {
		
		// случай если два элемента идут последовательно 
		if (lst->next[i] == j || lst->next[j] == i) {
			if (lst->next[j] == i) {
				int t = i;
				i = j;
				j = t;
			}
			
			list_data_t t = lst->data[i];
			lst->data[i] = lst->data[j];
			lst->data[j] = t;
			
			int jnext = lst->next[j];
			int iprev = lst->prev[i];
			int new_head = END_LIST; 
			int new_tail = END_LIST;
			
			if (j == lst->tail) {
				new_tail = i;
			} else {
				lst->prev[jnext] = i;
			}
			lst->prev[i] = j;
			lst->next[i] = jnext;
			
			if (i == lst->head) {
				new_head = j;
			} else {
				lst->next[iprev] = j;
			}
			lst->next[j] = i;
			lst->prev[j] = iprev;
			
			if (new_head != END_LIST) {
				lst->head = new_head;
			}
			
			if (new_tail != END_LIST) {
				lst->tail = new_tail;
			}
			
			lst->sorted = FALSE;
			
			return lst->errno;
		}
		
		list_data_t t = lst->data[i];
		lst->data[i] = lst->data[j];
		lst->data[j] = t;
		
		int inext = lst->next[i];
		int jnext = lst->next[j];
		int iprev = lst->prev[i];
		int jprev = lst->prev[j];
		int new_head = END_LIST; 
		int new_tail = END_LIST;
		
		if (j == lst->tail) {
			new_tail = i;
		} else {
			lst->prev[jnext] = i;
		}
		if (j == lst->head) {
			new_tail = i;
		} else {
			lst->next[jprev] = i;
		}
		lst->next[i] = jnext;
		lst->prev[i] = jprev;
		
		if (i == lst->tail) {
			new_tail = j;
		} else {
			lst->prev[inext] = j;
		}
		if (i == lst->head) {
			new_head = j;
		} else {
			lst->next[iprev] = j;
		}
		lst->next[j] = inext;
		lst->prev[j] = iprev;
		
		if (new_head != END_LIST) {
			lst->head = new_head;
		}
		
		if (new_tail != END_LIST) {
			lst->tail = new_tail;
		}
		
		lst->sorted = FALSE;
		
		return lst->errno;
	}
	
	return lst->errno;
}
