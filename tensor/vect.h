#pragma once

#include <iostream>
#include "vectbase.h"
#include "matrbase.h"
#include "basis.h"

//template<typename T, std::size_t N> class Vector;
//template<typename T, std::size_t N> Vector<T, N> vector_product(const Vector<T, N>& lhs, const Vector<T, N>& rhs);

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

	Vector(const  _matrix& basis) : _vector(T()), _handler(basis) {}; // zero vector ctor in basis
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

	friend Vector<T, N> vector_product(const Vector<T, N>& _lhs, const Vector<T, N>& _rhs) {
		static_assert(N == 3 && " vector_product is implemented only for 3-dimensional vectors.");
		Vector<T, N> res(_lhs);
		vect_base<T, N>& comp = static_cast<vect_base<T, N>&>(res);
		const vect_base<T, N> lhs = _lhs.get_comp_at_basis(_rhs);
		const vect_base<T, N>& rhs = static_cast<const vect_base<T, N>&>(_rhs);
		comp[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
		comp[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
		comp[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
		return res;
	}
};

template<typename T, size_t N>
void Vector<T, N>::change_basis(const _handler& m) {
	static_cast<vect_base<T, N>&>(*this) = get_comp_at_basis(m);
	this->set_basis(m);
}

template<typename T, size_t N>
vect_base<T, N> Vector<T, N>::get_comp_at_basis(const   _handler& m) const {
	if (*this == m) {
		return *this;
	}
	else {
		const matrix_base<T, N>& Rl = *this->get();
		const matrix_base<T, N>& Rr = m.get()->transpose();
		const vect_base<T, N>& comp = static_cast<const vect_base<T, N>&> (*this);
		return (comp * Rl) * Rr;
	}
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
	vect_base<T, N>::operator+=(v.get_comp_at_basis(*this));
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


// output component of Tensor at GLOBAL_DEFAULT_BASIS basis
template<typename T, std::size_t N>
std::ostream& operator<<(std::ostream& out, const Vector<T, N>& v) {
	vect_base<T, N> c = v.get_comp_at_basis(GLOBAL_DEFAULT_BASIS<T, N>);
	out << c;
	return out;
};