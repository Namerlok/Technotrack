#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

//! название файла хранящего код для ассемблирования
#define FILE_IN "code.bin"

//! название выходного файла
#define FILE_OUT "code_disasm.txt"

//! режим открытия файла
#define WRITING "w"

//! режим открытия файла
#define APPEND "a"

//! режим открытия файла
#define READING "r"

//! режим открытия файла
#define READING_BINARY "rb"

//! режим открытия файла
#define WRITING_BINARY "wb"

//----------------------------------------------------------------

//! для хранения кода и его длины
struct buf_s {
	long int *buf;
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
 * @brief дизассемблирует код
 *
 * @param buf - код для дизассемблирования
 *
 * @return int - код ошибки
 */
int Disassembler (const buf_t *buf);

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
