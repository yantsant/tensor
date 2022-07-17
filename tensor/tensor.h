#pragma once
#include "matrbase.h"
#include "quat.h"
#include "vect.h"
#include "basis.h"

template<typename T, std::size_t N> class Tensor;
template<typename T, std::size_t N> class Vector;
//template<typename T, std::size_t N> Tensor<T,N> outer_product (const Vector<T, N>& lhs, const Vector<T, N>& rhs);

template<typename T, size_t N>
class Tensor : private basis_handler<T, N, matrix_base<T, N>, matrix_base<T, N>>,
			   private matrix_base<T, N>
{
	typedef  matrix_base  <T, N>          _matrix;
	typedef  basis_handler<T, N, _matrix, _matrix> _handler;
	typedef  shared_handler_object<const  _matrix> _shared;
public:
	virtual void      change_basis     (const _shared& m) override;
	matrix_base<T, N> get_comp_at_basis(const _shared& m) const override; // calc comp of this at basis
	Tensor(const _matrix& comp, const _matrix& basis) : _matrix(comp), _handler(basis) {};
	Tensor(const _matrix& comp, const _shared& basis) : _matrix(comp), _handler(basis) {};
	Tensor(const _matrix& comp, const  Tensor& basis) : _matrix(comp), _handler(static_cast<const _handler&>(basis)()) {};
	Tensor(const  Tensor& tens) : _matrix(static_cast<_matrix>(tens)), _handler(static_cast<const _handler&>(tens)()) {};

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

	virtual T       convolution(const Tensor<T, N>& rhs) const;
	//friend Tensor<T, N> outer_product(const Vector<T, N>& lhs, const Vector<T, N>& rhs);
};

template<typename T, size_t N>
void Tensor<T, N>::change_basis(const shared_handler_object<const matrix_base<T, N>>& m) {
	static_cast<matrix_base<T, N>&>(*this) = get_comp_at_basis(m);
	this->set_basis(m);
}

template<typename T, size_t N>
Tensor<T, N>& Tensor<T, N>::operator = (const T& value) {
	return static_cast<Tensor<T, N>&>(matrix_base<T, N>::operator=(value));
};

template<typename T, size_t N>
Tensor<T, N>& Tensor<T, N>::operator = (const Tensor<T, N>& t)  {
	return static_cast<Tensor<T, N>&>(matrix_base<T, N>::operator=(t.get_comp_at_basis(this->get_basis())));
};

template<typename T, size_t N>
Tensor<T, N>  Tensor<T, N>::operator + (const Tensor<T, N>& t) const {
	return Tensor<T, N>(matrix_base<T, N>::operator+(t.get_comp_at_basis(this->get_basis())), this->get_basis());
}

template<typename T, size_t N>
Tensor<T, N>  Tensor<T, N>::operator - (const Tensor<T, N>& t) const {
	return Tensor<T, N>(matrix_base<T, N>::operator-(t.get_comp_at_basis(this->get_basis())), this->get_basis());
}

template<typename T, size_t N>
Tensor<T, N>  Tensor<T, N>::operator * (const Tensor<T, N>& t) const  {
	return Tensor<T, N>(matrix_base<T, N>::operator*(t.get_comp_at_basis(this->get_basis())), this->get_basis());
}

template<typename T, size_t N>
Tensor<T, N>& Tensor<T, N>::operator +=(const Tensor<T, N>& t)  {
	matrix_base<T, N>::operator+=(t.get_comp_at_basis(this->get_basis()));
	return *this;
}

template<typename T, size_t N>
Tensor<T, N>& Tensor<T, N>::operator -=(const Tensor<T, N>& t)  {
	matrix_base<T, N>::operator-=(t.get_comp_at_basis(this->get_basis()));
	return *this;
}

template<typename T, size_t N>
Tensor<T, N>& Tensor<T, N>::operator *=(const Tensor<T, N>& t){
	matrix_base<T, N>::operator*=(t.get_comp_at_basis(this->get_basis()));
	return *this;
}

template<typename T, std::size_t N>
T   Tensor<T, N>::convolution(const Tensor<T, N>& rhs) const {
	return static_cast<matrix_base<T, N>>(*this).convolution(rhs.get_comp_at_basis(this->get_basis()));
}

/* return components of this in the target basis m*/
template<typename T, size_t N>
matrix_base<T, N> Tensor<T, N>::get_comp_at_basis(const shared_handler_object<const matrix_base<T, N>>& m) const {
	matrix_base<T, N> res;
	if (this->same_basis(m)) {
		res = static_cast<matrix_base<T, N>>(*this);
	}
	else {
		const matrix_base<T, N>& op   = this->get_stored_object() * m.get_stored_object().transpose();
		const matrix_base<T, N>& comp = static_cast<const matrix_base<T, N>&> (*this);
		res = this->transform(TRANSPOSE::TRUE, op, TRANSPOSE::FALSE); // op^t * (*this) * op
	}
	return res;
}

//template<typename T, size_t N>
//Tensor<T, N> outer_product(const Vector<T, N>& lhs, const Vector<T, N>& rhs){
//
//	//const Vector<T, N>& lhs = _lhs * lhs.get_basis_comp();
//	//const Vector<T, N>& rhs ;
//	//matrix_base  <T, N>  m = lhs.outer_product(rhs);
//	return Tensor<T, N>(m, matrix_base<T, N>(MATRIXINITTYPE::INDENT));
//}