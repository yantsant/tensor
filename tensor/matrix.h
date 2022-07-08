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

template<typename T, std::size_t N>
class Matrix
{
//protected:
	matr<T, N> comp;
public:
	Matrix(MATRIXINITTYPE IT);
	Matrix(T val = (T)0)            {*this = val; };
	Matrix(const matr<T, N>& _comp) { comp = _comp; }
	Matrix(const Matrix& _m)        { comp = _m.comp; };
	Matrix(Matrix&& _m) noexcept    { comp = std::move(_m.comp);};


	//void   get_components(Matrix& m) const;
	//Matrix get_components( ) const;

	Matrix transpose();
	static Matrix transpose(Matrix& m);
	Matrix scal_transp(const Matrix& rhs);
	void   scal_transp(const Matrix& rhs, Matrix& res);
	Matrix transp_scal(const Matrix& rhs);
	void   transp_scal(const Matrix& rhs, Matrix& res);
	Matrix left_transform (const Matrix& op)  const;
	void   left_transform (const Matrix& op, Matrix& res)  const;
	Matrix right_transform(const Matrix& op) const;
	void   right_transform(const Matrix& op, Matrix& res) const;

	void     operator = (const double vl) {
		for (size_t row = 0; row < N; row++)
			for (size_t col = 0; col < N; col++)
				comp[row][col] = (T)vl;
	};

	//Matrix& operator = (const Matrix& m);
	void copy(const Matrix& m);
	const   Matrix& ref() const { return *this; };
	virtual Matrix& operator = (const Matrix& m);
	virtual Matrix  operator + (const Matrix& m) const;
	virtual Matrix  operator - (const Matrix& m) const;
	virtual Matrix  operator * (const Matrix& m) const;
	virtual Matrix& operator +=(const Matrix& m);
	virtual Matrix& operator -=(const Matrix& m);
	virtual Matrix  operator *=(const Matrix& m);

};


template<typename T, std::size_t N>
void Matrix<T, N>::copy(const Matrix& m)
{
	comp = m.comp;
}

template<typename T, std::size_t N>
Matrix<T, N> Matrix<T, N>::transpose()
{
	Matrix<T, N> res(*this);
	matrix_operator::t(res);
	return res;
}

// transpose this = this^T
template<typename T, std::size_t N>
static Matrix<T, N> Matrix<T, N>::transpose(Matrix& m)
{
	matrix_operator::t(m);
}

// this . rhs^T
template<typename T, std::size_t N>
Matrix<T, N> Matrix<T, N>::scal_transp(const Matrix& rhs)
{
	Matrix<T, N> res(*this);
	matrix_operator::dott(comp, rhs.comp, res);
	return res;
}
template<typename T, std::size_t N>
void Matrix<T, N>::scal_transp(const Matrix& rhs, Matrix& res)
{
	matrix_operator::dott(comp, rhs.comp, res);
}

// this^T . rhs
template<typename T, std::size_t N>
Matrix<T, N> Matrix<T, N>::transp_scal(const Matrix& rhs)
{
	Matrix<T, N> res(*this);
	matrix_operator::tdot(comp, rhs.comp, res);
	return res;
}
template<typename T, std::size_t N>
void Matrix<T, N>::transp_scal(const Matrix& rhs, Matrix& res)
{
	matrix_operator::tdot(comp, rhs.comp, res);
}


// op . this . op^T
template<typename T, std::size_t N>
Matrix<T, N> Matrix<T, N>::left_transform(const Matrix& op) const
{
	Matrix<T, N> res;
	res.comp = matrix_operator::dotdott(op.comp, comp);
	return res;
}
template<typename T, std::size_t N>
void Matrix<T, N>::left_transform(const Matrix& op, Matrix& res) const
{
	matrix_operator::dotdott(op.comp, comp, res.comp);
}

// op^T . this . op
template<typename T, std::size_t N>
Matrix<T, N> Matrix<T, N>::right_transform(const Matrix& op) const
{
	Matrix<T, N> res;
	res.comp = matrix_operator::tdotdot(op.comp, comp);
	return res;
}
template<typename T, std::size_t N>
void Matrix<T, N>::right_transform(const Matrix& op, Matrix& res) const
{
	matrix_operator::tdotdot(op.comp, comp, res.comp);
}

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
Matrix<T, N>& Matrix<T, N>::operator = (const Matrix<T, N>& m)
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