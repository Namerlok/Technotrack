#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

//----------------------------------------------------------------

//! название файла хранящего произведение
#define FILE_IN "EugeneOnegin.txt"

//! название выходного файла
#define FILE_OUT "Result.txt"

//! режим открытия файла
#define CREATES "w"

//! режим открытия файла
#define APPEND "a"

//! режим открытия файла
#define READING "r"

//! режим открытия файла
#define READING_BINARY "rb"

//! число строк в одной строфе
#define STR_STROF 14

//----------------------------------------------------------------

//! название файла хранящего произведение
//const char* FILE_IN = "EugeneOnegin.txt";
/*
//! название выходного файла
const char* FILE_OUT = "Result.txt";

//! режим открытия файла
const char* CREATES = "w";

//! режим открытия файла
const char* APPEND = "a";

//! режим открытия файла
const char* READING = "r";

//! режим открытия файла
const char* READING_BINARY = "rb";
*/
//----------------------------------------------------------------
struct line {
	char *str;
	int len;
};
typedef struct line line_T;

//----------------------------------------------------------------

/*!
 * @brief Приветственные слова
 *
 * @return void
 */
void Epigraph ();


/*!
 * @brief Считывание романа
 *
 * @param buf - буфер хранящий произведение
 * @param filename - имя входного файла
 * @param mode - режим открытия входного файла
 *
 * @return число символов
 */
long int ReadNovelToBuf (char** buf,
					   	 const char* filename,
						 const char* mode);

/*!
 * @brief заполняет массив структур указателями на строки романа и их длиннами
 *
 * @param buf - буфер хранящий произведение
 * @param filename - имя входного файла
 * @param mode - режим открытия входного файла
 * @param n_str - количество строчек в произведении
 *
 * @warning нужно освободить возвращаемый указатель командой free()
 * @warning	нужно освободить buf командой free (strings_it[0].str) (где strings возврацаемый указатель)
 *
 * @return масив структур
 */
line_T* StringsNovel (const char* filename,
					  const char* mode,
					  long int* n_str);

/*!
 * @brief Вывод произведения
 *
 * @param strings - масив указателей на выводимые строки
 * @param n_str - количество строк
 * @param filename - имя выходного файла
 * @param mode - оежим открытия выходного файла
 *
 * @return void
 */
void WriteNovel (const line_T* strings,
				 const long int n_str,
				 const char* filename,
				 const char* mode);

/*!
 * @brief Меняет местами указатели на строки
 *
 * @param st1 - первый указатель на указатель на строку
 * @param st2 - второй указатель на указатель на строку
 *
 * @return void
 */
void Swap (line_T* st1,
		   line_T* st2);

/*!
 * @brief сравнение строк по алфавиту
 *
 * @param str1 - структура со строкой и ее длиной
 * @param str2 - структура со строкой и ее длиной
 *
 * @return значение, указывающее результат сравнения след образом
 * 			Нуль:			обе строки равны;
 * 			Боьше нуля: 	строка по адресу str1 больше строки по адресу str2;
 * 			Меньше нуля:	строка по адресу str1 меньше строки по адресу str2;
 */
int StrcmpAlpha (const line_T* str1,
				 const line_T* str2);

/*!
 * @brief сравнение строк по рифме
 *
 * @param str1 - структура со строкой и ее длиной
 * @param str2 - структура со строкой и ее длиной
 *
 * @return значение, указывающее результат сравнения след образом
 * 			Нуль:			обе строки равны;
 * 			Боьше нуля: 	строка по адресу str1 больше строки по адресу str2;
 * 			Меньше нуля:	строка по адресу str1 меньше строки по адресу str2;
 */
int StrcmpRhyme (const line_T* str1,
				 const line_T* str2);

/*!
 * @brief Сортировка произведения по алфавиту
 *
 * @param strings - масив структур, хранящий указатели на строки и их длины
 * @param n_str - количество строк в произведении
 * @param comp - указатель на функцию сравнения
 *
 * @return void
 */
void Sorting (line_T* strings,
			  const long int n_str,
			  const int (*comp)(const line_T* str1,
					  	  	  	const line_T* str2));


/*!
 * @brief Прощальные слова
 *
 * @return void
 */
void Postscript ();

//----------------------------------------------------------------


