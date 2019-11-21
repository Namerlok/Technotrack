/*!
 * \mainpage
 * \author Загребельный Аркадий
 * \version 0.1
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
	buf_file.size = ReadFileToBuf (&(buf_file.buf), FILE_IN, READING_BINARY);
	
	code_t bin = {calloc (START_SIZE_CODE_BIN, sizeof (int)), 0};
	
	if (Assembler (&buf_file, &bin))
		fprintf (stderr, "Сode assembly error!!!");
		
	WriteFile (&bin, FILE_OUT, WRITING_BINARY);
	
	free (buf_file.buf);
	free (bin.code);
	
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
		(commands[i]).hash = HashWord (commands[i].com);
}

long int HashWord (const char *string) {
	assert (string != NULL);
	
	long int hash = 0;
	for (int i = 0; !isspace(string[i]); i++)
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
							{";", 0}
						   };
	HashCommands (commands, sizeof (commands) / sizeof (command_t));

	marks_t marks = {calloc (START_SIZE_MARK, sizeof(mark_t)), 0};
	
	bin->size = 0;
	
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
	
	for (int i = 0; i < buf_code->size; i++) {
		for (; isspace (buf_code->buf[i]) && i <= buf_code->size; i++) {}
		switch (HashWord (&(buf_code->buf[i]))) {
			case commands[0].hash: //"add"
				bin->code[bin->size++] = ADD;
				for (; isalpha (buf_code->buf[i]) && i < buf_code->size; i++) {} // пропустить название команды
				break;
			case commands[1].hash: //"sub"
				bin->code[bin->size++] = SUB;
				for (; isalpha (buf_code->buf[i]) && i < buf_code->size; i++) {} // пропустить название команды
				break;
			case commands[2].hash: //"mul"
				bin->code[bin->size++] = MUL;
				for (; isalpha (buf_code->buf[i]) && i < buf_code->size; i++) {} // пропустить название команды
				break;
			case commands[3].hash: //"div"
				bin->code[bin->size++] = DIV;
				for (; isalpha (buf_code->buf[i]) && i < buf_code->size; i++) {} // пропустить название команды
				break;
			case commands[4].hash: //"sqrt"
				bin->code[bin->size++] = SQRT;
				for (; isalpha (buf_code->buf[i]) && i < buf_code->size; i++) {} // пропустить название команды
				break;
			case commands[5].hash: //"sin"
				bin->code[bin->size++] = SIN;
				for (; isalpha (buf_code->buf[i]) && i < buf_code->size; i++) {} // пропустить название команды
				break;
			case commands[6].hash: //"cos"
				bin->code[bin->size++] = COS;
				for (; isalpha (buf_code->buf[i]) && i < buf_code->size; i++) {} // пропустить название команды
				break;
			case commands[7].hash: //"end"
				bin->code[bin->size++] = END;
				for (; isalpha (buf_code->buf[i]) && i < buf_code->size; i++) {} // пропустить название команды
				break;
			case commands[8].hash: //"push"
				ProcessingComPush (buf_code, &i, bin);
				break;
			case commands[9].hash: //"pop"
				ProcessingComPop (buf_code, &i, bin);
				break;
			case commands[10].hash: //"jmp"
				Jump (buf_code, &i, marks, bin, JMP);
				break;
			case commands[11].hash: //"ja"
				Jump (buf_code, &i, marks, bin, JA);
				break;
			case commands[12].hash: //"jae"
				Jump (buf_code, &i, marks, bin, JAE);
				break;
			case commands[13].hash: //"jb"
				Jump (buf_code, &i, marks, bin, JB);
				break;
			case commands[14].hash: //"jbe"
				Jump (buf_code, &i, marks, bin, JBE);
				break;
			case commands[15].hash: //"je"
				Jump (buf_code, &i, marks, bin, JE);
				break;
			case commands[16].hash: //"jne"
				Jump (buf_code, &i, marks, bin, JNE);
				break;
			case commands[17].hash: //"<-"
				JumpMark (buf_code, &i, marks, bin);
				break;
			case commands[18].hash: //"call"
////////////////////////////////////////////////////////
				break;
			case commands[19].hash: //";"
				break;
			default: 
				char *word = NULL;
				fprintf (stderr, "ERROR: invalid command: \"%s\"", word = WordFromString(buf_code->(buf + *iter)););
				free (word); word =NULL;
				break;
		}
	}
}

void Jump (const buf_t *buf_code,
		   int *iter,
		   const marks_t *marks,
		   code_t *bin,
		   const int encod) {
	assert (com != NULL);
	assert (iter != NULL);
	assert (marks != NULL);
	assert (bin != NULL);

	for (; isalpha (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++) {} // пропустить название команды
	
	bin->code[bin->size++] = encod;
	
	for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
	
	bin->code[bin->size++] = FindMark(marks, HashWord (buf_code->(buf + *iter)));
	
	for (; isalpha (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить название метки
}

int FindMark (const marks_t *marks,
			  const long int hash_name) {
	assert (marks != NULL);
	int pos = 0;
	
	for (int i = 0; i < marks->size; i++) {
		if (marks->(mark + i)->name == hash_name) {
			pos = marks->(mark + i)->pos;
			break;
		}
	}
	
	return pos;
}

void JumpMark (const buf_t *buf_code,
			   int *iter,
			   marks_t *marks,
			   const code_t *bin) {
	assert (com != NULL);
	assert (iter != NULL);
	assert (marks != NULL);
	assert (bin != NULL);
	
	*iter += 2; // пропуск символа метки "<-"
	for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
	
	hash_name = HashWord (buf_code->(buf + *iter));
	
	if (!FindMark(marks, hash_name)) {
		char *word = NULL;
		fprintf (stderr, "ERROR: This marker (%s) is already in use.", 
				 word = WordFromString(buf_code->(buf + *iter)));
		free (word); word =NULL;
	}
	
	for (; isalpha (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++) {} // пропустить название метки
	
	marks->(mark + marks->size)->name = hash_name;
	marks->(mark + marks->size)->pos = bin->size;
	marks->size++;
}

char *WordFromString (const char *string) {
	assert (string != NULL);
	
	char *word = calloc (MAX_SIZE_WORD, sizeof (char));
	int i = 0;
	
	for (; isspace (string[i]) && i < strlen(string); i++)  {} // пропустить пробельных символов
	
	for (int j = 0; !isspace (string[i]) && i < strlen(string) && j < MAX_SIZE_WORD; i++, j++) {
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
	
	*iter += 4; // пропустить название команды "push"
	
	for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
	
	if (buf_code->buf[*iter] == '[') {
		(*iter)++; // пропус '['
		for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
		if (isdigit (buf_code->buf[*iter])) {
			bin->code[bin->size++] = PUSH_STK;
			bin->code[bin->size++] = DigitFromString(buf_code->buf[*iter]);
			for (; isdigit (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
		} else if (!strncmp (buf_code->(buf + *iter), "ax", 2)) {
			bin->code[bin->size++] = PUSH_REG;
			bin->code[bin->size++] = AX;
			*iter += 2; // пропус 'ax'
		} else if (!strncmp (buf_code->(buf + *iter), "bx", 2)) {
			bin->code[bin->size++] = PUSH_REG;
			bin->code[bin->size++] = BX;
			*iter += 2; // пропус 'bx'
		} else if (!strncmp (buf_code->(buf + *iter), "cx", 2)) {
			bin->code[bin->size++] = PUSH_REG;
			bin->code[bin->size++] = CX;
			*iter += 2; // пропус 'cx'
		} else if (!strncmp (buf_code->(buf + *iter), "dx", 2)) {
			bin->code[bin->size++] = PUSH_REG;
			bin->code[bin->size++] = DX;
			*iter += 2; // пропус 'dx'
		} else {
			fprintf (stderr, "Error: Invalid argument to push!");
		}
		for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
		if (buf_code->buf[*iter] == ']') {
			(*iter)++; // пропус ']'
		} else {
			fprintf (stderr, "Error: Invalid argument to push!");
		}
	} else if (!strncmp (buf_code->(buf + *iter), "Ram[", 4)) {
		*iter += 4; // пропус "Ram["
		for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
		if (isdigit (buf_code->buf[*iter])) {
			bin->code[bin->size++] = PUSH_RAM;
			bin->code[bin->size++] = DigitFromString(buf_code->buf[*iter]);
			for (; isdigit (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
		} else {
			fprintf (stderr, "Error: Invalid argument to push!");
		}
		for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
		if (buf_code->buf[*iter] == ']') {
			(*iter)++; // пропус ']'
		} else {
			fprintf (stderr, "Error: Invalid argument to push!");
		}
	} else {
		fprintf (stderr, "Error: Invalid argument to push!");
	}
}

void ProcessingComPop (const buf_t *buf_code,
					   int *iter, 
					   code_t *bin) {
	assert (buf_code != NULL);
	assert (iter != NULL);
	assert (bin != NULL);
	
	*iter += 4; // пропустить название команды "pop"
	
	if (buf_code->buf[*iter] == '[') {
		(*iter)++; // пропус '['
		
		for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
		
		if (!strncmp (buf_code->(buf + *iter), "ax", 2)) {
			bin->code[bin->size++] = POP_REG;
			bin->code[bin->size++] = AX;
			*iter += 2; // пропус 'ax'
		} else if (!strncmp (buf_code->(buf + *iter), "bx", 2)) {
			bin->code[bin->size++] = POP_REG;
			bin->code[bin->size++] = BX;
			*iter += 2; // пропус 'bx'
		} else if (!strncmp (buf_code->(buf + *iter), "cx", 2)) {
			bin->code[bin->size++] = POP_REG;
			bin->code[bin->size++] = CX;
			*iter += 2; // пропус 'cx'
		} else if (!strncmp (buf_code->(buf + *iter), "dx", 2)) {
			bin->code[bin->size++] = POP_REG;
			bin->code[bin->size++] = DX;
			*iter += 2; // пропус 'dx'
		} else {
			fprintf (stderr, "Error: Invalid argument to pop!");
		}
		for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
		if (buf_code->buf[*iter] == ']') {
			(*iter)++; // пропус ']'
		} else {
			fprintf (stderr, "Error: Invalid argument to pop!");
		}
	} else if (!strncmp (buf_code->(buf + *iter), "Ram[", 4)) {
		iter += 4; // пропус "Ram["
		
		for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
		if (isdigit (buf_code->buf[*iter])) {
			bin->code[bin->size++] = POP_RAM;
			bin->code[bin->size++] = DigitFromString(buf_code->buf[*iter]);
			for (; isdigit (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
		} else {
			fprintf (stderr, "Error: Invalid argument to pop!");
		}
		for (; isspace (buf_code->buf[*iter]) && *iter < buf_code->size; (*iter)++)  {} // пропустить пробельных символов
		if (buf_code->buf[*iter] == ']') {
			(*iter)++; // пропус ']'
		} else {
			fprintf (stderr, "Error: Invalid argument to pop!");
		}
	} else {
		bin->code[bin->size++] = POP_STK;
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

void Postscript () {

	printf ("\n"
			"##################################################\n"
			"# Program finished working is over               #\n"
			"##################################################\n");
}
