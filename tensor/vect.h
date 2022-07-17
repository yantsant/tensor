#pragma once

#include <iostream>
#include "vectbase.h"
#include "matrbase.h"
#include "basis.h"
template<typename T, size_t N>
const matrix_base<T, N> GLOBAL_DEFAULT_BASIS = matrix_base<T, N>(MATRIXINITTYPE::INDENT);



template<typename T, size_t N>
class Vector : private basis_handler<T, N, matrix_base<T, N>, vect_base<T, N>>,
			   protected vect_base<T, N>
{
	typedef  vect_base    <T, N>          _vector;
	typedef  matrix_base  <T, N>          _matrix;
	typedef  basis_handler<T, N, _matrix, _vector> _handler;
	typedef  shared_handler_object<const _matrix>  _shared;
public:
	virtual void      change_basis     (const _shared& m) override;
	vect_base<T, N>   get_comp_at_basis(const _shared& m) const override; // calc comp of this at basis

	Vector(const _vector& comp, const _matrix& basis) : _vector(comp), _handler(basis) {};
	Vector(const _vector& comp, const _shared& basis) : _vector(comp), _handler(basis) {};
	Vector(const  Vector& vect) : _vector(static_cast<_vector>(vect)), _handler(static_cast<const _handler&>(vect)()) {};

	inline Vector  operator - ();
	inline Vector& operator = (const Vector<T, N>& v);
	inline Vector& operator +=(const Vector<T, N>& v);
	inline Vector& operator -=(const Vector<T, N>& v);
	inline Vector  operator + (const Vector<T, N>& v) const;
	inline Vector  operator - (const Vector<T, N>& v) const;
	inline T       operator * (const Vector<T, N>& v) const;
	inline Vector  operator * (const T& mult) const;
	inline Vector  vector_product(const Vector& rhs)  const;
};

template<typename T, size_t N>
void Vector<T, N>::change_basis(const shared_handler_object<const matrix_base<T, N>>& m) {
	static_cast<vect_base<T, N>&>(*this) = get_comp_at_basis(m);
	this->set_basis(m);
}

template<typename T, size_t N>
vect_base<T, N> Vector<T, N>::get_comp_at_basis(const shared_handler_object<const matrix_base<T, N>>& m) const {
	vect_base<T, N> res;
	if (this->same_basis(m)) {
		res = static_cast<vect_base<T, N>>(*this);
	}
	else {
		const matrix_base<T, N>& Rl = this->get_stored_object();
		const matrix_base<T, N>& Rr = m.get_stored_object().transpose()();
		const vect_base<T, N>& comp = static_cast<const vect_base<T, N>&> (*this);
		res = (comp * Rl) * Rr;
	}
	return res;
}

template<typename T, size_t N>
Vector<T, N> Vector<T, N>::operator - () {
	return  Vector<T, N>(vect_base<T, N>::operator-(), this->get_basis());
}

template<typename T, size_t N>
Vector<T, N>& Vector<T, N>::operator = (const Vector<T, N>& v) {
	return static_cast<Vector<T, N>&>(vect_base<T, N>::operator=(v.get_comp_at_basis(this->get_basis())));
}

template<typename T, size_t N>
Vector<T, N> Vector<T, N>::operator + (const Vector<T, N>& v) const {
	return  Vector<T, N>(vect_base<T, N>::operator+(v.get_comp_at_basis(this->get_basis())), this->get_basis());
}

template<typename T, size_t N>
Vector<T, N> Vector<T, N>::operator - (const Vector<T, N>& v) const {
	return  Vector<T, N>(vect_base<T, N>::operator-(v.get_comp_at_basis(this->get_basis())), this->get_basis());
}

template<typename T, size_t N>
Vector<T, N>& Vector<T, N>::operator +=(const Vector<T, N>& v) {
	vect_base<T, N>::operator+=(v.comp_at_basis(this->get_basis()));
	return *this;
}

template<typename T, size_t N>
Vector<T, N>& Vector<T, N>::operator -=(const Vector<T, N>& v) {
	vect_base<T, N>::operator-=(v.get_comp_at_basis(this->get_basis()));
	return *this;
}

template<typename T, size_t N>
T Vector<T, N>::operator * (const Vector<T, N>& v) const {
	return vect_base<T, N>::operator*(v.get_comp_at_basis(this->get_basis()));
}

template<typename T, size_t N>
Vector<T, N> Vector<T, N>::operator * (const T& mult) const {
	return Vector<T, N>(vect_base<T, N>::operator*(mult), this->get_basis());
}

template<typename T, size_t N>
Vector<T, N> Vector<T, N >::vector_product(const Vector& rhs) const{
	vect_base<T, N > comp = vect_base<T, N >::vector_product(rhs.get_comp_at_basis(this->get_basis()));
	return Vector<T, N>(comp, this->get_basis());
}
