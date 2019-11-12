#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

//----------------------------------------------------------------

/*!
 * @brief Считывание коэффициентов квадратного уравнения ax^2 + bx + c = 0
 *
 * @param a - коэффициент уравнения при x^2
 * @param b - коэффициент уравнения при x
 * @param c - свободный член
 *
 * @return void
 */
void Reading_coef (double* a, double* b, double* c);

/*!
 * @brief Приветственные слова
 *
 * @return void
 */
void Epigraph();

/*!
 * @brief Решение квадратного уравнения ax^2 + bx + c = 0
 *
 * @param a - коэффициент уравнения при x^2
 * @param b - коэффициент уравнения при x
 * @param c - свободный член
 * @param l_x1 - указатель для записи первого корня уравнения
 * @param l_x2 - указатель для записи второго корня уравнения
 *
 * @return Количество корней уравнения
 */
int  SolveSquare (double a, double b, double c, double* l_x1, double* l_x2);

/*!
 * @brief Решение линейного уравнения bx + c = 0
 *
 * @param b - коэффициент уравнения при x
 * @param c - свободный член
 * @param l_x1 - указатель для записи решения уравнения
 *
 * @return Количество корней уравнения
 */
int  SolveLine (double b, double c, double* l_x1);

/*!
 * @brief Unit test функции решения квадратного уравнения
 *
 * @return void
 */
void Unit_test_SolveSquare();

/*!
 * @brief Вводные слова для Unit test
 *
 * @return void
 */
void Epigraph_Unit_test();

//----------------------------------------------------------------

//# define SS_INF_ROOTS -1
//! Погрешность сравнения double чисел
#define NUMBER_EROOR 0.0001

/*!
 * @brief Количество решений уравнения(для сравнивания в switch)
 */
enum COMPONENTS {
	SS_INF_ROOTS 	= -1,	///< = -1, бесконечное число решений
	NO_ROOTS	=  0,	///< =  0, нет действительных корней
	ONE_ROOTS	=  1,	///< =  1, один корень
	TWO_ROOTS	=  2	///< =  2, два корня
};
