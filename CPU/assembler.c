/*!
 * \mainpage
 * \author Загребельный Аркадий
 * \version 0.1
 * \date Ноябрь 2019 года
 *
 * \brief Программа для ассемблирования кода для CPU
 */

#include "WorkWithFiles.h"
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
		
	WriteFile (&bin, FILE_OUT, WRITING);
	
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

void HashCommands (command_t *commands, 
				   const long int size) {
	assert (commands != NULL);
	
	for (int i = 0; i < size; i++)
		commands[i].hash = HashWord (commands[i].com);
}

long int HashWord (const char *string) {
	assert (string != NULL);
	
	long int hash = 0;
	for (int i = 0; !isspace (string[i]) && i < strlen (string); i++)
			hash += string[i] * (i + 1);
	
	return hash;
}

int Assembler (const buf_t *buf_code,
			   code_t *bin) {
	assert (buf_code != NULL);
	assert (bin != NULL);
	
	command_t commands[] = {{"add", 0},
							{"sub", 0},
							{"mul", 0},
							{"div", 0},
							{"sqrt", 0},
							{"sin", 0},
							{"cos", 0},
							{"end", 0},
							{"push", 0},
							{"pop", 0},
							{"jmp", 0},
							{"ja", 0},
							{"jae", 0},
							{"jb", 0},
							{"jbe", 0},
							{"je", 0},
							{"jne", 0},
							{"<-", 0},
							{"call", 0},
							{";", 0},
							{"function", 0},
							{"return", 0}
						   };

	HashCommands (commands, sizeof (commands) / sizeof (command_t));

	marks_t marks = {calloc (START_SIZE_MARK, sizeof(mark_t)), 0, START_SIZE_MARK};

	Processing (buf_code, &marks, bin, commands, (int)sizeof(commands) / (int)sizeof(command_t));
	Processing (buf_code, &marks, bin, commands, (int)sizeof(commands) / (int)sizeof(command_t));
	
	free (marks.mark);
	
	return 0;
}

void Processing (const buf_t *buf_code,
				 marks_t *marks,
				 code_t *bin,
				 const command_t *commands,
				 const int n_commands) {
	assert (buf_code != NULL);
	assert (marks != NULL);
	assert (bin != NULL);
	assert (commands != NULL);
	
	bin->size = 0;
	int i = 0;
	while (i < buf_code->size) {
		for (; isspace (buf_code->buf[i]) && i < buf_code->size; i++) {}
		if (i >= buf_code->size)
			break;
		long int hash = HashWord (&(buf_code->buf[i]));
		if (hash == commands[0].hash) { //"add"
			AddItemBinCode (bin, ADD);
			i += strlen (commands[0].com); // пропустить название команды
		} else if (hash == commands[1].hash) { //"sub"
			AddItemBinCode (bin, SUB);
			i += strlen (commands[1].com); // пропустить название команды
		} else if (hash == commands[2].hash) { //"mul"
			AddItemBinCode (bin, MUL);
			i += strlen (commands[2].com); // пропустить название команды
		} else if (hash == commands[3].hash) { //"div"
			AddItemBinCode (bin, DIV);
			i += strlen (commands[3].com); // пропустить название команды
		} else if (hash == commands[4].hash) { //"sqrt"
			AddItemBinCode (bin, SQRT);
			i += strlen (commands[4].com); // пропустить название команды
		} else if (hash == commands[5].hash) { //"sin"
			AddItemBinCode (bin, SIN);
			i += strlen (commands[5].com); // пропустить название команды
		} else if (hash == commands[6].hash) { //"cos"
			AddItemBinCode (bin, COS);
			i += strlen (commands[6].com); // пропустить название команды
		} else if (hash == commands[7].hash) { //"end"
			AddItemBinCode (bin, END);
			i += strlen (commands[7].com); // пропустить название команды
		} else if (hash == commands[8].hash) { //"push"
			i += strlen (commands[8].com); // пропустить название команды
			ProcessingComPush (buf_code, &i, bin);
		} else if (hash == commands[9].hash) { //"pop"
			i += strlen (commands[9].com); // пропустить название команды
			ProcessingComPop (buf_code, &i, bin);
		} else if (hash == commands[10].hash) { //"jmp"
			i += strlen (commands[10].com); // пропустить название команды
			Jump (buf_code, &i, marks, bin, JMP);
		} else if (hash == commands[11].hash) { //"ja"
			i += strlen (commands[11].com); // пропустить название команды
			Jump (buf_code, &i, marks, bin, JA);
		} else if (hash == commands[12].hash) { //"jae"
			i += strlen (commands[12].com); // пропустить название команды
			Jump (buf_code, &i, marks, bin, JAE);
		} else if (hash == commands[13].hash) { //"jb"
			i += strlen (commands[13].com); // пропустить название команды
			Jump (buf_code, &i, marks, bin, JB);
		} else if (hash == commands[14].hash) { //"jbe"
			i += strlen (commands[14].com); // пропустить название команды
			Jump (buf_code, &i, marks, bin, JBE);
		} else if (hash == commands[15].hash) { //"je"
			i += strlen (commands[15].com); // пропустить название команды
			Jump (buf_code, &i, marks, bin, JE);
		} else if (hash == commands[16].hash) { //"jne"
			i += strlen (commands[16].com); // пропустить название команды
			Jump (buf_code, &i, marks, bin, JNE);
		} else if (hash == commands[17].hash) { //"<-"
			i += strlen (commands[17].com); // пропуск символа метки "<-"
			JumpMark (buf_code, &i, marks, bin);
		} else if (hash == commands[18].hash) { //"call"
			i += strlen (commands[18].com); // пропуск символа метки "call"
			Jump (buf_code, &i, marks, bin, CALL);
		} else if (buf_code->buf[i] == ';') { //";"
			for (; buf_code->buf[i] != '\n' && buf_code->buf[i] != '\0' && i < buf_code->size; i++) {}
		} else if (hash == commands[20].hash) { //"function"
			i += strlen (commands[20].com); // пропустить название команды
			JumpMark (buf_code, &i, marks, bin);
		} else if (hash == commands[21].hash) { //"ret"
			AddItemBinCode (bin, RET);
			i += strlen (commands[21].com); // пропустить название команды
		} else {
			char *word = NULL;
			fprintf (stderr, "ERROR: invalid command: \"%s\"\n", 
					 word = WordFromString(&(buf_code->buf[i])));
			free (word); word =NULL;
			return;
		}
	}
}

void Jump (const buf_t *buf_code,
		   int *iter,
		   const marks_t *marks,
		   code_t *bin,
		   const int encod) {
	assert (buf_code != NULL);
	assert (iter != NULL);
	assert (marks != NULL);
	assert (bin != NULL);
	
	AddItemBinCode (bin, encod);
	
	for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
	
	AddItemBinCode (bin, FindMark(marks, HashWord (&(buf_code->buf[*iter]))));
	
	for (; !isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить название метки
}

int FindMark (const marks_t *marks,
			  const long int hash_name) {
	assert (marks != NULL);
	int pos = 0;
	
	for (int i = 0; i < marks->size; i++) {
		if (marks->mark[i].name == hash_name) {
			pos = marks->mark[i].pos;
			break;
		}
	}
	
	return pos;
}

void JumpMark (const buf_t *buf_code,
			   int *iter,
			   marks_t *marks,
			   const code_t *bin) {
	assert (buf_code != NULL);
	assert (iter != NULL);
	assert (marks != NULL);
	assert (bin != NULL);
	
	for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
	
	long int hash_name = HashWord (&(buf_code->buf[*iter]));
	
	/*
	if (!FindMark(marks, hash_name)) {
		char *word = NULL;
		fprintf (stderr, "ERROR: This marker (%s) is already in use.\n", 
				 word = WordFromString(&(buf_code->buf[*iter])));
		free (word); word = NULL;
	}
	*/
	
	for (; !isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++) {} // пропустить название метки
	
	mark_t mark = {hash_name, bin->size};
	
	AddMark (marks, &mark);
}

char *WordFromString (const char *string) {
	assert (string != NULL);
	
	char *word = calloc (MAX_SIZE_WORD, sizeof (char));
	
	int i = 0;
	for (; isspace (string[i]) && i < strlen(string); i++)  {} // пропустить пробельных символов
	
	int j = 0;
	for (; !isspace (string[i]) && i < strlen(string) && j < MAX_SIZE_WORD; i++, j++) {
		word[j] = string[i];
	} 
	word[++j] = '\0';
	
	return word;
}

void ProcessingComPush (const buf_t *buf_code,
						int *iter, 
						code_t *bin) {
	assert (buf_code != NULL);
	assert (iter!= NULL);
	assert (bin != NULL);
	
	for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
	
	if (buf_code->buf[*iter] == '[') {
		(*iter)++; // пропус '['
		for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
		if (isdigit (buf_code->buf[*iter])) {
			AddItemBinCode (bin, PUSH_STK);
			AddItemBinCode (bin, DigitFromString(&(buf_code->buf[*iter])));
			for (; isdigit (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
		} else if (!strncmp (&(buf_code->buf[*iter]), "ax", 2)) {
			AddItemBinCode (bin, PUSH_REG);
			AddItemBinCode (bin, AX);
			*iter += 2; // пропус 'ax'
		} else if (!strncmp (&(buf_code->buf[*iter]), "bx", 2)) {
			AddItemBinCode (bin, PUSH_REG);
			AddItemBinCode (bin, BX);
			*iter += 2; // пропус 'bx'
		} else if (!strncmp (&(buf_code->buf[*iter]), "cx", 2)) {
			AddItemBinCode (bin, PUSH_REG);
			AddItemBinCode (bin, CX);
			*iter += 2; // пропус 'cx'
		} else if (!strncmp (&(buf_code->buf[*iter]), "dx", 2)) {
			AddItemBinCode (bin, PUSH_REG);
			AddItemBinCode (bin, DX);
			*iter += 2; // пропус 'dx'
		} else {
			fprintf (stderr, "Error: Invalid argument to push!\n");
		}
		for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
		if (buf_code->buf[*iter] == ']') {
			(*iter)++; // пропус ']'
		} else {
			fprintf (stderr, "Error: Invalid argument to push!\n");
		}
	} else if (!strncmp (&(buf_code->buf[*iter]), "Ram[", 4)) {
		*iter += 4; // пропус "Ram["
		for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
		if (isdigit (buf_code->buf[*iter])) {
			AddItemBinCode (bin, PUSH_RAM);
			AddItemBinCode (bin, DigitFromString(&(buf_code->buf[*iter])));
			for (; isdigit (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
		} else {
			fprintf (stderr, "Error: Invalid argument to push!\n");
		}
		for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
		if (buf_code->buf[*iter] == ']') {
			(*iter)++; // пропус ']'
		} else {
			fprintf (stderr, "Error: Invalid argument to push!\n");
		}
	} else {
		fprintf (stderr, "Error: Invalid argument to push!\n");
	}
}

void ProcessingComPop (const buf_t *buf_code,
					   int *iter, 
					   code_t *bin) {
	assert (buf_code != NULL);
	assert (iter != NULL);
	assert (bin != NULL);

	for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
	if (buf_code->buf[*iter] == '[') {
		(*iter)++; // пропус '['
		
		for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
		
		if (!strncmp (&(buf_code->buf[*iter]), "ax", 2)) {
			AddItemBinCode (bin, POP_REG);
			AddItemBinCode (bin, AX);
			*iter += 2; // пропус 'ax'
		} else if (!strncmp (&(buf_code->buf[*iter]), "bx", 2)) {
			AddItemBinCode (bin, POP_REG);
			AddItemBinCode (bin, BX);
			*iter += 2; // пропус 'bx'
		} else if (!strncmp (&(buf_code->buf[*iter]), "cx", 2)) {
			AddItemBinCode (bin, POP_REG);
			AddItemBinCode (bin, CX);
			*iter += 2; // пропус 'cx'
		} else if (!strncmp (&(buf_code->buf[*iter]), "dx", 2)) {
			AddItemBinCode (bin, POP_REG);
			AddItemBinCode (bin, DX);
			*iter += 2; // пропус 'dx'
		} else {
			fprintf (stderr, "Error: Invalid argument to pop!\n");
		}
		for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
		if (buf_code->buf[*iter] == ']') {
			(*iter)++; // пропус ']'
		} else {
			fprintf (stderr, "Error: Invalid argument to pop!\n");
		}
	} else if (!strncmp (&(buf_code->buf[*iter]), "Ram[", 4)) {
		*iter += 4; // пропус "Ram["
		
		for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
		if (isdigit (buf_code->buf[*iter])) {
			AddItemBinCode (bin, POP_RAM);
			AddItemBinCode (bin, DigitFromString(&(buf_code->buf[*iter])));
			for (; isdigit (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
		} else {
			fprintf (stderr, "Error: Invalid argument to pop!\n");
		}
		for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
		if (buf_code->buf[*iter] == ']') {
			(*iter)++; // пропус ']'
		} else {
			fprintf (stderr, "Error: Invalid argument to pop!\n");
		}
	} else {
		AddItemBinCode (bin, POP_STK);
	}
	
}

int DigitFromString (const char *string) {
	assert (string != NULL);
	int num = 0;
	
	int i = 0;
	for (; isspace (string[i]) && i < strlen (string); i++) {}
	for (; isdigit(string[i]) && i < strlen (string); i++) {
		num = num * 10 + string[i] - '0';
	}
	return num;
}

int ResizeBinCode (code_t *bin, 
				   int new_size) {
	assert (bin != NULL);
	assert (new_size > 0);

	elem_bin *p = bin->data;
	bin->max_size = new_size;
	bin->data = realloc (bin->data, bin->max_size * sizeof(elem_bin));
	if (bin->data == NULL) {
		bin->data = p;
		return 1;
	}
   
    return 0;
}

int ResizeMarks (marks_t *code, 
				 int new_size) {
	assert (code != NULL);
	assert (new_size > 0);

	mark_t *p = code->mark;
	code->max_size = new_size;
	code->mark = realloc (code->mark, code->max_size * sizeof(mark_t));
	if (code->mark == NULL) {
		code->mark = p;
		return 1;
	}
 
    return 0;
}

int AddItemBinCode (code_t *bin, 
					elem_bin value) {
	assert (bin != NULL);
	int errcode = 0;
	
	if (bin->size >= bin->max_size)
		errcode = ResizeBinCode (bin, bin->max_size + START_SIZE_CODE_BIN);
		
	bin->data[bin->size++] = value;
	
	return errcode;
}

int AddMark (marks_t *marks, 
			 const mark_t *value) {
	assert (marks != NULL);
	int errcode = 0;
	
	if (marks->size >= marks->max_size)
		errcode = ResizeMarks (marks, marks->max_size + START_SIZE_MARK);
		
	marks->mark[marks->size++] = *value;
	
	return errcode;
}

void Postscript () {

	printf ("\n"
			"##################################################\n"
			"# Program finished working is over               #\n"
			"##################################################\n");
}
