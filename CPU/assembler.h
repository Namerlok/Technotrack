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

//! для хранения бинарного кода и его длины
struct code_s {
	int *code;
	long int size;
};
typedef struct code_s code_t;

//! для хранения команды и ее хеширования
struct command_s {
	char *com;
	long int hash;	
};
typedef struct command_s command_t;


//! для хранения названия метки и ее позиции в коде
struct mark_s {
	long int name;
	long int pos;
};
typedef struct mark_s mark_t;

//! массив меток с его размером
struct marks_s {
	mark_t *mark;
	long int size;
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
 * @brief Хеширует названия команд
 *
 * @param commands - массив структур с коммандой и хешем
 * @param size - размер массива
 * 
 * @return void
 */
void HashCommands (command_t *commands, 
				   const long int size);

/*!
 * @brief Хеширует любую последовательность не пробельных символов  
 *
 * @param string - строка со словом (хешируется только первое слово)
 * 
 * @return хеш слова
 */
long int HashWord (const char *string);

/*!
 * @brief ассемблирует код по двухпроходной схеме
 *
 * @param buf_code - код для ассемблирования
 * @param bin - ассемблированный код
 *
 * @return int - код ошибки
 */
int Assembler (const buf_t *buf_code, 
			   code_t *bin);

/*!
 * @brief ассемблирует код
 *
 * @param buf_code - код для ассемблирования
 * @param marks - метоки и их позиций в коде
 * @param bin - ассемблированный код
 * @param commands - массив структур с коммандой и хешем
 * @param n_commands - размер массива commands
 *
 * @return void
 */
void Processing (const buf_t *buf_code,
				 marks_t *marks,
				 code_t *bin,
				 const command_t *commands,
				 const int n_commands);

/*!
 * @brief обработка команды прыжка
 *
 * @param buf_code - код для ассемблирования
 * @param iter - указатель на переменную с позицией в буфере
 * @param marks - метоки и их позиций в коде
 * @param bin - ассемблированный код
 * @param encod - кодировака для ассемблирования
 *
 * @return void
 */
void Jump (const buf_t *buf_code,
		   int *iter,
		   const marks_t *marks,
		   code_t *bin,
		   const int encod);

/*!
 * @brief поиск метки
 *
 * @param marks - метоки и их позиций в коде
 * @param hash_name - хеш имени
 *
 * @return позиция метки
 */
int FindMark (const marks_t *marks,
			  const long int hash_name);

/*!
 * @brief обработка новой метки
 *
 * @param buf_code - код для ассемблирования
 * @param iter - указатель на переменную с позицией в буфере
 * @param marks - метоки и их позиций в коде
 * @param bin - ассемблированный код
 *
 * @return void
 */
void JumpMark (const buf_t *buf_code,
			   int *iter,
			   marks_t *marks,
			   const code_t *bin);

/*!
 * @brief вычленяет первое слово из строки 
 *
 * @param string - строка для обработки
 *
 * @return первое слово
 */
char *WordFromString (const char *string);

/*!
 * @brief обработка команды "push"
 *
 * @param buf_code - код для ассемблирования
 * @param iter - указатель на переменную с позицией в буфере
 * @param bin - ассемблированный код
 *
 * @return void
 */
void ProcessingComPush (const buf_t *buf_code,
						int *iter, 
						code_t *bin);

/*!
 * @brief обработка команды "pop"
 *
 * @param buf_code - код для ассемблирования
 * @param iter - указатель на переменную с позицией в буфере
 * @param bin - ассемблированный код
 *
 * @return void
 */
void ProcessingComPop (const buf_t *buf_code,
					   int *iter, 
					   code_t *bin);

/*!
 * @brief вычленяет числа из строки
 *
 * @param string - строка для обработки
 *
 * @return полученное число
 */
int DigitFromString (const char *string);

/*!
 * @brief Прощальные слова
 *
 * @return void
 */
void Postscript ();

#endif
