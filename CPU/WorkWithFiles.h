#ifndef WORKWITHFILES_H
#define WORKWITHFILES_H

/* название входного и выходного файлов объявляются в вызываемом файле
//! название файла хранящего произведение
#define FILE_IN "EugeneOnegin.txt"

//! название выходного файла
#define FILE_OUT "Result.txt"
*/
//! режим открытия файла
#define CREATES "w"

//! режим открытия файла
#define APPEND "a"

//! режим открытия файла
#define READING "r"

//! режим открытия файла
#define READING_BINARY "rb"

//! режим открытия файла
#define WRITING_BINARY "wb"

//----------------------------------------------------------------
struct line {
	char *str;
	int len;
};
typedef struct line line_t;

//----------------------------------------------------------------

/*!
 * @brief Считывание файла
 *
 * @param buf - буфер с входным файлом
 * @param filename - имя входного файла
 * @param mode - режим открытия входного файла
 *
 * @return число символов в файле
 */
long int ReadFileToBuf (char** buf,
					   	const char* filename,
						const char* mode);

/*!
 * @brief заполняет массив структур указателями на строки входного файла и их длиннами
 *
 * @param buf - буфер хранящий входной файл
 * @param filename - имя входного файла
 * @param mode - режим открытия входного файла
 * @param n_str - для записи количества строчек во входном файле
 *
 * @warning нужно освободить возвращаемый указатель командой free()
 * @warning	нужно освободить buf командой free (strings_it[0].str) (где strings_it возврацаемый указатель)
 *
 * @return масив структур
 */
line_t *StringsFile (const char* filename,
					 const char* mode,
					 long int* n_str);

/*!
 * @brief Вывод массива
 *
 * @param buffer - масcив указателей на выводимые строки
 * @param buf_length - длинна массива
 * @param filename - имя выходного файла
 * @param mode - режим открытия выходного файла
 *
 * @return void
 */
void WriteFile (code_t *out_code,
				const char *filename,
				const char *mode);

#endif


