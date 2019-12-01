DEF_CMD (end, 0, 0, {
	return TRUE;
})

DEF_CMD (push, 1, 1, {
	long int arg = cpu->code[cpu->iter++];
	StackPush (cpu->stk, &arg);
})

DEF_CMD (pushram, 2, 1, {
	long int arg = cpu->code[cpu->iter++];
		if (arg > MEMORY_SIZE) {
		cpu->err = CPU_GOING_BEYOND_RAM_ARR;
		return FALSE;
	}
	StackPush (cpu->stk, &(cpu->ram[arg]));
})

DEF_CMD (pushreg, 3, 1, {
	long int arg = cpu->code[cpu->iter++];
	if (arg > NUMBER_OF_REGISTERS) {
		cpu->err = CPU_GOING_BEYOND_REGISTER_ARR;
		return FALSE;
	}
	StackPush (cpu->stk, &(cpu->reg[arg]));
})

DEF_CMD (pop, 4, 0, {
	StackPop (cpu->stk);
})

DEF_CMD (popram, 5, 1, {
	long int arg = cpu->code[cpu->iter++];
	if (arg > MEMORY_SIZE) {
		cpu->err = CPU_GOING_BEYOND_RAM_ARR;
		return FALSE;
	}
	cpu->ram[arg] = StackPop (cpu->stk);
})

DEF_CMD (popreg, 6, 1, {
	long int arg = cpu->code[cpu->iter++];
	if (arg > NUMBER_OF_REGISTERS) {
		cpu->err = CPU_GOING_BEYOND_REGISTER_ARR;
		return FALSE;
	}
	cpu->reg[arg] = StackPop (cpu->stk);
})

DEF_CMD (add, 7, 0, {
	long int arg1 = StackPop (cpu->stk);
	long int arg2 = StackPop (cpu->stk);
	arg1 = arg2 + arg1;
	StackPush (cpu->stk, &(arg1));
})

DEF_CMD (sub, 8, 0, {
	long int arg1 = StackPop (cpu->stk);
	long int arg2 = StackPop (cpu->stk);
	arg1 = arg2 - arg1;
	StackPush (cpu->stk, &(arg1));
})

DEF_CMD (mul, 9, 0, {
	long int arg1 = StackPop (cpu->stk);
	long int arg2 = StackPop (cpu->stk);
	arg1 = arg2 * arg1;
	StackPush (cpu->stk, &(arg1));
})

DEF_CMD (div, 10, 0, {
	long int arg1 = StackPop (cpu->stk);
	long int arg2 = StackPop (cpu->stk);
	if (arg1 == 0) {
		cpu->err = CPU_DIVISION_BY_ZERO;
		return FALSE;
	}
	arg1 = arg2 / arg1;
	StackPush (cpu->stk, &(arg1));
})

DEF_CMD (sqrt, 11, 0, {
	long int arg = StackPop (cpu->stk);
	if (arg < 0) {
		cpu->err = CPU_DIVISION_BY_ZERO;
		return FALSE;
	}
	arg = sqrt (arg);
	StackPush (cpu->stk, &(arg));
})

DEF_CMD (sin, 12, 0, {
	long int arg = StackPop (cpu->stk);
	arg = sin (arg);
	StackPush (cpu->stk, &(arg));
})

DEF_CMD (cos, 13, 0, {
	long int arg = StackPop (cpu->stk);
	arg = cos (arg);
	StackPush (cpu->stk, &(arg));
})

DEF_CMD (jmp, 14, 1, {
	long int pos = cpu->code[cpu->iter++];
	if (pos >= cpu->code_size) {
		cpu->err = CPU_GOING_BEYOND_CODE_ARR;
		return FALSE;
	}
	cpu->iter = pos;
})

DEF_CMD (ja, 15, 1, {
	long int arg1 = StackPop (cpu->stk);
	long int arg2 = StackPop (cpu->stk);
	long int pos = cpu->code[cpu->iter++];
	if (pos >= cpu->code_size) {
		cpu->err = CPU_GOING_BEYOND_CODE_ARR;
		return FALSE;
	}
	if (arg2 > arg1)
		cpu->iter = pos;
})

DEF_CMD (jae, 16, 1, {
	long int arg1 = StackPop (cpu->stk);
	long int arg2 = StackPop (cpu->stk);
	long int pos = cpu->code[cpu->iter++];
	if (pos >= cpu->code_size) {
		cpu->err = CPU_GOING_BEYOND_CODE_ARR;
		return FALSE;
	}
	if (arg2 >= arg1)
		cpu->iter = pos;
})

DEF_CMD (jb, 17, 1, {
	long int arg1 = StackPop (cpu->stk);
	long int arg2 = StackPop (cpu->stk);
	long int pos = cpu->code[cpu->iter++];
	if (pos >= cpu->code_size) {
		cpu->err = CPU_GOING_BEYOND_CODE_ARR;
		return FALSE;
	}
	if (arg2 < arg1)
		cpu->iter = pos;
})

DEF_CMD (jbe, 18, 1, {
	long int arg1 = StackPop (cpu->stk);
	long int arg2 = StackPop (cpu->stk);
	long int pos = cpu->code[cpu->iter++];
	if (pos >= cpu->code_size) {
		cpu->err = CPU_GOING_BEYOND_CODE_ARR;
		return FALSE;
	}
	if (arg2 <= arg1)
		cpu->iter = pos;
})

DEF_CMD (je, 19, 1, {
	long int arg1 = StackPop (cpu->stk);
	long int arg2 = StackPop (cpu->stk);
	long int pos = cpu->code[cpu->iter++];
	if (pos >= cpu->code_size) {
		cpu->err = CPU_GOING_BEYOND_CODE_ARR;
		return FALSE;
	}
	if (arg2 == arg1)
		cpu->iter = pos;
})

DEF_CMD (jne, 20, 1, {
	long int arg1 = StackPop (cpu->stk);
	long int arg2 = StackPop (cpu->stk);
	long int pos = cpu->code[cpu->iter++];
	if (pos >= cpu->code_size) {
		cpu->err = CPU_GOING_BEYOND_CODE_ARR;
		return FALSE;
	}
	if (arg2 != arg1)
		cpu->iter = pos;
})

DEF_CMD (call, 21, 1, {
	long int pos = StackPop (cpu->stk);
	if (pos >= cpu->code_size) {
		cpu->err = CPU_GOING_BEYOND_CODE_ARR;
		return FALSE;
	}
	StackPush (cpu->fun_stk, (long int *) &(cpu->iter));
	cpu->iter = pos;
})

DEF_CMD (ret, 22, 0, {
	cpu->iter = StackPop (cpu->fun_stk);
})

DEF_CMD (out, 23, 0, {
	printf ("%li\n", StackPop (cpu->stk));
})

DEF_CMD (in, 24, 0, {
	long int arg = 0;
	printf ("write nember: ");
	scanf ("%li", &arg);
	StackPush (cpu->stk, &(arg));
})

DEF_CMD (dump, 25, 0, {
	CPUDump (cpu);
})
