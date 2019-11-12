#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

//----------------------------------------------------------------

//! максимальная длинна входной строки
#define STRING_LENGTH 128

//! максимальная количество входных строк
#define NUMBER_OF_LINES 6500

//! название файла хранящего произведение
#define FILE_IN "EugeneOnegin.txt"

//! название выходного файла
#define FILE_OUT "Result.txt"

//! режим открытия файла
#define CREATES "w"

//! режим открытия файла
#define ATTACHES "a"

//! режим открытия файла
#define READING "r"

//----------------------------------------------------------------

/*!
 * @brief Приветственные слова
 *
 * @return void
 */
void Epigraph();

/*!
 * @brief Считывание романа
 *
 * @param buf - буфер хранящий произведение
 *
 * @return число считанных строк
 */
int read_novel_to_buf (char buf[][STRING_LENGTH]);

/*!
 * @brief запивывает указатели на строки в массив указателей pointers
 *
 * @param buf - двумерный массив с романом
 * @param pointers - массив указателй на строки
 *
 * @return void
 */
void filling_pointers (char* pointers[],
					   char buf[][STRING_LENGTH]);

/*!
 * @brief Вывод произведения
 *
 * @param pointers - масив указателей на выводимые строки
 * @param n_str - количество строк
 * @param file_name - имя выходного файла
 * @param mode - оежим открытия выходного файла
 *
 * @return void
 */
void write_novel (char* pointers[],
				  int n_str,
				  char* file_name,
				  char* mode);

/*!
 * @brief Меняет местами указатели на строки
 *
 * @param p1 - первый указатель на указуказатель на строку
 * @param p2 - второй указатель на указуказатель на строку
 *
 * @return void
 */
void swap_pointers (char** p1,
					char** p2);

/*!
 * @brief сравнение строк по алфавиту
 *
 * @param str1 - первая строка
 * @param str2 - вторая строка
 *
 * @return значение, указывающее результат сравнения след образом
 * 			Нуль:			обе строки равны;
 * 			Боьше нуля: 	строка по адресу str1 больше строки по адресу str2;
 * 			Меньше нуля:	строка по адресу str1 меньше строки по адресу str2;
 */
int strcmp_alpha (char* str1,
				  char* str2);

/*!
 * @brief Сортировка произведения по алфавиту
 *
 * @param pointers - масив указателей, который будет заполнен
 * 						отсортированными указателями на строками
 * @param buf - двумерный массив с романом
 * @param n_str - количество строк в произведении
 *
 * @return void
 */
void sorting_alpha (char* pointers[],
					char buf[][STRING_LENGTH],
					int n_str);

/*!
 * @brief сравнение строк по рифме
 *
 * @param str1 - первая строка
 * @param str2 - вторая строка
 *
 * @return значение, указывающее результат сравнения след образом
 * 			Нуль:			обе строки равны;
 * 			Боьше нуля: 	строка по адресу str1 больше строки по адресу str2;
 * 			Меньше нуля:	строка по адресу str1 меньше строки по адресу str2;
 */
int strcmp_rhyme (char* str1,
				  char* str2);

/*!
 * @brief Сортировка произведения по рифме
 *
 * @param pointers - масив указателей, который будет заполнен
 * 						отсортированными указателями на строками
 * @param buf - двумерный массив с романом
 * @param n_str - количество строк в произведении
 *
 * @return void
 */
void rhyme_sorting (char* pointers[],
					char buf[][STRING_LENGTH],
					int n_str);

/*!
 * @brief Прощальные слова
 *
 * @return void
 */
void postscript ();

//----------------------------------------------------------------


