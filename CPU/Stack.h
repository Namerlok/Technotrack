#ifndef STACK_H
#define STACK_H

#include <stdio.h>

//! начальный размер массива стека
#define INIT_ST_SIZE 30

//! размер канарейки в стеке
#define SIZE_CANARY sizeof(long long int)/sizeof(elem_t)

//! множитель для изменения размера массива стека
#define MULTIPLIER_STACK 2

//! разница между числом для увеличения и для уменьшения стека
#define DIFFERENCE 5

//! название выходного файла dump
#define DUMP_FILE_NAME "StackDump.txt"

//! модификация открыия файла в StackDump
#define DUMP_WRITE_MODE "w"

//! Коды ошибок
enum STACK_EROOR_CODE {
	STACK_OVERFLOW				= 1,
	STACK_UNDERFLOW				= 2,
	STACK_OUT_OF_MEMORY			= 3,
	STACK_IMPOSSIBLE_VAR_VALUE	= 4,
	STACK_IMPOSSIBLE_POINTERS	= 5,
	STACK_ERROR_HASH			= 6,
	STACK_ERROR_CANARY			= 7,
	STACK_MAX_EL
};

//! условная коомпиляция для мониторинга ошибок стека
#ifdef NDEBUG
	#define ASSERT_OK(stk)
#else
	#define ASSERT_OK(stk) if (!StackOK (stk)) {\
						   	   StackDump (stk);\
						   	   assert (!"StackOK");\
						   }
#endif

//----------------------------------------------------------------

//! тип элемента стека
typedef long int elem_t;

//! сруктура стека
typedef struct stack {
	long long int canary_beg;

	char* stack_name;

	elem_t* data;
	size_t size;
	int top;

	int err_code;
	long int hash;

	long long int canary_end;
} stack_t;

typedef struct double_canary {
	long long int canary_beg;
	long long int canary_end;
} double_canary_t ;

typedef union canary_union {
	elem_t el[SIZE_CANARY];
	long long int  canary;
} canary_union_t;

//----------------------------------------------------------------

/*!
 * @brief создание стека
 *
 * @warning нужно освободить стек командой StackDelete (stack_t*)
 *
 * @return stack_t указатель на созданный стек
 */
stack_t* StackCreate (char *stk_name);

/*!
 * @brief Удаление стека
 *
 * @param stk указатель на удаляемый стек
 *
 * @return void
 */
void StackDelete (stack_t *stk);

/*!
 * @brief Изменение размера стека
 *
 * @param stk указатель на изменяемый стек
 * @param new_size новый размер стека
 *
 * @return int наличие ошибки
 */
int StackResize (stack_t *stk, int new_size);

/*!
 * @brief добавление элемента в стек
 *
 * @param stk указатель на стек
 * @param value указатель на новое значение
 *
 * @return int наличие ошибки
 */
int StackPush (stack_t *stk, elem_t* value);

/*!
 * @brief считывание элемента из стек с последующим его удалением
 *
 * @param stk указатель на стек
 *
 * @return elem_t элемент стека
 */
elem_t StackPop (stack_t *stk);

/*!
 * @brief Верификатор
 *
 * @param stk указатель на стек
 *
 * @return false or true
 */
char StackOK (stack_t* stk);

/*!
 * @brief при возникновении ошибки Dump выводит информации
 *  о стеке в файл "StackDump.txt"
 *
 * @param stk указатель на стек
 *
 * @return void
 */
void StackDump (const stack_t *stk);

/*!
 * @brief хеширование стека
 *
 * @param stk указатель на стек
 *
 * @return long int возвращает Hash
 */
long int StackHash (const stack_t *stk);

/*!
 * @brief поиск канареек в стеке 
 *
 * @param stk указатель на стек
 *
 * @return double_canary_t найденные канарейки
 */
double_canary_t find_canary (const stack_t *stk);

/*!
 * @brief По коду возвращает строку которая опысывает эту ошибку
 *
 * @param err код ошибки
 *
 * @return char* описание ошибки
 */
char* ErrorDefinition (int err);

#endif
