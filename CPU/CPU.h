#ifndef CPU_H
#define CPU_H

//! название файла хранящего код для ассемблирования
#define FILE_IN "code.bin"

//! режим открытия файла
#define READING "r"

//! режим открытия файла
#define READING_BINARY "rb"

//! режим открытия файла
#define WRITING "w"

//! название стека
#define NAME_CPU_STACK "CPUStack"

//! название стека
#define NAME_CPU_FUNCTION_STACK "CPUFunctionStack"

//! количество регистров
#define NUMBER_OF_REGISTERS 4

//! количество элементов в RAM
#define MEMORY_SIZE 16

//! имя файла для cpu damp
#define CPU_DUMP_FILE_NAME "CPUDamp.txt"

//! ложь
#define FALSE 0

//! правда
#define TRUE 1

//----------------------------------------------------------------

//! для хранения данных необходимых для работы CPU
typedef struct CPU {
	stack_t *stk; // стек чисел
	stack_t *fun_stk; // стек для вызова функций
	int *reg;
	int *ram;
	long int *code;
	int iter;
	long int code_size;
	int err;
} CPU_t;
//----------------------------------------------------------------

enum CPUERROR {
	CPU_UNKNOWN_COMMAND,
	CPU_STACK_ERROR,
	CPU_MEMORY_ALLOCATION_ERROR,
	CPU_POINTER_EQUALS_NULL,
	CPU_GOING_BEYOND_CODE_ARR
};

//----------------------------------------------------------------

/*!
 * @brief Приветственные слова
 *
 * @return void
 */
void Epigraph ();

/*!
 * @brief создание структуры CPU
 *
 * @param cpu - указатель на структуру cpu
 *
 * @warning нужно освободить стек командой CPUkDelete (CPU_t*)
 *
 * @return void
 */
void CPUCreate (CPU_t *cpu);

/*!
 * @brief Считывание файла
 *
 * @param buf - буфер с входным файлом
 * @param filename - имя входного файла
 * @param mode - режим открытия входного файла
 *
 * @return число символов в файле
 */
long int ReadFileToBuf (long int **buf,
					   	const char *filename,
						const char *mode);

/*!
 * @brief Реализация ЦП
 *
 * @param cpu - указатель на структуру cpu
 *
 * @return если есть ошибки, то FALSE (0), если нету, то TRUE (1)
 */
int CPUImplementation (CPU_t *cpu);

/*!
 * @brief верификатор CPU
 *
 * @param cpu - указатель на структуру cpu
 *
 * @return если есть ошибки, то FALSE (0), если нету, то TRUE (1)
 */
int CPUOK (CPU_t *cpu);

/*!
 * @brief dump CPU
 *
 * @param cpu - указатель на структуру cpu
 *
 * @return void
 */
void CPUDump (CPU_t *cpu);

/*!
 * @brief По коду возвращает строку которая опысывает эту ошибку
 *
 * @param err код ошибки
 *
 * @return char* описание ошибки
 */
char *ErrorDefinitionCPU (int err);

/*!
 * @brief Удаление структуры CPU
 *
 * @param cpu - указатель на структуру cpu
 *
 * @return void
 */
int CPUDelete (CPU_t *cpu);

/*!
 * @brief Считывание файла
 *
 * @param buf - буфер с входным файлом
 * @param filename - имя входного файла
 * @param mode - режим открытия входного файла
 *
 * @return число символов в файле
 */
long int ReadFileToBuf (long int **buf,
					   	const char *filename,
						const char *mode);

/*!
 * @brief Прощальные слова
 *
 * @return void
 */
void Postscript ();

#endif
