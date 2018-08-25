/*
Fast complex math

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef COMPLEX_HH
#define COMPLEX_HH

template <typename T>
class Complex
{
	T re, im;
public:
	typedef T value_type;
	Complex() : re(0), im(0) {}
	Complex(T r) : re(r), im(0) {}
	Complex(T r, T i) : re(r), im(i) {}
	inline T real() const { return re; }
	inline T imag() const { return im; }
	inline void real(T r) { re = r; }
	inline void imag(T i) { im = i; }
	inline Complex<T> operator = (T a)
	{
		real() = a;
		imag() = 0;
		return *this;
	}
	inline Complex<T> operator += (Complex<T> a)
	{
		return *this = a + *this;
	}
	inline Complex<T> operator -= (Complex<T> a)
	{
		return *this = *this - a;
	}
	inline Complex<T> operator *= (Complex<T> a)
	{
		return *this = a * *this;
	}
	inline Complex<T> operator *= (T a)
	{
		return *this = a * *this;
	}
	inline Complex<T> operator /= (T a)
	{
		return *this = *this / a;
	}
	inline Complex<T> operator /= (Complex<T> a)
	{
		return *this = *this / a;
	}
};

template <typename T>
static inline Complex<T> operator + (Complex<T> a, Complex<T> b)
{
	return Complex<T>(a.real() + b.real(), a.imag() + b.imag());
}

template <typename T>
static inline Complex<T> operator + (Complex<T> a)
{
	return a;
}

template <typename T>
static inline Complex<T> operator - (Complex<T> a, Complex<T> b)
{
	return Complex<T>(a.real() - b.real(), a.imag() - b.imag());
}

template <typename T>
static inline Complex<T> operator - (Complex<T> a)
{
	return Complex<T>(-a.real(), -a.imag());
}

template <typename T>
static inline Complex<T> operator * (T a, Complex<T> b)
{
	return Complex<T>(a * b.real(), a * b.imag());
}

template <typename T>
static inline Complex<T> operator / (Complex<T> a, T b)
{
	return Complex<T>(a.real() / b, a.imag() / b);
}

template <typename T>
static inline Complex<T> operator * (Complex<T> a, Complex<T> b)
{
	return Complex<T>(a.real() * b.real() - a.imag() * b.imag(), a.real() * b.imag() + a.imag() * b.real());
}

template <typename T>
static inline Complex<T> operator / (Complex<T> a, Complex<T> b)
{
	return Complex<T>((a.real() * b.real() + a.imag() * b.imag()) / (b.real() * b.real() + b.imag() * b.imag()),
			(a.imag() * b.real() - a.real() * b.imag()) / (b.real() * b.real() + b.imag() * b.imag()));
}

template <typename T>
static inline Complex<T> exp(Complex<T> a)
{
	return Complex<T>(exp(a.real()) * cos(a.imag()), exp(a.real()) * sin(a.imag()));
}

template <typename T>
static inline T abs(Complex<T> a)
{
	return hypot(a.real(), a.imag());
}

template <typename T>
static inline T arg(Complex<T> a)
{
	return atan2(a.imag(), a.real());
}

template <typename T>
static inline T norm(Complex<T> a)
{
	return a.real() * a.real() + a.imag() * a.imag();
}

#endif
