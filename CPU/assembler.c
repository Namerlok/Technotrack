/*!
 * \mainpage
 * \author Загребельный Аркадий
 * \version 0.2
 * \date Ноябрь 2019 года
 *
 * \brief Программа для ассемблирования кода для CPU
 */

#include "encodings.h"
#include "assembler.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h> 
#include <stdio.h>

int main () {
	
	Epigraph ();	
 
	buf_t buf_file = {NULL, 0};
	buf_file.size = ReadFileToBuf (&(buf_file.buf), FILE_IN, READING);
	
	code_t bin = {calloc (START_SIZE_CODE_BIN, sizeof (elem_bin)), 0, START_SIZE_CODE_BIN};
	
	if (Assembler (&buf_file, &bin))
		fprintf (stderr, "Сode assembly error!!!\n");
	
	for (int i = 0; i < bin.size; i++)
		printf ("%li ", bin.data[i]);
	printf ("\n");
		
	WriteFile (&bin, FILE_OUT, WRITING_BINARY);
	
	free (buf_file.buf);	buf_file.buf = NULL;
	free (bin.data);		bin.data = NULL;
	
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

int Assembler (const buf_t *buf,
			   code_t *bin) {
	assert (buf);
	assert (bin);

	marks_t marks = {calloc (START_SIZE_MARK, sizeof(mark_t)), 0, START_SIZE_MARK};

	PreProcessing (buf, &marks, bin);
	
	for (int i = 0; i < marks.size; i++)
		printf ("Function assembler: marks.data[i].name = %s\tpos = %li\n", marks.data[i].name, marks.data[i].pos);
	printf ("\n");
	
	Processing (buf, &marks, bin);
	
	FreeMark (&marks);
	
	printf ("Function Assembler is finish\n"); 
	
	return 0;
}

void PreProcessing (const buf_t *buf,
				 marks_t *marks,
				 code_t *bin) {
	assert (buf);
	assert (marks);
	assert (bin);
	
	bin->size = 0;
	int i = 0;
	SkipSpace (buf, &i); 
	while (i < buf->size) {
		int len = 0;
		SkipSpace (buf, &i); ////////
		#define DEF_CMD(name, encode, argc, code) \
			if (!strncmp(buf->buf + i, #name, len = strlen (#name))) { \
				printf ("Function PreProcessing:" #name "\n"); \
				i += len; \
				bin->size +=  argc + 1;\
				SkipSpace (buf, &i); \
				for (int j = 0; j < argc; j++ ) { \
					SkipWord (buf, &i); \
					SkipSpace (buf, &i); \
				} \
			} else
		#include "commands.h"
		#undef DEF_CMD
		if (buf->buf[i] == ';') { // пропуск коментария
			for (; buf->buf[i] != '\n' && buf->buf[i] != '\0' && i < buf->size; i++) {}
			SkipSpace (buf, &i); ////////
		} else if (IsMark (buf, i)) {
			AddMark (buf, &i, marks, bin);
			SkipSpace (buf, &i);
		}
	}
}

void Processing (const buf_t *buf,
				 marks_t *marks,
				 code_t *bin) {
	assert (buf);
	assert (marks);
	assert (bin);
	
	bin->size = 0;
	int i = 0;
	SkipSpace (buf, &i); 
	while (i < buf->size) {
		int len = 0;
		SkipSpace (buf, &i); ////////
		#define DEF_CMD(name, encode, argc, code) \
			if (!strncmp(buf->buf + i, #name, len = strlen (#name))) { \
				printf ("Function Processing:" #name "\n"); \
				i += len; \
				AddItemBinCode (bin, encode); \
				SkipSpace (buf, &i); \
				int errcode = 0; \
				if ((errcode = ProcArg (buf, &i, marks, bin, argc)) != 0) \
					fprintf (stderr, "Error processing argument(s). Error №:%i\n", errcode); \
				SkipSpace (buf, &i); \
			} else
		#include "commands.h"
		#undef DEF_CMD
		if (buf->buf[i] == ';') { // пропуск коментария
			for (; buf->buf[i] != '\n' && buf->buf[i] != '\0' && i < buf->size; i++) {}
			SkipSpace (buf, &i); ////////
		} else if (IsMark (buf, i)) {
			SkipWord (buf, &i);
			SkipSpace (buf, &i);
		} else {	
			char arg[MAX_SIZE_WORD] = "\0";
			int r_pos = 0;
			sscanf (buf->buf + i, "%[^\n\t ]%n", arg, &r_pos);
			fprintf (stderr, "Unknown command: %s\n", arg);
			return;
		}
	}
}

int ProcArg (const buf_t *buf,
			 int *iter,
			 const marks_t *marks,
			 code_t *bin,
			 const int argc) {
				 
	assert (buf);
	assert (iter);
	assert (marks);
	assert (bin);
	
	int errcode = 0;
	for (int i = 0; i < argc; i++) {
		int r_pos = 0;
		char arg[MAX_SIZE_WORD] = "\0";
		SkipSpace (buf, iter);
		sscanf (buf->buf + *iter, "%[^;:\n\t ]%n", arg, &r_pos);
		printf ("\tFunction ProcArg: iter = %i\targ = %s\tr_pos = %i\n", *iter, arg, r_pos);
		*iter += r_pos;
		int pos_mark = -1;
		if ((pos_mark = FindMark (marks, arg)) != -1) {
			AddItemBinCode (bin, pos_mark);
		} else if (IsStrDigit (arg)) {
			printf ("\tFunction ProcArg: If (IsStrDigit (arg))\n");
			AddItemBinCode (bin, atoi (arg));
		} else if (!strcmp (arg, "ax")) {
			AddItemBinCode (bin, AX);
		} else if (!strcmp (arg, "bx")) {
			AddItemBinCode (bin, BX);
		} else if (!strcmp (arg, "cx")) {
			AddItemBinCode (bin, CX);
		} else if (!strcmp (arg, "dx")) {
			AddItemBinCode (bin, DX);
		} else {
			fprintf (stderr, "Unknown argument\n");
			errcode = 1;
			goto error_exit;
		}
	}
	
	error_exit:
	return errcode;
}

int IsStrDigit (const char* str) {
	
	assert (str);
	
	printf ("\t\tFunction IsStrDigit: str = %s\n", str);
	for (int i = 0; i < strlen(str); i++) {
		if (!isdigit (str[i])) {
			return 0;
		}
	}
	
	return 1;
}

int IsMark (const buf_t *buf,
			int iter) {
	assert (buf);
	
	/*
	SkipSpace (buf, &iter);
	while (!isspace(*(buf->buf + iter)) && iter < buf->size - 1) {
		if (buf->buf[iter] == ':')
			return 1;
		iter++;
	}
	*/
	
	char word[MAX_SIZE_WORD] = "\0";
	int r_pos;
	SkipSpace (buf, &iter);
	sscanf (buf->buf + iter, "%[^;\n\t ]%n", word, &r_pos);
	
//	printf ("Function: IsMark. word = %s, r_pos = %i\n", word, r_pos);
	
	if (word[r_pos - 1] == ':')
		return 1;
	
	return 0;
}

int FindMark (const marks_t *marks,
			  const char *name) {
	assert (marks);
	int pos = -1;
	
	printf ("\t\tFunction FindMark: name marks = %s\n", name);
	for (int i = 0; i < marks->size; i++) {
		printf("\t\tFunction FindMark: strcmp (marks->data[%i].name, name) = %i\n", 
		i, strcmp (marks->data[i].name, name));
		if (!strcmp (marks->data[i].name, name)) {
			printf ("\t\tFunction FindMark: name marks = %s\tmarks->data[i].name = %s\n",
					 name, marks->data[i].name);
			pos = marks->data[i].pos;
			return pos;
		}
	}
	
	return pos;
}

void AddMark (const buf_t *buf,
			  int *iter,
			  marks_t *marks,
			  code_t *bin) {
	assert (buf);
	assert (iter);
	assert (marks);
		
	char mark[MAX_SIZE_WORD] = "\0";
	int r_pos;
	SkipSpace (buf, iter);
	sscanf (buf->buf + *iter, "%[^;:\n\t ]%n", mark, &r_pos);
	
	if (FindMark (marks, mark) != -1) {
		fprintf (stderr, "mark \"%s\" is used twice\n", mark);
		exit (-1);
	}
	
	printf ("\tFunction: AddMark. iter = %i\tmark = %s\tr_pos = %i\n", *iter, mark, r_pos);
	
	(*iter) += r_pos + 1;
	
	if (marks->size >= marks->max_size)
		ResizeMarks (marks, marks->max_size + START_SIZE_MARK);
	
	marks->data[marks->size].name = calloc (strlen (mark) + 1, sizeof (char));
	strcpy (marks->data[marks->size].name, mark);
	marks->data[marks->size++].pos = bin->size;
}

void FreeMark (marks_t *marks) {
	assert (marks);
	
	for (int i = 0; i < marks->size; i++) {
		free (marks->data[i].name);
		marks->data[i].name = NULL;
	}
	free (marks->data);
	marks->data = NULL;
}

int ResizeBinCode (code_t *bin, 
				   int new_size) {
	assert (bin);
	assert (new_size > 0);

	void *p = bin->data;
	bin->max_size = new_size;
	bin->data = realloc (bin->data, bin->max_size * sizeof(*(bin->data)));
	if (bin->data == NULL) {
		bin->data = p;
		return 1;
	}
   
    return 0;
}

int ResizeMarks (marks_t *marks, 
				 int new_size) {
	assert (marks);
	assert (new_size > 0);

	void *p = marks->data;
	marks->max_size = new_size;
	marks->data = realloc (marks->data, marks->max_size * sizeof(*(marks->data)));
	if (marks->data == NULL) {
		marks->data = p;
		return 1;
	}
 
    return 0;
}

int AddItemBinCode (code_t *bin, 
					elem_bin value) {
	assert (bin);
	
	int errcode = 0;
	
	if (bin->size >= bin->max_size)
		errcode = ResizeBinCode (bin, bin->max_size + START_SIZE_CODE_BIN);
		
	bin->data[bin->size++] = value;
	
	return errcode;
}

void SkipSpace (const buf_t *buf,
				int *iter) {
	assert (buf);
	assert (iter);
	
	// пропустить пробельных символов
	for (; isspace (buf->buf[*iter]) && *iter < buf->size; (*iter)++)  {} 
}

void SkipWord (const buf_t *buf,
			   int *iter) {
	assert (buf);
	assert (iter);
	
	int r_pos = 0; \
		sscanf (buf->buf + *iter, "%*[^\n\t ]%n", &r_pos); \
	*iter += r_pos; \

}

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
	
	if (out_code->size != fwrite (out_code->data, sizeof (out_code->data[0]), out_code->size, out)) {
		fprintf (stderr, "The number of recording objects is different from the specified!!!\n");
	}
	
	fclose (out);
}

void Postscript () {

	printf ("\n"
			"##################################################\n"
			"# Program finished working is over               #\n"
			"##################################################\n");
}
