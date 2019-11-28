#ifndef ASSEMBLER_H
#define ASSEMBLER_H
//----------------------------------------------------------------

//! название файла хранящего код для ассемблирования
#define FILE_IN "code.txt"

//! название выходного файла
#define FILE_OUT "code.bin"

//! начальный размер массива храшящего ассемблированный код
#define START_SIZE_CODE_BIN 64

//! начальный размер массива храшящего метки
#define START_SIZE_MARK 16

//! множитель для изменения размера массива меток и ас. кода
#define MULTIPLIER_ARR 2

//! максимальный размер слова
#define MAX_SIZE_WORD 128


//----------------------------------------------------------------

typedef long int elem_bin;

//! для хранения бинарного кода и его длины
struct code_s {
	elem_bin *data;
	long int size;
	long int max_size;
};
typedef struct code_s code_t;

//! для хранения названия метки и ее позиции в коде
struct mark_s {
	char *name;
	long int pos;
};
typedef struct mark_s mark_t;

//! массив меток с его размером
struct marks_s {
	mark_t *data;
	long int size;
	long int max_size;
};
typedef struct marks_s marks_t;

//! для хранения кода и его длины
struct buf_s {
	char *buf;
	long int size;
};
typedef struct buf_s buf_t;

//----------------------------------------------------------------

/*!
 * @brief Приветственные слова
 *
 * @return void
 */
void Epigraph ();

/*!
 * @brief ассемблирует код по двухпроходной схеме
 *
 * @param buf - код для ассемблирования
 * @param bin - ассемблированный код
 *
 * @return int - код ошибки
 */
int Assembler (const buf_t *buf, 
			   code_t *bin);
/*!
 * @brief обработка меток
 *
 * @param buf - код для ассемблирования
 * @param marks - метоки и их позиций в коде
 * @param bin - ассемблированный код
 *
 * @return void
 */
void PreProcessing (const buf_t *buf,
				 marks_t *marks,
				 code_t *bin);

/*!
 * @brief ассемблирует код
 *
 * @param buf - код для ассемблирования
 * @param marks - метоки и их позиций в коде
 * @param bin - ассемблированный код
 *
 * @return void
 */
void Processing (const buf_t *buf,
				 marks_t *marks,
				 code_t *bin);

/*!
 * @brief обработка аргументов команды
 *
 * @param buf - код для ассемблирования
 * @param iter - указатель на переменную с позицией в буфере
 * @param marks - метоки и их позиций в коде
 * @param bin - ассемблированный код
 * @param argc - количество аргументов
 * 
 * @return void
 */
int ProcArg (const buf_t *buf,
			 int *iter,
			 const marks_t *marks,
			 code_t *bin,
			 const int argc);

/*!
 * @brief проверяет является ли эта строка числом
 *
 * @param str - строка
 *
 * @return позиция метки
 */
int IsStrDigit (const char *str);

/*!
 * @brief проверяет является ли следующее слово в строке buf меткой
 *
 * @param buf - код для ассемблирования
 * @param iter - указатель на переменную с позицией в буфере
 *
 * @return позиция метки
 */
int IsMark (const buf_t *buf,
			int iter);
			
/*!
 * @brief поиск метки
 *
 * @param marks - метоки и их позиций в коде
 * @param name - именя метки
 *
 * @return позиция метки
 */
int FindMark (const marks_t *marks,
			  const char *name);

/*!
 * @brief добавление новой метки
 *
 * @param buf - код для ассемблирования
 * @param iter - указатель на переменную с позицией в буфере
 * @param marks - метоки и их позиций в коде
 * 
 * @return позиция метки
 */
void AddMark (const buf_t *buf,
			  int *iter,
			  marks_t *marks,
			  code_t *bin);

/*!
 * @brief добавление новой метки
 *
 * @param marks - удаление структуры меток
 * 
 * @return позиция метки
 */
void FreeMark (marks_t *marks);

/*!
 * @brief изменение размера массива бинарного кода
 *
 * @param bin - ассемблированный код
 * @param new_size - новый размер
 *
 * @return код ошибки
 */
int ResizeBinCode (code_t *marks, 
				   int new_size);

/*!
 * @brief изменение размера массива меток
 *
 * @param code - метки
 * @param new_size - новый размер
 *
 * @return код ошибки
 */
int ResizeMarks (marks_t *code, 
				 int new_size);

/*!
 * @brief добавление нового элемента в бинарный код
 *
 * @param bin - ассемблированный код
 * @param value - новое значение
 *
 * @return код ошибки
 */
int AddItemBinCode (code_t *bin, 
					elem_bin value);

/*!
 * @brief пропуск пробельных символов 
 *
 * @param buf_code - код для ассемблирования
 * @param iter - указатель на переменную с позицией в буфере
 *
 * @return void
 */
void SkipSpace (const buf_t *buf,
				int *iter);

/*!
 * @brief пропуск любой последовательности не пробельных символов 
 *
 * @param buf_code - код для ассемблирования
 * @param iter - указатель на переменную с позицией в буфере
 *
 * @return void
 */
void SkipWord (const buf_t *buf,
				int *iter);

/*!
 * @brief Прощальные слова
 *
 * @return void
 */
void Postscript ();

#endif
