#include <cmath>
#include <string>
#include <sstream>
#include <iostream>

#include "utest.h"

using namespace std;

stringstream COUT;

// Переопределяем с помощью препроцессора cout из iostream
// на наш stringstream COUT
#define cout COUT


void func(double a, double b, double c, double d, double e, double f)
{
if ((a == 0) && (b == 0) && (c == 0) && (d == 0) && (e == 0) && (f == 0))
	{
		cout << '5';
	}
	else if ((a*d - c * b != 0) && ((e*d - b * f != 0) || (a*f - c * e != 0)))
	{
		double y = (a * f - c * e) / (a * d - c * b);
		double x = (d * e - b * f) / (d * a - b * c);
		cout << "2 " << x << ' ' << y;
	}
	else if (((a*d - c * b == 0) && ((e*d - b * f != 0) || (a*f - c * e != 0))) || 
		(a == 0 && c == 0 && e / b != f / d) || 
		(b == 0 && d == 0 && e / a != f / c) || 
		(a == 0 && b == 0 && c == 0 && d == 0 && (e / f > 0)))
	{
		if (((a == 0 && b == 0 && e == 0 && d != 0 && c == 0) || 
			(c == 0 && d == 0 && f == 0 && b != 0 && a == 0)))
		{
			double y;
			if (b == 0)
				y = f / d;
			else if (d == 0)
				y = e / b;
			else if (e == 0 || f == 0)
				y = 0;
			cout << '4' << ' ' << y;
		}
		else if (((a == 0 && b == 0 && e == 0 && c != 0 && d == 0) || 
			(c == 0 && d == 0 && f == 0 && a != 0 && b == 0)))
		{
			double x;
			if (a == 0)
				x = f / c;
			else if (c == 0)
				x = e / a;
			else if (e == 0 || f == 0)
				x = 0;
			cout << '3' << ' ' << x;
		}
		else
			cout << '0';
	}
	else if (a == 0 && c == 0)
	{
		double y;
		if (e == 0)
			y = f / d;
		else if (f == 0)
			y = e / b;
		else
			y = e / b;
		cout << '4' << ' ' << y;
	}
	else if (b == 0 && d == 0)
	{
		double x;
		if (e == 0)
			x = f / c;
		else if (f == 0)
			x = e / a;
		else
			x = e / a;
		cout << '3' << ' ' << x;
	}
	else if (b == 0 && e == 0)
	{
		double k, n;
		k = -c / d;
		n = f / d;
		cout << '1' << ' ' << k << ' ' << n;
	}
	else if (d == 0 && f == 0)
	{
		double k, n;
		k = -a / b;
		n = e / b;
		cout << '1' << ' ' << k << ' ' << n;
	}
	else if (a == 0 && e == 0)
	{
		double k, n;
		k = -d / c;
		n = f / c;
		cout << '1' << ' ' << k << ' ' << n;
	}
	else if (c == 0 && f == 0)
	{
		double k, n;
		k = -b / a;
		n = e / a;
		cout << '1' << ' ' << k << ' ' << n;
	}
	else if ((a / b == c / d))
	{
		double k, n;
		k = -c / d;
		n = f / d;
		cout << '1' << ' ' << k << ' ' << n;
	}
	else
	{
		cout << "Are you kidding me?";
	}

}

// Возвращаем прежний cout
#undef cout

#define ASSERT_COUT(VALUE) \
	cout << "EXECUTION VALUE: " << COUT.str() << endl; \
	cout << "EXPECTED VALUE: " << VALUE << endl; \
	ASSERT_TRUE(COUT.str() == VALUE); \
	COUT.str("");

UTEST_MAIN();

UTEST(func, all_zero){
	func(0,0,0,0,0,0);
	ASSERT_COUT("5");
}


UTEST(func, single_solution){
	func(2,3,5,-2,12,11);
	ASSERT_COUT("2 3 2");
}


UTEST(func, infinite_solutions_linear){
	func(3,-2,6,-4,5,10);
	ASSERT_COUT("1 1.5 -2.5");
}


UTEST(func, no_roots){
	func(1,2,1,2,1,2);
	ASSERT_COUT("0");
}


UTEST(func, any_x){
	func(0,1,0,1,1,1);
	ASSERT_COUT("4 1");
}


UTEST(func, any_y){
	func(1,0,1,0,1,1);
	ASSERT_COUT("3 1");
}
