#ifndef ENCODINGS_H
#define ENCODINGS_H

//! Кодировка команд
enum encodings {
	PUSH_STK	=  4436, ///< хеширование название "push stack"
	PUSH_REG	=  8065, ///< хеширование название "push register"
	PUSH_RAM	=  2214, ///< хеширование название "push RAM"
	POP_STK		=  3540, ///< хеширование название "pop stack"
	POP_REG		=  6841, ///< хеширование название "pop register"
	POP_RAM		=  1598, ///< хеширование название "pop RAM"
	JMP			=   667, ///< хеширование название "jump"
	JA			=  4507, ///< хеширование название "jump adove"
	JAE			= 11643, ///< хеширование название "jump adove equal"
	JB			=  4433, ///< хеширование название "jump below"
	JBE			= 11749, ///< хеширование название "jump below equal"
	JE			=  4481, ///< хеширование название "jump equal"
	JNE			=  8846, ///< хеширование название "jump not equal"
	MARK		=  3520, ///< хеширование название "jump mark"
	ADD			=  3004, ///< хеширование название "addition"
	SUB			=  5935, ///< хеширование название "subtraction"
	MUL			=  9764, ///< хеширование название "multiplication"
	DIV			=  3066, ///< хеширование название "division"
	SQRT		=  5501, ///< хеширование название "square root"
	SIN			=  1123, ///< хеширование название "sinus"
	COS			=  1636, ///< хеширование название "cosine"
	END			=  5590, ///< хеширование название "end program"
	CALL		=  8058, ///< хеширование название "call function"
	AX			=  5193, ///< хеширование название "register ax"
	BX			=  5203, ///< хеширование название "register bx"
	CX			=  5213, ///< хеширование название "register cx"
	DX			=  5223  ///< хеширование название "register dx"
};

#endif
