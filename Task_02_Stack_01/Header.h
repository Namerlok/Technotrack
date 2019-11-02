#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

//----------------------------------------------------------------

//! начальный размер массива стека
#define INIT_ST_SIZE 30

//! размер канарейки в стеке
#define SIZE_CANARY sizeof(long long int)/sizeof(elem_t)

//! множитель для изменения размера массива стека
#define MULTIPLIER_STACK 2

//! код ошибки переполнения стека
#define STACK_OVERFLOW 1

//! код ошибки опустошения стека
#define STACK_UNDERFLOW 2

//! код ошибки нехватки памяти
#define OUT_OF_MEMORY 3

//! значение top в стеке < 0
#define IMPOSSIBLE_VAR_VALUE 4

//! ошибка хеширования
#define ERROR_HASH 5

//! сломалась канарейка
#define ERROR_CANARY 6

//! название выходного файла dump
#define DUMP_FILE_NAME "StackDump.txt"

//! модификация открыия файла в StackDump
#define DUMP_WRITE_MODE "w"

//! условная коомпиляция для мониторинга ошибок стека
#ifdef DEBUG
	#define ASSERT_OK(stk) if (!StackOK (stk)) {\
						   	   StackDump (stk);\
						   	   assert (!"StackOK");\
						   }
#else
	#define ASSERT_OK(stk)
#endif

//----------------------------------------------------------------

//! тип элемента стека
typedef int elem_t;

//! сруктура стека
typedef struct stack {
	char* stack_name;

	elem_t* data;
	size_t size;
	int top;

	int err_code;
	long int hash;
} stack_t;

typedef struct {
	long long int canary_beg;
	long long int canary_end;
} canary_t;

typedef union {
	elem_t el[SIZE_CANARY];
	long long int  canary;
} canary_enum;

//----------------------------------------------------------------

//////////////////////////////////////////////////////
/*!
 * @brief Приветственные слова
 *
 * @return void
 */
void Epigraph ();

/*!
 * @brief Прощальные слова
 *
 * @return void
 */
void Postscript ();
///////////////////////////////////////////////////////

/*!
 * @brief создание стека
 *
 * @warning нужно освободить стек командой StackDelete (stack_t*)
 *
 * @return stack_t указатель на созданный стек
 */
stack_t* StackCreat (char *stk_name);

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
 *
 * @return void
 */
void StackResize (stack_t *stk);

/*!
 * @brief добавление элемента в стек
 *
 * @param stk указатель на стек
 * @param value указатель на новое значение
 *
 * @return void
 */
void StackPush (stack_t *stk, elem_t* value);

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
 * @brief при возникновении ошибки Dump выводит информации
 *  о стеке в файл "StackDump.txt"
 *
 * @param stk указатель на стек
 *
 * @return void
 */
canary_t find_canary (const stack_t *stk);

/*!
 * @brief По коду возвращает строку которая опысывает эту ошибку
 *
 * @param err код ошибки
 *
 * @return char* описание ошибки
 */
char* ErrorDefinition (const int err);
