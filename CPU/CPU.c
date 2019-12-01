/*!
 * \mainpage
 * \author Загребельный Аркадий
 * \version 0.2
 * \date Ноябрь 2019 года
 *
 * \brief soft для CPU
 */
 
#include "encodings.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h> 
#include <stdio.h>
#include "Stack.h"
#include <math.h>
#include "CPU.h"

int main () {
	
	Epigraph ();
	
	CPU_t cpu = {};
	
	
	CPUCreate (&cpu);
	CPUDump (&cpu);
	cpu.code_size = ReadFileToBuf (&(cpu.code), FILE_IN, READING_BINARY);
	CPUImplementation (&cpu);
	CPUDelete (&cpu);
	
	Postscript ();
	
	return 0;
}

void Epigraph () {

	printf ("##################################################\n"
			"# soft of CPU                                    #\n"
			"# Arkadiy Zagrebelnyy, 2019                      #\n"
			"##################################################\n"
			"\n");
}

void CPUCreate (CPU_t *cpu) {
	assert(cpu);
	
	cpu->err = 0;
	
	cpu->stk = StackCreate (NAME_CPU_STACK);
	if (StackOK (cpu->stk) == FALSE)
		cpu->err = CPU_STACK_ERROR;
		
	cpu->fun_stk = StackCreate (NAME_CPU_FUNCTION_STACK);
	if (StackOK (cpu->fun_stk) == FALSE)
		cpu->err = CPU_STACK_ERROR;
		
	cpu->reg = calloc (NUMBER_OF_REGISTERS, sizeof(long int));
	cpu->ram = calloc (MEMORY_SIZE, sizeof(long int));
	if (cpu->reg == NULL || cpu->ram == NULL)
		cpu->err = CPU_MEMORY_ALLOCATION_ERROR;
	
	cpu->iter = 0;
	cpu->code = NULL;
	cpu->code_size = 0;
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
	
	size_f /= sizeof(**buf);

	if (readcount != size_f) {
		fprintf (stderr, "The number of bytes read differs from the specified!!!\n"
						 "must: %li\tget: %li", size_f, readcount);
	}

	return readcount;
}

int CPUImplementation (CPU_t *cpu) {
	assert (cpu);

	CPUOK (cpu);
	
	cpu->iter = 0;
	while (cpu->iter < cpu->code_size) {
		switch (cpu->code[cpu->iter]) {
		#define DEF_CMD(name, encode, argc, code) \
		case encode: \
			cpu->iter++; \
			code \
			break;
		#include "commands.h"
		#undef DEF_CMD
		
		default:
			cpu->err = CPU_UNKNOWN_COMMAND;
			return FALSE;
		}
	}
	
	return TRUE;
}

int CPUOK (CPU_t *cpu) {
	assert (cpu);
	
	if (cpu->err != 0)
		return FALSE;
		
	if (StackOK (cpu->stk) == FALSE || StackOK (cpu->fun_stk) == FALSE) {
		cpu->err = CPU_STACK_ERROR;
		return FALSE;
	}
	
	if (cpu->reg == NULL || cpu->ram == NULL || cpu->code == NULL) {
		cpu->err = CPU_POINTER_EQUALS_NULL;
		return FALSE;
	}
	
	if (cpu->iter > cpu->code_size) {
		cpu->err = CPU_GOING_BEYOND_CODE_ARR;
		return FALSE;
	}
	
	return TRUE;
}

void CPUDump (CPU_t *cpu) {	
	assert (cpu);
	
	FILE* out = fopen (CPU_DUMP_FILE_NAME, WRITING);
	if (out == NULL)
		perror ("Открытие файла для cpu damp\n");
	
	fprintf (out, "CPU [%p] {\n", cpu);
	fprintf (out, "Stack: [%p]\n", cpu->stk);
	StackDump (cpu->stk);
	fprintf (out, "Function stack: [%p]\n", cpu->fun_stk);
	StackDump (cpu->fun_stk);
	fprintf (out, "Register[%i]: [%p] {\n", NUMBER_OF_REGISTERS, cpu->reg);
	for (int i = 0; i < NUMBER_OF_REGISTERS; i++)
		fprintf (out, "\t[%i] = %li\n", i, cpu->reg[i]);
	fprintf (out, "}\n");
	fprintf (out, "RAM[%i]: [%p] {\n", MEMORY_SIZE, cpu->ram);
	for (int i = 0; i < MEMORY_SIZE; i++)
		fprintf (out, "\t[%i] = %li\n", i, cpu->ram[i]);
	fprintf (out, "}\n");
	fprintf (out, "iter = %i\n", cpu->iter);
	fprintf (out, "code_size = %li\n", cpu->code_size);
	fprintf (out, "err = %i (%s)\n", cpu->err, 
			 (cpu->err) ? (ErrorDefinitionCPU (cpu->err)) : ("ok"));
	fprintf (out, "}\n");
}

char *ErrorDefinitionCPU (int err) {
	switch (err) {
		case CPU_UNKNOWN_COMMAND:
			return "незнакомая команда";
		case CPU_STACK_ERROR: 
			return "ошибки стека";
		case CPU_MEMORY_ALLOCATION_ERROR:	
			return "ошибки выделения памяти";
		case CPU_POINTER_EQUALS_NULL: 
			return "указатель равен NULL";
		case CPU_GOING_BEYOND_CODE_ARR: 
			return "выход за пределы массива кода";
		case CPU_GOING_BEYOND_RAM_ARR: 
			return "выход за пределы массива ram";
		case CPU_GOING_BEYOND_REGISTER_ARR: 
			return "выход за пределы массива reg";
		case CPU_DIVISION_BY_ZERO: 
			return "деление на нуль";
		case CPU_NEGATIVE_VAL_UNDER_ROOT: 
			return "отрицательное значение под квадратным корнем";
		default: 
			return "неопределенный код ошибки";
	}
}

int CPUDelete (CPU_t *cpu) {
	assert (cpu);
	
	StackDelete (cpu->stk);
	StackDelete (cpu->fun_stk);
	
	free (cpu->reg);	cpu->reg = NULL;
	free (cpu->ram);	cpu->ram = NULL;
	free (cpu->code);	cpu->code = NULL;
	
	return 0;
}

void Postscript () {

	printf ("\n"
			"##################################################\n"
			"# Program finished working is over               #\n"
			"##################################################\n");
}
