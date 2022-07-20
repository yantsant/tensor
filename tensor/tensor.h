#pragma once
#include "matrbase.h"
#include "quat.h"
#include "vect.h"
#include "basis.h"

template<typename T, std::size_t N> class Tensor;
template<typename T, std::size_t N> class Vector;
template<typename T, std::size_t N> std::ostream& operator<< (std::ostream& o, const Tensor<T, N>& t);
template<typename T, std::size_t N> Tensor<T,N> outer_product (const Vector<T, N>& lhs, const Vector<T, N>& rhs);

template<typename T, size_t N>
class Tensor : private shared_handler_basis<T, N>,
			   private matrix_base<T, N>
{
	typedef  matrix_base  <T, N>        _matrix;
	typedef  shared_handler_basis<T, N> _handler;
public:
	virtual void      change_basis     (const _handler& m) override;
	matrix_base<T, N> get_comp_at_basis(const _handler& m) const; // calc comp of this at basis
	Tensor(const _matrix& comp, const _handler& basis) : _matrix(comp), _handler(basis) {};
	Tensor(const _matrix& comp, const   Tensor& basis) : _matrix(comp), _handler(basis) {};
	Tensor(const  Tensor&  tens) : _matrix(tens), _handler(tens) {}; // copy constructor
	Tensor(const  Tensor&& tens) : _matrix(tens), _handler(tens) {}; // move constructor

	friend std::ostream& operator<< <>(std::ostream& out, const Tensor& t);
	virtual Tensor& operator = (const Tensor& t);
	virtual Tensor& operator = (Tensor&& t) noexcept;
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
	friend Tensor<T, N> outer_product(const Vector<T, N>& lhs, const Vector<T, N>& rhs);
};

template<typename T, size_t N>
void Tensor<T, N>::change_basis(const _handler& m) {
	static_cast<matrix_base<T, N>&>(*this) = get_comp_at_basis(m);
	this->set_basis(m);
}

template<typename T, size_t N>
Tensor<T, N>& Tensor<T, N>::operator = (Tensor<T, N>&& t) noexcept {
	matrix_base<T, N>::operator=(static_cast<matrix_base<T, N>&&>(t));// matrix_base<T, N>::move(static_cast<matrix_base<T, N>&&>(t));
	shared_handler_basis<T, N>::move(static_cast<shared_handler_basis<T, N>&&>(t));
	return *this;
};

template<typename T, size_t N>
Tensor<T, N>& Tensor<T, N>::operator = (const Tensor<T, N>& t)  {
	static_cast<matrix_base<T, N>&>(matrix_base<T, N>::operator=(t));
	static_cast<shared_handler_basis<T, N>&>(shared_handler_basis<T, N>::operator=(t));
	return *this;
};

template<typename T, size_t N>
Tensor<T, N>  Tensor<T, N>::operator + (const Tensor<T, N>& t) const {
	return Tensor<T, N>(matrix_base<T, N>::operator+(t.get_comp_at_basis(*this)), *this);
}

template<typename T, size_t N>
Tensor<T, N>  Tensor<T, N>::operator - (const Tensor<T, N>& t) const {
	return Tensor<T, N>(matrix_base<T, N>::operator-(t.get_comp_at_basis(*this)), *this);
}

template<typename T, size_t N>
Tensor<T, N>  Tensor<T, N>::operator * (const Tensor<T, N>& t) const  {
	return Tensor<T, N>(matrix_base<T, N>::operator*(t.get_comp_at_basis(*this)), *this);
}

template<typename T, size_t N>
Tensor<T, N>& Tensor<T, N>::operator +=(const Tensor<T, N>& t)  {
	matrix_base<T, N>::operator+=(t.get_comp_at_basis(*this));
	return *this;
}

template<typename T, size_t N>
Tensor<T, N>& Tensor<T, N>::operator -=(const Tensor<T, N>& t)  {
	matrix_base<T, N>::operator-=(t.get_comp_at_basis(*this));
	return *this;
}

template<typename T, size_t N>
Tensor<T, N>& Tensor<T, N>::operator *=(const Tensor<T, N>& t){
	matrix_base<T, N>::operator*=(t.get_comp_at_basis(*this));
	return *this;
}

template<typename T, std::size_t N>
T   Tensor<T, N>::convolution(const Tensor<T, N>& rhs) const {
	return static_cast<matrix_base<T, N>>(*this).convolution(rhs.get_comp_at_basis(*this));
}

/* return components of this in the target basis m*/
template<typename T, size_t N>
matrix_base<T, N> Tensor<T, N>::get_comp_at_basis(const   shared_handler_basis<T, N>& m) const {
	matrix_base<T, N> res (MATRIXINITTYPE::NOINIT);
	if (*this == m) {
		res = static_cast<matrix_base<T, N>>(*this);
	}
	else {
		const matrix_base<T, N>& op   = *this->get() * m.get()->transpose();
		const matrix_base<T, N>& comp = static_cast<const matrix_base<T, N>&> (*this);
		res = this->transform(TRANSPOSE::TRUE, op, TRANSPOSE::FALSE); // op^t * (*this) * op
	}
	return res;
}

template<typename T, size_t N>
Tensor<T, N> outer_product(const Vector<T, N>& _lhs, const Vector<T, N>& _rhs){
	const vect_base<T, N>& lhs = _lhs.get_comp_at_basis(GLOBAL_DEFAULT_BASIS<T, N>);
	const vect_base<T, N>& rhs = _rhs.get_comp_at_basis(GLOBAL_DEFAULT_BASIS<T, N>);
	matrix_base  <T, N>  m = lhs.outer_product(rhs);
	return Tensor<T, N>(m, GLOBAL_DEFAULT_BASIS<T, N>);
}

// output component of Tensor at GLOBAL_DEFAULT_BASIS basis
template<typename T, std::size_t N>
std::ostream& operator<<(std::ostream& out, const Tensor<T, N>& t) {
	const matrix_base<T, N>& m = t.get_comp_at_basis(GLOBAL_DEFAULT_BASIS<T, N>);
	out << m;
	return out;
};
