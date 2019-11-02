/*!
 * \mainpage
 * \author Загребельный Аркадий
 * \version 0.1
 * \date Октябрь 2019 года
 *
 * \brief Реализация динамического стека
 */

#include "Header.h"

/*!
 * @brief Реализация динамического стека
 *
 * @return Статус выхода из программы
 */
int main () {

	Epigraph ();

	stack_t *stk = StackCreat ("stk");

	elem_t value = 10;
	StackPush (stk, &value);
	StackPush (stk, &value);
	StackPush (stk, &value);
	
	printf ("%i\n", StackPop(stk));
	printf ("%i\n", StackPop(stk));
	printf ("%i\n", StackPop(stk));
	printf ("%i\n", StackPop(stk));
	
//	StackDump (stk);

	StackDelete (stk);

	Postscript ();

	return 0;
}

//////////////////////////////////////////////////////////////////

/*!
 * @brief Приветственные слова
 *
 * @return void
 */
void Epigraph () {

	printf ("##################################################\n"
			"# Dynamic stack                                  #\n"
			"# Arkadiy Zagrebelnyy, 2019                      #\n"
			"##################################################\n"
			"\n");
}

/*!
 * @brief Прощальные слова
 *
 * @return void
 */
void Postscript () {

	printf ("\n"
			"##################################################\n"
			"# Program finished working is over               #\n"
			"##################################################\n");
}

/*!
 * @brief создание стека
 *
 * @warning нужно освободить стек командой StackDelete (stack_t*)
 *
 * @return stack_t указатель на созданный стек
 */
stack_t *StackCreat (char *stk_name) {
	stack_t *out = calloc (1, sizeof(stack_t));

	if (out == NULL)
		exit (OUT_OF_MEMORY);
	out->size = INIT_ST_SIZE;
	out->data = calloc (out->size, sizeof (elem_t));
	if (out->data == NULL) {
		free (out);
		exit (OUT_OF_MEMORY);
	}
	out->top = SIZE_CANARY;
	for (int i = 0; i < SIZE_CANARY; i++) {
		out->data[i] = ~out->data[i];
		out->data[out->size - 1 - i] = ~out->data[out->size - 1 - i];
	}
	out->err_code = 0;
	out->stack_name = calloc (strlen(stk_name), sizeof (char));
	memcpy (out->stack_name, stk_name, strlen(stk_name));
	out->hash = StackHash (out);

	ASSERT_OK(out);
	return out;
}

/*!
 * @brief Удаление стека
 *
 * @param stk указатель на удаляемый стек
 *
 * @return void
 */
void StackDelete (stack_t *stk) {
	assert (stk != NULL);

    free (stk->data);
    free (stk->stack_name);
    free (stk);
}

/*!
 * @brief Изменение размера стека
 *
 * @param stk указатель на изменяемый стек
 *
 * @return void
 */
void StackResize (stack_t *stk) {
	assert (stk != NULL);
	ASSERT_OK(stk);

	stk->size *= MULTIPLIER_STACK;
    stk->data = realloc (stk->data, stk->size * sizeof(elem_t));
    if (stk->data == NULL) {
        stk->err_code = STACK_OVERFLOW;
    }
	for (int i = 0; i < SIZE_CANARY; i++)
		stk->data[stk->size - 1 - i] = ~stk->data[stk->size - 1 - i];
	stk->hash = StackHash (stk);

    ASSERT_OK(stk);
}

/*!
 * @brief добавление элемента в стек
 *
 * @param st указатель на стек
 * @param value указатель на новое значение
 *
 * @return void
 */
void StackPush (stack_t *stk, elem_t *value) {
	assert (stk != NULL);
	assert (value != NULL);
	ASSERT_OK(stk);

    if (stk->top >= stk->size - SIZE_CANARY)
    	StackResize (stk);
    stk->data[stk->top++] = *value;
    stk->hash = StackHash (stk);

    ASSERT_OK(stk);
}

/*!
 * @brief считывание элемента из стек с последующим его удалением
 *
 * @param stk указатель на стек
 *
 * @return elem_t указатель на элемент стека
 */
elem_t StackPop (stack_t *stk) {
	assert (stk != NULL);
	ASSERT_OK(stk);

    if (stk->top <= SIZE_CANARY) {
        stk->err_code = STACK_UNDERFLOW;
        ASSERT_OK(stk);
 //       return ///////////////////////////////////////////////////
    }
	elem_t elem = stk->data[--stk->top];
	stk->hash = StackHash (stk);

    ASSERT_OK(stk);
    return elem;
}

/*!
 * @brief хеширование стека
 *
 * @param stk указатель на стек
 *
 * @return long int возвращает Hash
 */
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

/*!
 * @brief Верификатор
 *
 * @param stk указатель на стек
 *
 * @return false or true
 */
char StackOK (stack_t* stk) {
	assert (stk != NULL);
	
	if (stk->err_code != 0)
		return 0;
	
	if (stk->top < 0) {
		stk->err_code = IMPOSSIBLE_VAR_VALUE;
		return 0;
	}

	if (stk->hash != StackHash(stk)) {
		stk->err_code = ERROR_HASH;
		return 0;
	}

	canary_t canary = find_canary(stk);
	if (canary.canary_beg != canary.canary_end) {
		stk->err_code = ERROR_CANARY;
		return 0;
	}
		
	return 1;
}

/*!
 * @brief при возникновении ошибки Dump выводит информации
 *  о стеке в файл "StackDump.txt"
 *
 * @param stk указатель на стек
 *
 * @return void
 */
void StackDump (const stack_t *stk) {
	assert (stk != NULL);

	canary_t canary = find_canary (stk);
	
	FILE* out = fopen (DUMP_FILE_NAME, DUMP_WRITE_MODE);

	fprintf (out, "stack_t \"%s\" [%p] {\n", stk->stack_name, stk);
	fprintf (out, "\tcanary1  = %lli\n", canary.canary_beg);
	fprintf (out, "\tsize     = %li\n", stk->top - SIZE_CANARY);
	fprintf (out, "\tdata[%li] = [%p]\n", stk->size - 2 * SIZE_CANARY, stk->data);
	for (int i = SIZE_CANARY; i < stk->size - SIZE_CANARY; i++) {
		if (i < stk->top)
			fprintf (out, "\t\t*[%2li]: %i\n", i - SIZE_CANARY, stk->data[i]);
		else
			fprintf (out, "\t\t [%2li]: %i\n", i - SIZE_CANARY, stk->data[i]);
	}
	fprintf (out, "\thash     = %li\n", stk->hash);
	fprintf (out, "\tcanary2  = %lli\n", canary.canary_end);
	fprintf (out, "\terrcode  = %i (%s)\n", stk->err_code, 
			(stk->err_code) ? ErrorDefinition (stk->err_code) : "ok");
	fprintf (out, "}\n");
	
	fclose (out);
}

/*!
 * @brief ищет канарейки в стеке
 *
 * @param stk указатель на стек
 *
 * @return canary_t найденные канарейки
 */
canary_t find_canary (const stack_t *stk) {
	assert (stk != NULL);

	canary_enum can1 = {0};
	canary_enum can2 = {0};
	for (int i = 0; i < SIZE_CANARY; i++) {
		can1.el[i] = stk->data[i];
		can2.el[i] = stk->data[stk->size - 1 - i];
	}

	canary_t canary = {can1.canary, can2.canary};
	return canary;
}

/*!
 * @brief По коду возвращает строку которая опысывает эту ошибку
 *
 * @param err код ошибки
 *
 * @return char* описание ошибки
 */
char* ErrorDefinition (const int err) {
	if (err == STACK_OVERFLOW ) 
		return "ошибки переполнения стека";
	else if (err == STACK_UNDERFLOW) 
		return "ошибки опустошения стека";
	else if (err == OUT_OF_MEMORY ) 
		return "ошибки нехватки памяти";
	else if (err == IMPOSSIBLE_VAR_VALUE ) 
		return "значение top в стеке < 0";
	else if (err == ERROR_HASH ) 
		return "ошибка хеширования";
	else if (err == ERROR_CANARY ) 
		return "сломалась канарейка";
	else
		return "неопределенный код ошибки";
}
