/*!
 * \mainpage
 * \author Загребельный Аркадий
 * \version 0.1
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

	char buf[NUMBER_OF_LINES][STRING_LENGTH];
	int n_str = read_novel_to_buf (buf);

	char* pointers[NUMBER_OF_LINES];

	sorting_alpha (pointers, buf, n_str);
	write_novel (pointers, n_str, FILE_OUT, CREATES);

	rhyme_sorting (pointers, buf, n_str);
	write_novel (pointers, n_str, FILE_OUT, ATTACHES);

	filling_pointers (pointers, buf);
	write_novel (pointers, n_str, FILE_OUT, ATTACHES);

	postscript ();

	return 0;
}

//////////////////////////////////////////////////////////////////

/*!
 * @brief Приветственные слова
 *
 * @return void
 */
void Epigraph() {

	printf ("# Sorting of the novel in verses \"Eugene Onegin\"\n"
			"# Arkadiy, 2019\n"
			"\n");
}

/*!
 * @brief Считывание романа
 *
 * @param buf - буфер хранящий произведение
 *
 * @return число считанных строк
 */
int read_novel_to_buf (char buf[][STRING_LENGTH]) {

	assert (buf != NULL);

	FILE *in = fopen (FILE_IN, READING);
	if (in == NULL)
		perror ("Открытие входного файла\n");

	int n_str = 0;
	while (fgets (buf[n_str], STRING_LENGTH, in) != NULL &&
			n_str < NUMBER_OF_LINES) {
		int len = strlen (buf[n_str]), iter = 0;
		buf[n_str][len - 1] = '\0';
		while (!isalpha (buf[n_str][iter]) && buf[n_str][iter] != '\0')
			iter++;
		if (buf[n_str][iter] != '\0')
			n_str++;
	}

	fclose (in);
	return n_str;
}

/*!
 * @brief запивывает указатели на строки в массив указателей pointers
 *
 * @param buf - двумерный массив с романом
 * @param pointers - массив указателй на строки
 *
 * @return void
 */
void filling_pointers (char* pointers[],
					   char buf[][STRING_LENGTH]) {

	assert (buf != NULL);
	assert (pointers != NULL);

	for (int i = 0; i < NUMBER_OF_LINES; i++) {
		pointers[i] = buf[i];
	}
}

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
				  char* mode) {

	assert (pointers != NULL);
	assert (file_name != NULL);
	assert (mode != NULL);

	FILE *out = fopen (file_name, mode);
	if (out == NULL)
		perror ("Открытие выходного файла\n");

	fprintf (out, "####################################################\n"
				  "\tВвывод произведения\n"
			      "\n");
	for (int i = 0; i < n_str; i++) {
		fprintf (out, "%s\n", pointers[i]);
		if ((i + 1) % 14 == 0 )
			fprintf (out, "\n");
	}

	fclose (out);
}

/*!
 * @brief Меняет местами указатели на строки
 *
 * @param p1 - первый указатель на указуказатель на строку
 * @param p2 - второй указатель на указуказатель на строку
 *
 * @return void
 */
void swap_pointers (char** p1,
					char** p2) {

	assert (p1 != NULL);
	assert (p2 != NULL);

	char* swap = NULL;

	swap = *p1;
	*p1 = *p2;
	*p2 = swap;
}

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
				  char* str2) {

	assert (str1 != NULL);
	assert (str2 != NULL);

	int len1 = strlen (str1);
	int len2 = strlen (str2);
	int iter1 = 0, iter2 = 0;

	for (iter1 = 0, iter2 = 0; iter1 < len1 && iter2 < len2; iter1++, iter2++) {
		while (!isalpha (str1[iter1]))
			iter1++;
		while (!isalpha (str2[iter2]))
			iter2++;

		if (tolower (str1[iter1]) != tolower (str2[iter2]))
			return tolower (str1[iter1]) - tolower (str2[iter2]);
	}

	if (iter1 == len1 &&  iter2 < len2)
		return -1;
	else if (iter1 < len1 &&  iter2 == len2)
		return 1;
	else
		return 0;
}

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
					int n_str) {

	assert (buf != NULL);
	assert (pointers != NULL);

	filling_pointers (pointers, buf);

	for (int i = 1; i < n_str; i++)
		for (int j = 0; j < n_str - i; j++)
			if (strcmp_alpha (pointers[j], pointers[j + 1]) > 0)
				swap_pointers (&pointers[j], &pointers[j + 1]);
	//////////////////////////////скобки
}

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
				  char* str2) {

	assert (str1 != NULL);
	assert (str2 != NULL);

	int len1 = strlen (str1);
	int len2 = strlen (str2);
	int iter1 = 0, iter2 = 0;

	for (int iter1 = len1 - 1, iter2 = len2 - 1; iter1 >= 0 && iter2 >= 0; iter1--, iter2--) {
		while (!isalpha (str1[iter1]))
			iter1--;
		while (!isalpha (str2[iter2]))
			iter2--;

		if (tolower (str1[iter1]) != tolower (str2[iter2]))
			return tolower (str1[iter1]) - tolower (str2[iter2]);
	}

	if (iter1 == 0 &&  iter2 > 0)
		return -1;
	else if (iter1 > 0 &&  iter2 == 0)
		return 1;
	else
		return 0;
}

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
					int n_str) {

	assert (buf != NULL);
	assert (pointers != NULL);

	filling_pointers (pointers, buf);

	for (int i = 1; i < n_str; i++)
		for (int j = 0; j < n_str - i; j++)
			if (strcmp_rhyme (pointers[j], pointers[j + 1]) > 0)
				swap_pointers (&pointers[j], &pointers[j + 1]);
}

/*!
 * @brief Прощальные слова
 *
 * @return void
 */
void postscript () {

	printf ("# The sorting of the novel in the verses \"Eugene Onegin\" is over\n"
			"# See the file \"%s\"\n",
			FILE_OUT);
}
