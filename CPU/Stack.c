#include "Stack.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

stack_t *StackCreate (char *stk_name) {
	stack_t *out = calloc (1, sizeof(stack_t));

	if (out == NULL)
		exit (STACK_OUT_OF_MEMORY);
	out->size = INIT_ST_SIZE;
	out->data = calloc (out->size, sizeof (elem_t));
	if (out->data == NULL) {
		free (out);
		exit (STACK_OUT_OF_MEMORY);
	}
	out->top = SIZE_CANARY;
	for (int i = 0; i < SIZE_CANARY; i++) {
		out->data[i] = ~out->data[i];
		out->data[out->size - 1 - i] = ~out->data[out->size - 1 - i];
	}
	out->canary_beg = -1;
	out->canary_end = -1;
	out->err_code = 0;
	out->stack_name = calloc (strlen(stk_name), sizeof (char));
	memcpy (out->stack_name, stk_name, strlen(stk_name));
	out->hash = StackHash (out);

	ASSERT_OK(out);
	return out;
}

void StackDelete (stack_t *stk) {
	assert (stk != NULL); 

	if (stk->data != NULL)
		free (stk->data);
	if (stk->stack_name != NULL)
		free (stk->stack_name);
    free (stk);
}

int StackResize (stack_t *stk, int new_size) {
	assert (stk != NULL);
	assert (new_size > 0);
	ASSERT_OK(stk);

	elem_t *p = stk->data;
	stk->size = new_size;
	stk->data = realloc (stk->data, stk->size * sizeof(elem_t));
	if (stk->data == NULL) {
		stk->err_code = STACK_OVERFLOW;
		stk->data = p;
		return 1;
	}
	for (int i = 0; i < SIZE_CANARY; i++)
		stk->data[stk->size - 1 - i] = ~stk->data[stk->size - 1 - i];
	stk->hash = StackHash (stk);

    ASSERT_OK(stk);
    
    return 0;
}

int StackPush (stack_t *stk, elem_t *value) {
	assert (stk != NULL);
	assert (value != NULL);
	ASSERT_OK(stk);

    if (stk->top >= stk->size - SIZE_CANARY)
		if (StackResize (stk, (int)stk->size * MULTIPLIER_STACK))
			return 1;
    stk->data[stk->top++] = *value;
    stk->hash = StackHash (stk);

    ASSERT_OK(stk);
    
    return 0;
}

elem_t StackPop (stack_t *stk) {
	assert (stk != NULL);
	ASSERT_OK(stk);

	if (stk->top <= SIZE_CANARY) {
		stk->err_code = STACK_UNDERFLOW;
		ASSERT_OK(stk);
		return 0;
    }
    
    if (stk->size > INIT_ST_SIZE &&
		stk->top <= (int)stk->size / MULTIPLIER_STACK - DIFFERENCE)
		if (StackResize (stk, (int)stk->size / MULTIPLIER_STACK))
			return 0;
	elem_t elem = stk->data[--stk->top];
	stk->hash = StackHash (stk);

	ASSERT_OK(stk);
	return elem;
}

long int StackHash (const stack_t *stk) {
	assert (stk != NULL);

	long int hash = 0;

	for (int i = 0; i < strlen(stk->stack_name); i++)
		hash += (i + 1) * stk->stack_name[i];
	hash += stk->top + stk->size;
	for (int i = SIZE_CANARY; i < stk->top; i++)
		hash += (i + 1) * stk->data[i];

	return hash;
}

char StackOK (stack_t* stk) {
	assert (stk != NULL);
	
	if (stk->err_code != 0)
		return 0;
		
	if (stk->data == NULL || stk->stack_name == NULL) {
		stk->err_code = STACK_IMPOSSIBLE_POINTERS;
		return 0;
	}
	
	if (stk->top < 0) {
		stk->err_code = STACK_IMPOSSIBLE_VAR_VALUE;
		return 0;
	}

	if (stk->hash != StackHash(stk)) {
		stk->err_code = STACK_ERROR_HASH;
		return 0;
	}

	if (stk->canary_beg != stk->canary_end) {
		stk->err_code = STACK_ERROR_CANARY;
		return 0;
	}

	double_canary_t canary = find_canary(stk);
	if (canary.canary_beg != canary.canary_end) {
		stk->err_code = STACK_ERROR_CANARY;
		return 0;
	}
		
	return 1;
}

void StackDump (const stack_t *stk) {
	assert (stk != NULL);

	double_canary_t canary = find_canary (stk);
	
	FILE* out = fopen (DUMP_FILE_NAME, DUMP_WRITE_MODE);

	fprintf (out, "stack_t \"%s\" [%p] {\n", stk->stack_name, stk);
	fprintf (out, "\tcanary1  = %lli\n", canary.canary_beg);
	fprintf (out, "\tsize     = %li\n", stk->top - SIZE_CANARY);
	fprintf (out, "\tdata[%li] = [%p]\n", stk->size - 2 * SIZE_CANARY, stk->data);
	for (int i = SIZE_CANARY; i < stk->size - SIZE_CANARY; i++)
		fprintf (out, (i < stk->top) ? "\t\t*[%2li]: %li\n" : "\t\t [%2li]: %li\n",
				 i - SIZE_CANARY, stk->data[i]);
	fprintf (out, "\thash     = %li\n", stk->hash);
	fprintf (out, "\tcanary2  = %lli\n", canary.canary_end);
	fprintf (out, "\terrcode  = %i (%s)\n", stk->err_code, 
			(stk->err_code) ? ErrorDefinition (stk->err_code) : "ok");
	fprintf (out, "}\n");
	
	fclose (out);
}

double_canary_t find_canary (const stack_t *stk) {
	assert (stk != NULL);

	canary_union_t can1 = {0};
	canary_union_t can2 = {0};
	for (int i = 0; i < SIZE_CANARY; i++) {
		can1.el[i] = stk->data[i];
		can2.el[i] = stk->data[stk->size - 1 - i];
	}

	double_canary_t canary = {can1.canary, can2.canary};
	return canary;
}

char* ErrorDefinition (int err) {
	switch (err) {
		case STACK_OVERFLOW:
			return "ошибки переполнения стека";
		case STACK_UNDERFLOW: 
			return "ошибки опустошения стека";
		case STACK_OUT_OF_MEMORY:	
			return "ошибки нехватки памяти";
		case STACK_IMPOSSIBLE_VAR_VALUE: 
			return "значение top в стеке < 0";
		case STACK_IMPOSSIBLE_POINTERS: 
			return "указатель равен NULL";
		case STACK_ERROR_HASH: 
			return "ошибка хеширования";
		case STACK_ERROR_CANARY: 
			return "сломалась канарейка";
		default: 
			return "неопределенный код ошибки";
	}
}
