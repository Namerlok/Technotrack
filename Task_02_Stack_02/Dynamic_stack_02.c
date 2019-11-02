/*!
 * \mainpage
 * \author Загребельный Аркадий
 * \version 0.1
 * \date Октябрь 2019 года
 *
 * \brief Реализация динамического стека
 */
 
//----------------------------------------------------------------
 
#include "Stack.h"
#include "Dyn_st.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

//----------------------------------------------------------------

/*!
 * @brief Реализация динамического стека
 *
 * @return Статус выхода из программы
 */
int main () {

	Epigraph ();

	stack_t *stk = StackCreat ("stk");

	elem_t value = 10;
	StackPush (stk, &value);
	StackPush (stk, &value);
	StackPush (stk, &value);
	
	printf ("%i\n", StackPop(stk));
	printf ("%i\n", StackPop(stk));
	printf ("%i\n", StackPop(stk));
	printf ("%i\n", StackPop(stk));
	
//	StackDump (stk);

	StackDelete (stk);

	Postscript ();

	return 0;
}

//////////////////////////////////////////////////////////////////

void Epigraph () {

	printf ("##################################################\n"
			"# Dynamic stack                                  #\n"
			"# Arkadiy Zagrebelnyy, 2019                      #\n"
			"##################################################\n"
			"\n");
}

void Postscript () {

	printf ("\n"
			"##################################################\n"
			"# Program finished working is over               #\n"
			"##################################################\n");
}
