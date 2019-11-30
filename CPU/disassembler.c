/*!
 * \mainpage
 * \author Загребельный Аркадий
 * \version 0.1
 * \date Ноябрь 2019 года
 *
 * \brief Программа для дизассемблирования кода
 */
 
#include "encodings.h"
#include "disassembler.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h> 
#include <stdio.h>

int main () {
	
	Epigraph ();
	
	buf_t buf = {NULL, 0};
	buf.size = ReadFileToBuf (&(buf.buf), FILE_IN, READING_BINARY);
	
	if (Disassembler (&buf))
		fprintf (stderr, "Сode disassembly error!!!\n");
	
	free (buf.buf); 	buf.buf = NULL;
	
	Postscript ();
	
	return 0;
}

void Epigraph () {

	printf ("##################################################\n"
			"# Assembler                                      #\n"
			"# Arkadiy Zagrebelnyy, 2019                      #\n"
			"##################################################\n"
			"\n");
}

int Disassembler (const buf_t *buf) {
	assert (buf);
	
	FILE *out = fopen (FILE_OUT, WRITING_BINARY);
	if (out == NULL) {
		perror ("Открытие выходного файла\n");
	}
	
	int i = 0;
	while (i < buf->size) {
		switch (buf->buf[i]) {
		#define DEF_CMD(name, encode, argc, code) \
		case encode: \
			fprintf (out, #name " "); \
			i++; \
			for (int j = 0; j < argc; j++) \
				fprintf (out, "%li ", buf->buf[i++]); \
			fprintf (out, "\n"); \
			break;
		#include "commands.h"
		#undef DEF_CMD
		default:
			fclose (out);
			return 1;
		}
	}
	fclose (out);
	
	return 0;
}

long int ReadFileToBuf (long int **buf,
					   	const char *filename,
						const char *mode) {

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

void Postscript () {

	printf ("\n"
			"##################################################\n"
			"# Program finished working is over               #\n"
			"##################################################\n");
}


