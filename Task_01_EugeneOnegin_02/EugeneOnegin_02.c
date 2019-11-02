/*!
 * \mainpage
 * \author Загребельный Аркадий
 * \version 0.2
 * \date Сентябрь 2019 года
 *
 * \brief Программа для сортировки произвежения Евгений Онегин
 */

#include "Header.h"

/*!
 * @brief Программа для сортировки произведения Евгений Онегин
 *
 * @return Статус выхода из программы
 */
int main() {

	Epigraph();

	long int n_str = 0;
	line_T* strings_it = StringsNovel (FILE_IN, READING_BINARY, &n_str);
	line_T* strings = calloc (n_str, sizeof(line_T));

	memcpy (strings, strings_it, n_str * sizeof(line_T));
	Sorting (strings, n_str, StrcmpAlpha);
	WriteNovel (strings, n_str, FILE_OUT, CREATES);

	memcpy (strings, strings_it, n_str * sizeof(line_T));
	Sorting (strings, n_str, StrcmpRhyme);
	WriteNovel (strings, n_str, FILE_OUT, APPEND);

	WriteNovel (strings_it, n_str, FILE_OUT, APPEND);

	Postscript();

	free (strings);				// освобождаем выделенную память для strings
	free (strings_it[0].str);	// освобождаем выделенную память для buf
	free (strings_it);			// освобождаем выделенную память для strings_it

	return 0;
}

//////////////////////////////////////////////////////////////////

/*!
 * @brief Приветственные слова
 *
 * @return void
 */
void Epigraph () {

	printf ("# Sorting of the novel in verses \"Eugene Onegin\"\n"
			"# Arkadiy, 2019\n"
			"\n");
}

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
						 const char* mode) {

	assert (buf != NULL);
	assert (filename != NULL);
	assert (mode != NULL);

	FILE* in = fopen (filename, mode);
	if (in == NULL)
		perror ("Открытие входного файла\n");

	fseek (in, 0, SEEK_END);
	long int size_f = ftell (in);
	fseek (in, 0, SEEK_SET);

	*buf = calloc (size_f / sizeof(**buf) + 2, sizeof(**buf));
	long int readcount = fread (*buf , sizeof(**buf), size_f / sizeof(**buf), in);
	(*buf)[size_f / sizeof(**buf)] = '\n';
	(*buf)[size_f / sizeof(**buf) + 1] = '\0';

	fclose (in);

	if (readcount != size_f)
		fprintf (stderr, "The number of bytes read differs from the specified\n");

	return readcount;
}

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
					  long int* n_str) {

	assert (filename != NULL);
	assert (mode != NULL);
	assert (n_str != NULL);

	char* buf = NULL;
	long int bufsize = ReadNovelToBuf (&buf, filename, mode);

	for (char* s = memchr (buf, '\n', bufsize); s; s = memchr (s + 1, '\n', bufsize - (s - buf)))
		if (*(s + 1) != '\n' || *(s + 1) != '\r')
			(*n_str)++;

	line_T* strings = calloc (*n_str + 1, sizeof(line_T));
	char* nextpoints = buf;
	int i = 0;
	for (char* s = memchr (buf, '\n', bufsize); s; s = memchr (s + 1, '\n', bufsize - (s - buf))) {
		if (*(s + 1) != '\n' || *(s + 1) != '\r') {
			strings[i].str = nextpoints;
			strings[i++].len = (int)(s + 1 - nextpoints);
			nextpoints = s + 1;
		}
		*s = '\0';
	}

	return strings;
}

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
				 const long int n_str, // передать само значение
				 const char* filename,
				 const char* mode) {

	assert (strings != NULL);
	assert (filename != NULL);
	assert (mode != NULL);

	FILE *out = fopen (filename, mode);
	if (out == NULL)
		perror ("Открытие выходного файла\n");

	fprintf (out, "####################################################\n"
				  "####################################################\n"
				  "\tВвывод произведения\n"
			      "\n");
	for (int i = 0; i < n_str; i++) {
		fprintf (out, "%s\n", strings[i].str);
		if ((i + 1) % STR_STROF == 0 )
			fprintf (out, "\n");
	}

	fclose (out);
}


/*!
 * @brief Меняет местами указатели на строки
 *
 * @param st1 - первый указатель на указатель на строку
 * @param st2 - второй указатель на указатель на строку
 *
 * @return void
 */

void Swap (line_T* st1,
		   line_T* st2) {

	assert (st1 != NULL);
	assert (st2 != NULL);

	int len = st1->len;
	char* str = st1->str;

	st1->str = st2->str;
	st1->len = st2->len;
	st2->str = str;
	st2->len = len;
}

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
				 const line_T* str2) {

	assert (str1->str != NULL);
	assert (str2->str != NULL);
	assert (str1 != NULL);
	assert (str2 != NULL);

	int iter1 = 0, iter2 = 0;

	for (iter1 = 0, iter2 = 0; iter1 < str1->len && iter2 < str2->len; iter1++, iter2++) {
		while (!isalpha (str1->str[iter1]))
			iter1++;
		while (!isalpha (str2->str[iter2]))
			iter2++;

		if (tolower (str1->str[iter1]) != tolower (str2->str[iter2]))
			return tolower (str1->str[iter1]) - tolower (str2->str[iter2]);
	}

	return tolower (str1->str[iter1]) - tolower (str2->str[iter2]);
}

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
				 const line_T* str2) {

	assert (str1->str != NULL);
	assert (str2->str != NULL);
	assert (str1 != NULL);
	assert (str2 != NULL);

	int iter1 = 0, iter2 = 0;

	for (int iter1 = str1->len - 1, iter2 = str2->len - 1; iter1 >= 0 && iter2 >= 0; iter1--, iter2--) {
		while (!isalpha (str1->str[iter1]))
			iter1--;
		while (!isalpha (str2->str[iter2]))
			iter2--;

		if (tolower (str1->str[iter1]) != tolower (str2->str[iter2]))
			return tolower (str1->str[iter1]) - tolower (str2->str[iter2]);
	}

	if (iter1 < 0 &&  iter2 >= 0)
		return -1;
	else if (iter1 >= 0 &&  iter2 < 0)
		return 1;
	else
		return 0;
}

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
					  	  	  	const line_T* str2)) {

	assert (strings != NULL);
	assert (comp != NULL);

	for (int i = 1; i < n_str; i++)
		for (int j = 0; j < n_str - i; j++)
			if ((*comp) (&(strings[j]), &(strings[j + 1])) > 0)
				Swap (&(strings[j]), &(strings[j + 1]));
}

/*!
 * @brief Прощальные слова
 *
 * @return void
 */
void Postscript () {

	printf ("# The sorting of the novel in the verses \"Eugene Onegin\" is over\n"
			"# See the file \"%s\"\n",
			FILE_OUT);
}

