/*!
 * \mainpage
 * \author Загребельный Аркадий
 * \version 0.1
 * \date Ноябрь 2019 года
 *
 * \brief Файл с функциями для работы с файлами
 */

#include "WorkWithFiles.h"
#include "assembler.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

long int ReadFileToBuf (char** buf,
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
	if (*buf == NULL)
		fprintf (stderr, "Сannot allocate memory to create file buffer!!!\n");
	long int readcount = fread (*buf , sizeof(**buf), size_f / sizeof(**buf), in);
	(*buf)[size_f / sizeof(**buf)] = '\n';
	(*buf)[size_f / sizeof(**buf) + 1] = '\0';

	fclose (in);

	if (readcount != size_f)
		fprintf (stderr, "The number of bytes read differs from the specified!!!\n");

	return readcount;
}

line_t* StringsFile (const char* filename,
					 const char* mode,
					 long int* n_str) {

	assert (filename != NULL);
	assert (mode != NULL);
	assert (n_str != NULL);

	char* buf = NULL;
	long int bufsize = ReadFileToBuf (&buf, filename, mode);

	for (char* s = memchr (buf, '\n', bufsize); s; s = memchr (s + 1, '\n', bufsize - (s - buf)))
		if (*(s + 1) != '\n' || *(s + 1) != '\r')
			(*n_str)++;

	line_t* strings = calloc (*n_str + 1, sizeof(line_t));
	if (strings == NULL)
		fprintf (stderr, "Сannot allocate memory to create file line buffer!!!\n");
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

void WriteFile (code_t *out_code,
				const char *filename,
				const char *mode) {

	assert (out_code != NULL);
	assert (filename != NULL);
	assert (mode != NULL);

	FILE *out = fopen (filename, mode);
	if (out == NULL) {
		perror ("Открытие выходного файла\n");
	}
	
	if (out_code->size != fwrite (out_code->code, sizeof (out_code->code[0]), out_code->size, out)) {
		fprintf (stderr, "The number of recording objects is different from the specified!!!\n");
	}
	
	fclose (out);
}
