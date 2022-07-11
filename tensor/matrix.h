#pragma once

#include <random>
#include <iostream>
//#include "vect.h"
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

template<typename T, std::size_t N> class matrix_base;  // pre-declare the template class itself
template<typename T, std::size_t N> void _transpose(matrix_base<T, N>& m);
template<typename T, std::size_t N> std::ostream& operator<< (std::ostream& o, const matrix_base<T,N>& m);

template<typename T, std::size_t N>
class matrix_base : private std::array<std::array<T, N>, N>
{
	void set_zero();
	//std::shared_ptr<const matrix_base<T, N>> itself_shared_ptr;
	//void create_shared_ptr();// { itself_shared_ptr = std::shared_ptr<const matrix_base<T, N>>(this); };
public:
	matrix_base(MATRIXINITTYPE IT = MATRIXINITTYPE::ZERO);
	//~matrix_base();

	//void create_basis(std::shared_ptr<const matrix_base<T, N>>& srd_ptr) const;// { return std::shared_ptr<const matrix_base<T, N>>(this); };

	inline std::array<std::array<T, N>, N> operator()() const { 
		return static_cast<std::array<std::array<T, N>, N>>(*this); };

	matrix_base transpose() const;
	matrix_base scal     (TRANSPOSE left, const matrix_base& rhs, TRANSPOSE right) const;
	matrix_base transform(TRANSPOSE left, const matrix_base& op , TRANSPOSE right) const;

	matrix_base& operator = (const T& vl);
	matrix_base& operator = (const std::array<std::array<T, N>, N>& m);
	//matrix_base& operator = (const matrix_base& m);

	virtual matrix_base  operator + (const matrix_base& m) const;
	virtual matrix_base  operator - (const matrix_base& m) const;
	virtual matrix_base  operator * (const matrix_base& m) const;
	//virtual matrix_base  operator * (const vect_base& m) const;
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


//template<typename T, std::size_t N>
//void matrix_base <T, N>::create_shared_ptr() { 
//	itself_shared_ptr = std::shared_ptr<const matrix_base<T, N>>(this); 
//};
//
//template<typename T, std::size_t N>
//void  matrix_base <T, N>::create_basis(std::shared_ptr<const matrix_base<T, N>> &shrd_ptr) const {
//	shrd_ptr = itself_shared_ptr;
//};

//template<typename T, std::size_t N>
//matrix_base<T, N>::~matrix_base()
//{
//	//itself_shared_ptr.reset();
//	return; 
//	try
//	{
//		if (itself_shared_ptr.use_count() == 1)
//			itself_shared_ptr.reset();
//		else
//			throw std::underflow_error("destroying basis when other objects own them");
//	}
//	catch (const std::runtime_error& err)
//	{
//		fputs(err.what(), stderr);
//		exit( 1);
//	}
//}

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
void matrix_base<T, N>::set_zero()
{
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			(*this)[row][col] = (T)0;
}


template<typename T, std::size_t N>
matrix_base<T, N> matrix_base<T, N>::transpose() const
{
	matrix_base<T, N> res(*this);
	//transpose(res);
	for (size_t row = 0; row < N; row++)
		for (size_t col = row + 1; col < N; col++)
			std::swap(res[row][col], res[col][row]);
	return res;
}



// this^?l . rhs^?r
template<typename T, std::size_t N>
matrix_base<T, N> matrix_base<T, N>::scal(TRANSPOSE left, const matrix_base& rhs, TRANSPOSE right) const
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
matrix_base<T, N> matrix_base<T, N>::transform(TRANSPOSE left, const matrix_base& op, TRANSPOSE right) const
{
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
matrix_base<T, N>::matrix_base(MATRIXINITTYPE IT)
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

	//create_shared_ptr();
}

template<typename T, std::size_t N>
matrix_base<T, N>& matrix_base<T, N>::operator = (const T& vl)
{
	T value = (T)vl;
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			(*this)[row][col] = value;
	return (*this);
};


template<typename T, std::size_t N>
matrix_base<T, N>& matrix_base<T, N>::operator = (const std::array<std::array<T, N>, N>& m) {
	(*static_cast<std::array<std::array<T, N>, N>*>(this)) = m;
	return *this;
};

//template<typename T, std::size_t N>
//matrix_base<T, N>& matrix_base<T, N>::operator = (const matrix_base<T, N>& m) {
//	(*static_cast<std::array<std::array<T, N>, N>*>(this)) = m;
//	itself_shared_ptr = m.itself_shared_ptr;
//	return *this;
//};

template<typename T, std::size_t N>
matrix_base<T, N> matrix_base<T, N>::operator + (const matrix_base<T, N>& rhs) const
{
	matrix_base<T, N> nhs;

	if (N < 1) return nhs;

	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			nhs[row][col] = (*this)[row][col] + rhs[row][col];
	return nhs;
}

template<typename T, std::size_t N>
matrix_base<T, N> matrix_base<T, N>::operator - (const matrix_base<T, N>& rhs) const
{
	matrix_base<T, N> nhs;

	if (N < 1) return nhs;

	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			nhs[row][col] = (*this)[row][col] - rhs[row][col];
	return nhs;
}


template<typename T, std::size_t N>
matrix_base<T, N>& matrix_base<T, N>::operator += (const matrix_base<T, N>& rhs)
{
	if (N < 1) return *this;

	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			(*this)[row][col] += rhs[row][col];
	return *this;
}


template<typename T, std::size_t N>
matrix_base<T, N>& matrix_base<T, N>::operator -= (const matrix_base<T, N>& rhs)
{
	if (N < 1) return *this;

	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			(*this)[row][col] -= rhs[row][col];
	return *this;
}

template<typename T, std::size_t N>
matrix_base<T, N> matrix_base<T, N>::operator * (const T& val) const
{
	matrix_base<T, N> nhs;
	if (N < 1) return nhs;
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			nhs[row][col] = (*this)[row][col] * val;
	return nhs;
}

template<typename T, std::size_t N>
matrix_base<T, N> matrix_base<T, N>::operator * (const matrix_base<T, N>& rhs) const
{
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
matrix_base<T, N>& matrix_base<T, N>::operator *=(const T& val) 
{
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			(*this)[row][col] *= val;
	return *this;
}

template<typename T, std::size_t N>
matrix_base<T, N>& matrix_base<T, N>::operator *= (const matrix_base<T, N>& rhs)
{
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