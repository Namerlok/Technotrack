#ifndef ENCODINGS_H
#define ENCODINGS_H


//! Кодировка команд
enum encodings_v01 {
/*	PUSH_STK	=  4812, ///< хеширование название "pushstack"
	PUSH_REG	=  8518, ///< хеширование название "pushregister"
	PUSH_RAM	=  2446, ///< хеширование название "pushRAM"
	POP_STK		=  3841, ///< хеширование название "popstack"
	POP_REG		=  7212, ///< хеширование название "popregister"
	POP_RAM		=  1785, ///< хеширование название "popRAM"
	JMP			=  1115, ///< хеширование название "jump"
	JA			=  4830, ///< хеширование название "jumpadove"
	JAE			= 11260, ///< хеширование название "jumpadoveequal"
	JB			=  4926, ///< хеширование название "jumpbelow"
	JBE			= 11356, ///< хеширование название "jumpbelowequal"
	JE			=  4865, ///< хеширование название "jumpequal"
	JNE			=  8501, ///< хеширование название "jumpnotequal"
	MARK		=  3896, ///< хеширование название "jumpmark"
	ADD			=  3884, ///< хеширование название "addition"
	SUB			=  7145, ///< хеширование название "subtraction"
	MUL			= 11304, ///< хеширование название "multiplication"
	DIV			=  3946, ///< хеширование название "division"
	SQRT		=  6101, ///< хеширование название "squareroot"
	SIN			=  1698, ///< хеширование название "sinus"
	COS			=  2242, ///< хеширование название "cosine"
	END			=  5901, ///< хеширование название "endprogram"
	CALL		=  8458, ///< хеширование название "callfunction"
	RET			=  2362, ///< хеширование название "return"*/
	AX			=  0, 
	BX			=  1, 
	CX			=  2, 
	DX			=  3  
};

#define DEF_CMD(name, code, argc, instr) \
	CMD_##name = (code),

//! Кодировка команд
enum encodings_v02 {
	#include "commands.h"
	CMD_MAX
};

#undef DEF_CMD

#endif
