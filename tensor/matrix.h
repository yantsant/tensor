#pragma once

#include "typematr.h"
#include "vect.h"
#include <random>
const char DIM = 3;
enum class MATRIXINITTYPE
{
	ZERO,
	INDENT
};

enum class TRANSPOSE
{
	TRUE,
	FALSE
};
//const std::mt19937 random_engine;
//const std::random_device rd;
//
//template<typename T>
//const std::uniform_real_distribution<T> unidistr = std::uniform_real_distribution<T>((T)0, (T)1);
//
//template<typename T>
//T get_uniform_value() { 
//	return unidistr<T>(random_engine); 
//};

template<typename T, std::size_t N> class matrix;  // pre-declare the template class itself
template<typename T, std::size_t N> void _transpose(matrix<T, N>& m);
template<typename T, std::size_t N> std::ostream& operator<< (std::ostream& o, const matrix<T,N>& m);

template<typename T, std::size_t N>
class matrix : private std::array<std::array<T, N>, N>
{
	void set_zero();
public:
	matrix(MATRIXINITTYPE IT = MATRIXINITTYPE::ZERO);

	inline std::array<std::array<T, N>, N> operator()() const { 
		return static_cast<std::array<std::array<T, N>, N>>(*this); };

	matrix transpose() const;
	friend void transpose(matrix& m)
	{
		for (size_t row = 0; row < N; row++)
			for (size_t col = row + 1; col < N; col++)
				std::swap(m[row][col], m[col][row]);
	};
	matrix scal     (TRANSPOSE left, const matrix& rhs, TRANSPOSE right) const;
	matrix transform(TRANSPOSE left, const matrix& op , TRANSPOSE right) const;

	matrix& operator = (const T& vl);
	matrix& operator = (const std::array<std::array<T, N>, N>& m);

	virtual matrix  operator + (const matrix& m) const;
	virtual matrix  operator - (const matrix& m) const;
	virtual matrix  operator * (const matrix& m) const;
	virtual matrix  operator * (const T& val) const;
	virtual matrix& operator +=(const matrix& m);
	virtual matrix& operator -=(const matrix& m);
	virtual matrix& operator *=(const matrix& m);
	virtual matrix& operator *=(const T& val);


	friend std::ostream& operator<< <>(std::ostream& out, const matrix& a);
};


template<typename T, std::size_t N>
std::ostream& operator<<(std::ostream& out, const matrix<T, N>& a) {
	for (auto row : a)
	{
		for (auto col : row)
			out << col << " ";
		out << "\n";
	}
	out << "\n";
	return out;
};

//template<typename T, std::size_t N>
//void  transpose(matrix<T, N>& m) {
//	for (size_t row = 0; row < N; row++)
//		for (size_t col = row + 1; col < N; col++)
//			std::swap(m[row][col], m[col][row]);
//};

template<typename T, std::size_t N>
void matrix<T, N>::set_zero()
{
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			(*this)[row][col] = (T)0;
}


template<typename T, std::size_t N>
matrix<T, N> matrix<T, N>::transpose() const
{
	matrix<T, N> res(*this);
	//transpose(res);
	for (size_t row = 0; row < N; row++)
		for (size_t col = row + 1; col < N; col++)
			std::swap(res[row][col], res[col][row]);
	return res;
}



// this^?l . rhs^?r
template<typename T, std::size_t N>
matrix<T, N> matrix<T, N>::scal(TRANSPOSE left, const matrix& rhs, TRANSPOSE right) const
{
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
matrix<T, N> matrix<T, N>::transform(TRANSPOSE left, const matrix& op, TRANSPOSE right) const
{
	matrix<T, N> opt = op.transpose();
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
matrix<T, N>::matrix(MATRIXINITTYPE IT)
{
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
	default:
		set_zero();
		break;
	}
}

template<typename T, std::size_t N>
matrix<T, N>& matrix<T, N>::operator = (const T& vl)
{
	T value = (T)vl;
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			(*this)[row][col] = value;
	return (*this);
};


template<typename T, std::size_t N>
matrix<T, N>& matrix<T, N>::operator = (const std::array<std::array<T, N>, N>& m) {
	(*static_cast<std::array<std::array<T, N>, N>*>(this)) = m;
	return *this;
};

template<typename T, std::size_t N>
matrix<T, N> matrix<T, N>::operator + (const matrix<T, N>& rhs) const
{
	matrix<T, N> nhs;

	if (N < 1) return nhs;

	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			nhs[row][col] = (*this)[row][col] + rhs[row][col];
	return nhs;
}

template<typename T, std::size_t N>
matrix<T, N> matrix<T, N>::operator - (const matrix<T, N>& rhs) const
{
	matrix<T, N> nhs;

	if (N < 1) return nhs;

	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			nhs[row][col] = (*this)[row][col] - rhs[row][col];
	return nhs;
}


template<typename T, std::size_t N>
matrix<T, N>& matrix<T, N>::operator += (const matrix<T, N>& rhs)
{
	if (N < 1) return *this;

	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			(*this)[row][col] += rhs[row][col];
	return *this;
}


template<typename T, std::size_t N>
matrix<T, N>& matrix<T, N>::operator -= (const matrix<T, N>& rhs)
{
	if (N < 1) return *this;

	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			(*this)[row][col] -= rhs[row][col];
	return *this;
}

template<typename T, std::size_t N>
matrix<T, N> matrix<T, N>::operator * (const T& val) const
{
	matrix<T, N> nhs;
	if (N < 1) return nhs;
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			nhs[row][col] = (*this)[row][col] * val;
	return nhs;
}

template<typename T, std::size_t N>
matrix<T, N> matrix<T, N>::operator * (const matrix<T, N>& rhs) const
{
	matrix<T, N> nhs;
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
matrix<T, N>& matrix<T, N>::operator *=(const T& val) 
{
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			(*this)[row][col] *= val;
	return *this;
}

template<typename T, std::size_t N>
matrix<T, N>& matrix<T, N>::operator *= (const matrix<T, N>& rhs)
{
	if (N < 1) return (*this);

	matrix<T, N> nhs;
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