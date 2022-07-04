#pragma once

#include "type.h"
enum class MATRIXINITTYPE
{
	ZERO,
	INDENT

};

template<typename T, std::size_t N>
class Matrix
{
//protected:
	matr<T, N> comp;
public:
	Matrix(MATRIXINITTYPE IT);
	Matrix(T val = (T)0)            { *this = val; };
	Matrix(const matr<T, N>& _comp) { comp = _comp; }
	Matrix(const Matrix& _m)        { comp = _m.comp; };
	Matrix(Matrix&& _m) noexcept    { comp = std::move(_m.comp);};

	void     operator = (const double vl) {
		for (size_t row = 0; row < N; row++)
			for (size_t col = 0; col < N; col++)
				comp[row][col] = (T)vl;
	};

	//Matrix& operator = (const Matrix& m);
	const Matrix& operator = (const Matrix& m);
	Matrix  operator + (const Matrix& m) const;
	Matrix  operator - (const Matrix& m) const;
	Matrix  operator * (const Matrix& m) const;
	Matrix& operator +=(const Matrix& m);
	Matrix& operator -=(const Matrix& m);
	Matrix  operator *=(const Matrix& m);

};


template<typename T, std::size_t N>
std::ostream& operator<<(std::ostream& out, const Matrix<T, N>& a)
{
	out << a.comp;
	return out;
}


template<typename T, std::size_t N>
Matrix<T, N>::Matrix(MATRIXINITTYPE IT)
{
	switch (IT)
	{
	case MATRIXINITTYPE::ZERO  :
		*this = (T)0; 
		break;
	case MATRIXINITTYPE::INDENT:
		*this = (T)0;
		for (size_t row = 0; row < N; row++) 
			comp[row][row] = (T)1; 
		break;
	default:
		*this = (T)0;
		break;
	}
}

//template<typename T, std::size_t N>
//Matrix<T, N>& Matrix<T, N>::operator = (const Matrix<T, N>& m)
//{
//	this->comp = m.comp;
//	return *this;
//};

template<typename T, std::size_t N>
const Matrix<T, N>& Matrix<T, N>::operator = (const Matrix<T, N>& m)
{
	this->comp = m.comp;
	return *this;
};

template<typename T, std::size_t N>
Matrix<T, N> Matrix<T, N>::operator + (const Matrix<T, N>& m) const
{
	Matrix<T, N> newm((T)0);
	newm.comp = this->comp + m.comp;
	return newm;// std::move(newm);
}

template<typename T, std::size_t N>
Matrix<T, N>& Matrix<T, N>::operator += (const Matrix<T, N>& m)
{
	this->comp += m.comp;
	return *this;// std::move(newm);
}


template<typename T, std::size_t N>
Matrix<T, N> Matrix<T, N>::operator - (const Matrix<T, N>& m) const
{
	Matrix<T, N> newm((T)0);
	newm.comp = this->comp - m.comp;
	return newm;
}


template<typename T, std::size_t N>
Matrix<T, N>& Matrix<T, N>::operator -= (const Matrix<T, N>& m)
{
	this->comp -= m.comp;
	return *this;// std::move(newm);
}

template<typename T, std::size_t N>
Matrix<T, N> Matrix<T, N>::operator * (const Matrix<T, N>& m) const
{
	Matrix<T, N> newm((T)0);
	newm.comp = this->comp * m.comp;
	return newm;
}

template<typename T, std::size_t N>
Matrix<T, N> Matrix<T, N>::operator *= (const Matrix<T, N>& m)
{
	this->comp *= m.comp;
	return *this;// std::move(newm);
}