/*!
 * \mainpage
 * \author Загребельный Аркадий
 * \version 0.1
 * \date Сентябрь 2019 года
 *
 * \brief Программа для решения квадратного уравнения
 */


#include "Header.h"

/*!
 * @brief Программа для решения квадратного уравнения
 *
 * @return Статус выхода из программы
 */
int main() {
	Epigraph();

	Unit_test_SolveSquare();

	double a = NAN, b = NAN, c = NAN;
	double x1 = NAN, x2 = NAN;

	Reading_coef (&a, &b, &c);

	int nRoots = SolveSquare (a, b, c, &x1, &x2);

	switch (nRoots) {
	case NO_ROOTS		: printf ("No roots\n");
		break;
	case ONE_ROOTS		: printf ("x = %lg\n", x1);
		break;
	case TWO_ROOTS		: printf ("x1 = %lg, x2 = %lg\n", x1, x2);
		break;
	case SS_INF_ROOTS	: printf ("Any number\n");
		break;
	default			: printf ("ERROR_001: function main: nRoots = %d\n", nRoots);
		return 1;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////


/*!
 * @brief Считывание коэффициентов квадратного уравнения ax^2 + bx + c = 0
 *
 * @param a - коэффициент уравнения при x^2
 * @param b - коэффициент уравнения при x
 * @param c - свободный член
 *
 * @return void
 */
void Reading_coef( double* a, double* b, double* c ) {
	assert (a != NULL);
	assert (b != NULL);
	assert (c != NULL);

	assert (a != b);
	assert (a != c);
	assert (c != b);

	int i = 0;
	char buf[255];

	while (1) {
		printf ("Enter a, b, c: ");
		i = scanf ("%lf %lf %lf", a, b, c);
//		printf ("#\t%lf %lf %lf\n", *a, *b, *c);
		if ( (i == 3) && (isfinite (*a) != 0) && (isfinite (*b) != 0) && (isfinite (*c) != 0) )
			break;
		fgets (buf, 255, stdin);
		printf ("You entered incorrect data!\n");
	}
}

/*!
 * @brief Приветственные слова
 *
 * @return void
 */
void Epigraph() {
	printf ("# Square equation solver\n"
		"# Arkadiy, 2019\n"
		"\n");
}

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
int SolveSquare (double a, double b, double c, double* l_x1, double* l_x2) {
	assert (isfinite (a));
	assert (isfinite (b));
	assert (isfinite (c));

	assert (l_x1 != NULL);
	assert (l_x2 != NULL);
	assert (l_x1 != l_x2);

	// ax*x + bx + c = 0
	if (-NUMBER_EROOR <= a && NUMBER_EROOR >= a) {		// bx + c = 0
		return SolveLine (b, c, l_x1);
	} else if (-NUMBER_EROOR <= c && NUMBER_EROOR >= c) {	// ax*x + bx = 0
		*l_x1 = 0;
		return 1 + SolveLine (a, b, l_x2);
	} else {		// ax*x + bx + c = 0
		double d = b * b - 4.0 * a * c;

		if (d < 0) {
			return NO_ROOTS;
		} else if (-NUMBER_EROOR <= d && NUMBER_EROOR >= d) {
			*l_x1 = *l_x2 = -b / (2 * a);
			return ONE_ROOTS;
		} else { 	// d > 0
			double sqrt_d = sqrt (d);
			*l_x1 = (-b - sqrt_d) / (2 * a);
			*l_x2 = (-b + sqrt_d) / (2 * a);
			return TWO_ROOTS;
		}

	}
}

/*!
 * @brief Решение линейного уравнения bx + c = 0
 *
 * @param b - коэффициент уравнения при x
 * @param c - свободный член
 * @param l_x1 - указатель для записи решения уравнения
 *
 * @return Количество корней уравнения
 */
int SolveLine (double b, double c, double* l_x1) {
	assert (isfinite (b));
	assert (isfinite (c));

	assert (l_x1 != NULL);

	// bx + c = 0
	if (-NUMBER_EROOR <= b && NUMBER_EROOR >= b &&
		-NUMBER_EROOR <= c && NUMBER_EROOR >= c) {		// 0x + 0 = 0
		return SS_INF_ROOTS;
	} else if (-NUMBER_EROOR <= b && NUMBER_EROOR >= b &&
			   -NUMBER_EROOR >= c && NUMBER_EROOR <= c) {	// 0x + c = 0
		return NO_ROOTS;
	} else { 			// bx + c = 0
		*l_x1 = -c / b;
		return ONE_ROOTS;
	}
}

/*!
 * @brief Unit test функции решения квадратного уравнения
 *
 * @return void
 */
void Unit_test_SolveSquare() {

	Epigraph_Unit_test();

	double a = 0.0, b = 0.0, c = 0.0;	// Коэффициенты уравнения
	double l_x1 = 0.0, l_x2 = 0.0;		// Корни уравнения полученные при помощи SolveSquare()
	double x1 = 0.0, x2 = 0.0;			// Корни уравнения из файла
	int i = 0, n = 0;
	FILE *fp = fopen ("Unit_test_SolveSquare.txt", "r");

	fscanf (fp, "%i", &n);
	for (i = 1; i <= n; i++) {
		a = b = c = x1 = x2 = l_x1 = l_x2 = NAN;
		fscanf (fp, "%lf %lf %lf %lf %lf", &a, &b, &c, &x1, &x2);
		SolveSquare (a, b, c, &l_x1, &l_x2);
		if ( (((x1 - NUMBER_EROOR <= l_x1 && x1 + NUMBER_EROOR >= l_x1) || (isnan(l_x1) && isnan(x1))) &&
		      ((x2 - NUMBER_EROOR <= l_x2 && x2 + NUMBER_EROOR >= l_x2) || (isnan(l_x2) && isnan(x2)))) ||
		     (((x1 - NUMBER_EROOR <= l_x2 && x1 + NUMBER_EROOR >= l_x2) || (isnan(l_x2) && isnan(x1))) &&
		      ((x2 - NUMBER_EROOR <= l_x1 && x2 + NUMBER_EROOR >= l_x1) || (isnan(l_x1) && isnan(x2)))) ) {
			printf ("#U\tNo problem on test number %2.i\n", i);
		} else {
			printf ("#U\tProblem on test number %2.i\n"
					"#U\t\tTest: a = %lf, b = %lf, c = %lf, x = %lf, x = %lf\n"
					"#U\t\tResulting Solutions: x = %lf, x = %lf\n",
			       	i, a, b, c, x1, x2, l_x1, l_x2);
		}
	}
	printf ("\n");
	fclose (fp);
}

/*!
 * @brief Вводные слова для Unit test
 *
 * @return void
 */
void Epigraph_Unit_test() {
	printf ("#U\tUnit test of the function \"SolveSquare()\"\n"
			"#U\tCoefficient values are read from \"Unit_test_SolveSquare.txt\"\n"
			"#U\tAt the beginning of the file should contain the number of tests,\n"
			"#U\tand after the tests themselves: a b c x1 x2\n"
			"#U\t\n");
}
