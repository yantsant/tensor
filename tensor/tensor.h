#pragma once
#include "matrix.h"
#include "quat.h"
#include "vect.h"

template<typename T, std::size_t N> class Tensor;

template<typename T, size_t N>
class Tensor : public basis_handler<T, N >, public matrix_base<T, N>
{
public:
	~Tensor() { };

	virtual T       convolution(const Tensor<T, N>& rhs) const;
	virtual Tensor& operator = (const Tensor& t);
	virtual Tensor& operator = (const T& value);
	virtual Tensor  operator + (const Tensor& t) const;
	virtual Tensor  operator - (const Tensor& t) const;
	virtual Tensor  operator * (const Tensor& t) const;

	friend  Vector<T, N>  operator * (const Tensor& t, const Vector<T, N>& v)	{
		return Vector<T, N>(v.comp_at_basis(t.get_basis()) * static_cast<matrix_base<T, N>>(t), t.get_basis());	}

	friend  Vector<T, N>  operator * (const Vector<T, N>& v, const Tensor& t)	{
		return Vector<T, N>(static_cast<matrix_base<T, N>>(t) * v.comp_at_basis(t.get_basis()), t.get_basis());	}

	virtual Tensor& operator +=(const Tensor& t);
	virtual Tensor& operator -=(const Tensor& t);
	virtual Tensor& operator *=(const Tensor& t);

	matrix_base<T, N> comp_at_basis( const matrix_base<T, N>& target_basis) const;

	Tensor(MATRIXINITTYPE IT,             const matrix_base<T, N>& basis) : matrix_base<T, N>(IT  ), basis_handler<T, N>(basis) {};
	Tensor(const matrix_base<T, N>& comp, const matrix_base<T, N>& basis) : matrix_base<T, N>(comp), basis_handler<T, N>(basis) {};
};

template<typename T, std::size_t N>
T   Tensor<T, N>::convolution(const Tensor<T, N>& rhs) const {
	return static_cast<matrix_base<T, N>>(*this).convolution(rhs.comp_at_basis(this->get_basis()));
}

template<typename T, size_t N>
Tensor<T, N>& Tensor<T, N>::operator = (const T& value) {
	return static_cast<Tensor<T, N>&>(matrix_base<T, N>::operator=(value));
};

template<typename T, size_t N>
Tensor<T, N>& Tensor<T, N>::operator = (const Tensor<T, N>& t)  {
	return static_cast<Tensor<T, N>&>(matrix_base<T, N>::operator=(t.comp_at_basis(this->get_basis())));
};

template<typename T, size_t N>
Tensor<T, N>  Tensor<T, N>::operator + (const Tensor<T, N>& t) const {
	return Tensor<T, N>(matrix_base<T, N>::operator+(t.comp_at_basis(this->get_basis())), this->get_basis());
}
template<typename T, size_t N>
Tensor<T, N>  Tensor<T, N>::operator - (const Tensor<T, N>& t) const {
	return Tensor<T, N>(matrix_base<T, N>::operator-(t.comp_at_basis(this->get_basis())), this->get_basis());
}

template<typename T, size_t N>
Tensor<T, N>  Tensor<T, N>::operator * (const Tensor<T, N>& t) const  {
	return Tensor<T, N>(matrix_base<T, N>::operator*(t.comp_at_basis(this->get_basis())), this->get_basis());
}

template<typename T, size_t N>
Tensor<T, N>& Tensor<T, N>::operator +=(const Tensor<T, N>& t)  {
	matrix_base<T, N>::operator+=(t.comp_at_basis(this->get_basis()));
	return *this;
}

template<typename T, size_t N>
Tensor<T, N>& Tensor<T, N>::operator -=(const Tensor<T, N>& t)  {
	matrix_base<T, N>::operator-=(t.comp_at_basis(this->get_basis()));
	return *this;
}

template<typename T, size_t N>
Tensor<T, N>& Tensor<T, N>::operator *=(const Tensor<T, N>& t){
	matrix_base<T, N>::operator*=(t.comp_at_basis(this->get_basis()));
	return *this;
}

/* return components of this in the target basis m*/
template<typename T, size_t N>
matrix_base<T, N> Tensor<T, N>::comp_at_basis( const matrix_base<T, N>& m) const {
	matrix_base<T, N> res;
	if (this->same_basis(m)) {
		res = static_cast<matrix_base<T, N>>(*this);
	}
	else {
		const matrix_base<T, N>& op   = this->get_basis() * m.transpose();
		const matrix_base<T, N>& comp = static_cast<const matrix_base<T, N>&> (*this);
		res = this->transform(TRANSPOSE::TRUE, op, TRANSPOSE::FALSE); // op^t * (*this) * op
	}
	return res;
}