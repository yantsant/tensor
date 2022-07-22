#pragma once

#include <iostream>
#include "vectbase.h"
#include "matrbase.h"
#include "basis.h"

template<typename T, size_t N>
class Vector : private vect_base<T, N>,
			   private shared_handler_basis<T, N>
	           
{
	typedef  vect_base    <T, N>          _vector;
	typedef  matrix_base  <T, N>          _matrix;
	typedef  shared_handler_basis<T, N>  _handler;
public:
	virtual void      change_basis     (const _handler& m) override;
	vect_base<T, N>   get_comp_at_basis(const _handler& m) const; // calc comp of this at basis

	Vector(const _vector& comp, const  _matrix& basis) : _vector(comp), _handler(basis) {};
	Vector(const _vector& comp, const _handler& basis) : _vector(comp), _handler(basis) {};
	Vector(const  Vector&  vect)  : _vector(vect), _handler(vect) {}; // copy constructor
	Vector(Vector&& vect) noexcept: _vector(static_cast<_vector&&>(vect)), _handler(static_cast<_handler&&>(vect))  {}; // move constructor

	inline Vector  operator - ();
	inline Vector& operator = (const Vector<T, N>& v);
	inline Vector& operator = (Vector<T, N>&& v) noexcept;
	inline Vector& operator +=(const Vector<T, N>& v);
	inline Vector& operator -=(const Vector<T, N>& v);
	inline Vector  operator + (const Vector<T, N>& v) const;
	inline Vector  operator - (const Vector<T, N>& v) const;
	inline T       operator * (const Vector<T, N>& v) const;
	inline Vector  operator * (const T& mult) const;
	inline Vector  vector_product(const Vector& rhs)  const;
};

template<typename T, size_t N>
void Vector<T, N>::change_basis(const _handler& m) {
	static_cast<vect_base<T, N>&>(*this) = get_comp_at_basis(m);
	this->set_basis(m);
}

template<typename T, size_t N>
vect_base<T, N> Vector<T, N>::get_comp_at_basis(const   _handler& m) const {
	vect_base<T, N> res;
	if (*this == m) {
		res = static_cast<vect_base<T, N>>(*this);
	}
	else {
		const matrix_base<T, N>& Rl = *this->get();
		const matrix_base<T, N>& Rr = m.get()->transpose();
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
	_vector ::operator = (static_cast<const _vector &>(v));
	_handler::operator = (static_cast<const _handler&>(v));
	return *this;
};

template<typename T, size_t N>
Vector<T, N>& Vector<T, N>::operator =  (Vector&& v) noexcept {
	_vector ::operator = (static_cast<_vector &&>(v));
	_handler::operator = (static_cast<_handler&&>(v));
	return*this;
};

template<typename T, size_t N>
Vector<T, N> Vector<T, N>::operator + (const Vector<T, N>& v) const {
	return  Vector<T, N>(vect_base<T, N>::operator+(v.get_comp_at_basis(*this)), *this);
}

template<typename T, size_t N>
Vector<T, N> Vector<T, N>::operator - (const Vector<T, N>& v) const {
	return  Vector<T, N>(vect_base<T, N>::operator-(v.get_comp_at_basis(*this)), *this);
}

template<typename T, size_t N>
Vector<T, N>& Vector<T, N>::operator +=(const Vector<T, N>& v) {
	vect_base<T, N>::operator+=(v.comp_at_basis(*this));
	return *this;
}

template<typename T, size_t N>
Vector<T, N>& Vector<T, N>::operator -=(const Vector<T, N>& v) {
	vect_base<T, N>::operator-=(v.get_comp_at_basis(*this));
	return *this;
}

template<typename T, size_t N>
T Vector<T, N>::operator * (const Vector<T, N>& v) const {
	return vect_base<T, N>::operator*(v.get_comp_at_basis(*this));
}

template<typename T, size_t N>
Vector<T, N> Vector<T, N>::operator * (const T& mult) const {
	return Vector<T, N>(vect_base<T, N>::operator*(mult), *this);
}

template<typename T, size_t N>
Vector<T, N> Vector<T, N >::vector_product(const Vector& rhs) const{
	vect_base<T, N > comp = vect_base<T, N >::vector_product(rhs.get_comp_at_basis(*this));
	return Vector<T, N>(comp, this->get_basis());
}
