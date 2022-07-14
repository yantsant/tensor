#pragma once
#include <random>
#include <iostream>
//#include "vectbase.h"
enum class MATRIXINITTYPE
{
	ZERO,
	INDENT,
	RANDOM,
	//ORTOGONAL_RANDOM
};

enum class TRANSPOSE
{
	TRUE,
	FALSE
};

extern std::random_device rd;  // Will be used to obtain a seed for the random number engine
extern std::mt19937 gen; // Standard mersenne_twister_engine seeded with rd()
extern std::uniform_real_distribution<double> unidistr;

extern const size_t DIM;

template<typename T, std::size_t N> class matrix_base;
template<typename T, std::size_t N> std::ostream& operator<< (std::ostream& o, const matrix_base<T,N>& m);
template<typename T, std::size_t N> matrix_base<T, N> generate_rand_ort();

namespace matrix_generator
{
	template<typename T, std::size_t N>
	matrix_base<T, N> generate_rand_ort();

	template<typename T, std::size_t N>
	matrix_base<T, N> generate_rand() {
		std::array<std::array<T, N>, N> a;
		for (size_t row = 0; row < N; row++)
			for (size_t col = 0; col < N; col++)
				a[row][col] = static_cast<T>(unidistr(gen));
		return matrix_base<T, N>(a);
	};
};


template<typename T, std::size_t N>
class matrix_base : private std::array<std::array<T, N>, N>
{
	void set_zero();
	T precision;// = (T)1 / (T)1000000;
	void set_precision();
public:
	matrix_base(MATRIXINITTYPE IT = MATRIXINITTYPE::ZERO);
	matrix_base(const matrix_base<T, N>& m);
	matrix_base(const std::array<std::array<T, N>, N>& a);
	inline const std::array<std::array<T, N>, N> operator()() const { return static_cast<const std::array<std::array<T, N>, N>>(*this); };

	friend matrix_base<T, 3> generate_ort_rand();
	bool        check_ort() const;
	matrix_base transpose() const;
	matrix_base scal(TRANSPOSE left, const matrix_base& rhs, TRANSPOSE right) const;
	matrix_base transform(TRANSPOSE left, const matrix_base& op, TRANSPOSE right) const;

	matrix_base& operator = (const T& vl);
	//matrix_base& operator = (const matrix_base& m);
	virtual matrix_base  operator + (const matrix_base& m) const;
	virtual matrix_base  operator - (const matrix_base& m) const;
	virtual matrix_base  operator * (const matrix_base& m) const;
	virtual matrix_base  operator * (const T& val) const;
	virtual matrix_base& operator +=(const matrix_base& m);
	virtual matrix_base& operator -=(const matrix_base& m);
	virtual matrix_base& operator *=(const matrix_base& m);
	virtual matrix_base& operator *=(const T& val);

	friend std::ostream& operator<< <>(std::ostream& out, const matrix_base& a);
	friend void transpose(matrix_base& m)
	{
		for (size_t row = 0; row < N; row++)
			for (size_t col = row + 1; col < N; col++)
				std::swap(m[row][col], m[col][row]);
	};
};


template<typename T, std::size_t N>
void matrix_base<T, N>::set_precision()
{
	std::string type(typeid(T).name());
	if (type.find("double") != std::string::npos)
		precision = (T)1 / (T)1e10;
	else if (type.find("float") != std::string::npos)
		precision = (T)1 / (T)1000000;
	else
		precision = (T)1 / (T)100000;
}

template<typename T, std::size_t N>
matrix_base<T, N>::matrix_base(const matrix_base<T, N>& m) {
	(*this) = m;
	set_precision();
}

template<typename T, std::size_t N>
matrix_base<T, N>::matrix_base(const std::array<std::array<T, N>, N>& a) {
	static_cast<std::array<std::array<T, N>, N>&>(*this) = a;
	set_precision();
}

template<typename T, size_t N>
bool matrix_base<T, N >::check_ort() const {
	matrix_base<T, N> m = *this * this->transpose();// -matrix_base<T, N>(MATRIXINITTYPE::INDENT);
	T diag = 0;
	T nondiag = 0;
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			(row == col) ? diag += m[row][col] : nondiag += m[row][col];
	if (abs(diag - (T)N) / (T)N + abs(nondiag) <= precision)
			return true;
	return false;
}

template<typename T, std::size_t N>
std::ostream& operator<<(std::ostream& out, const matrix_base<T, N>& a) {
	for (auto row : a)
	{
		for (auto col : row)
			out << col << " ";
		out << "\n";
	}
	out << "\n";
	return out;
};

template<typename T, std::size_t N>
void matrix_base<T, N>::set_zero() {
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			(*this)[row][col] = (T)0;
}


template<typename T, std::size_t N>
matrix_base<T, N> matrix_base<T, N>::transpose() const {
	matrix_base<T, N> res(*this);
	//transpose(res);
	for (size_t row = 0; row < N; row++)
		for (size_t col = row + 1; col < N; col++)
			std::swap(res[row][col], res[col][row]);
	return res;
}



// this^?l . rhs^?r
template<typename T, std::size_t N>
matrix_base<T, N> matrix_base<T, N>::scal(TRANSPOSE left, const matrix_base& rhs, TRANSPOSE right) const {
	if (left == right)
	{
		if (left == TRANSPOSE::FALSE)
			return (*this) * rhs;
		else
			return transpose() * rhs;
	}
	else
	{
		if (left == TRANSPOSE::FALSE)
			return (*this) * rhs.transpose();
		else
			return transpose() * rhs;
	}
}

//  op^?l . this . op^?r
template<typename T, std::size_t N>
matrix_base<T, N> matrix_base<T, N>::transform(TRANSPOSE left, const matrix_base& op, TRANSPOSE right) const {
	matrix_base<T, N> opt = op.transpose();
	if (left == right)
	{
		if (left == TRANSPOSE::FALSE)
			return op * (*this) * op;
		else
			return opt * (*this) * opt;
	}
	else
	{
		if (left == TRANSPOSE::FALSE)
			return opt * (*this) * op;
		else
			return op * (*this) * opt;
	}
}

template<typename T, std::size_t N>
matrix_base<T, N>::matrix_base(MATRIXINITTYPE IT) {
	switch (IT)
	{
	case MATRIXINITTYPE::ZERO  :
		set_zero();
		break;
	case MATRIXINITTYPE::INDENT:
		set_zero();
		for (size_t row = 0; row < N; row++) 
			(*this)[row][row] = (T)1; 
		break;
	case MATRIXINITTYPE::RANDOM:
		for (size_t row = 0; row < N; row++)
			for (size_t col = 0; col < N; col++)
				(*this)[row][col] =  unidistr(gen);
		break;
	//case MATRIXINITTYPE::ORTOGONAL_RANDOM:
	//	if (N != 3) throw std::length_error("MATRIXINITTYPE::ORTOGONAL_RANDOM is implemented only for 3-dimensional matrix.");
	//	//tatic_cast<matrix_base<T, 3>>(*this) = generate_ort_rand<T,3>();
	////	quat;
	//	break;
	default:
		set_zero();
		break;
	}

	//auto x = generate_ort_rand<double, 3>();
	//auto y = (*this);
	//double z;
	set_precision();
}

//template<typename T, std::size_t N>
//matrix_base<T, N> generate_rand() {
//	for (size_t row = 0; row < N; row++)
//		for (size_t col = 0; col < N; col++)
//			(*this)[row][col] = unidistr<T>(gen);
//}

template<typename T, std::size_t N>
matrix_base<T, N>& matrix_base<T, N>::operator = (const T& vl) {
	T value = (T)vl;
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			(*this)[row][col] = value;
	return (*this);
};

template<typename T, std::size_t N>
matrix_base<T, N> matrix_base<T, N>::operator + (const matrix_base<T, N>& rhs) const {
	matrix_base<T, N> nhs;

	if (N < 1) return nhs;

	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			nhs[row][col] = (*this)[row][col] + rhs[row][col];
	return nhs;
}

template<typename T, std::size_t N>
matrix_base<T, N> matrix_base<T, N>::operator - (const matrix_base<T, N>& rhs) const {
	matrix_base<T, N> nhs;

	if (N < 1) return nhs;

	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			nhs[row][col] = (*this)[row][col] - rhs[row][col];
	return nhs;
}


template<typename T, std::size_t N>
matrix_base<T, N>& matrix_base<T, N>::operator += (const matrix_base<T, N>& rhs) {
	if (N < 1) return *this;

	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			(*this)[row][col] += rhs[row][col];
	return *this;
}


template<typename T, std::size_t N>
matrix_base<T, N>& matrix_base<T, N>::operator -= (const matrix_base<T, N>& rhs) {
	if (N < 1) return *this;

	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			(*this)[row][col] -= rhs[row][col];
	return *this;
}

template<typename T, std::size_t N>
matrix_base<T, N> matrix_base<T, N>::operator * (const T& val) const {
	matrix_base<T, N> nhs;
	if (N < 1) return nhs;
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			nhs[row][col] = (*this)[row][col] * val;
	return nhs;
}

template<typename T, std::size_t N>
matrix_base<T, N> matrix_base<T, N>::operator * (const matrix_base<T, N>& rhs) const {
	matrix_base<T, N> nhs;
	if (N < 1) return nhs;
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
		{
			nhs[row][col] = (T)0;
			for (size_t i = 0; i < N; i++)
				nhs[row][col] += (*this)[row][i] * rhs[i][col];
		}
	return nhs;
}

template<typename T, std::size_t N>
matrix_base<T, N>& matrix_base<T, N>::operator *=(const T& val)  {
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			(*this)[row][col] *= val;
	return *this;
}

template<typename T, std::size_t N>
matrix_base<T, N>& matrix_base<T, N>::operator *= (const matrix_base<T, N>& rhs) {
	if (N < 1) return (*this);

	matrix_base<T, N> nhs;
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
		{
			nhs[row][col] = (T)0;
			for (size_t i = 0; i < N; i++)
				nhs[row][col] += (*this)[row][i] * rhs[i][col];
		}
	(*this) = nhs;
	return (*this);
}