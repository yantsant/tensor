#pragma once

#include <iostream>
#include "vectbase.h"
#include "matrix.h"
template<typename T, size_t N>
const matrix_base<T, N> GLOBAL_DEFAULT_BASIS = matrix_base<T, N>(MATRIXINITTYPE::INDENT);


template<typename T, size_t N>
class basis_handler
{
	const matrix_base<T, N>* basis_comp = nullptr;
protected:
	bool same_basis(const matrix_base<T, N>& m) const { return (basis_comp == &m) ? true : false; };
	const matrix_base<T, N>& get_basis() const { return *basis_comp; }
	const void               set_basis(const matrix_base<T, N>& m) { basis_comp = &m; };
public:
	basis_handler(const matrix_base<T, N>& m);
};


template<typename T, size_t N>
basis_handler<T, N >::basis_handler(const matrix_base<T, N>& m) {
	if (!m.check_ort()) throw std::invalid_argument("basis matrix is not ortogonal");
	basis_comp = &m;
}



template<typename T, size_t N>
class Vector : private basis_handler<T, N>, public vect_base<T, N>
{
public:
	vect_base<T, N> comp_at_basis(const matrix_base<T, N>& m) const; // calc comp of this at basis

	Vector(const matrix_base<T, N>& m) : basis_handler<T, N>(m) {	};
	Vector(const T& val             , const matrix_base<T, N>& m) : vect_base<T, N>(val), basis_handler<T, N>(m) {};
	Vector(const vect_base<T, N>& v , const matrix_base<T, N>& m) : vect_base<T, N>(v)  , basis_handler<T, N>(m) {};
	Vector(const Vector<T, N>& v    , const matrix_base<T, N>& m);
	Vector(const std::array<T, N>& a, const matrix_base<T, N>& m);

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
Vector<T, N>::Vector(const Vector<T, N>& v, const matrix_base<T, N>& m) : basis_handler<T, N>(m) {
	static_cast<vect_base<T, N>&>(vect_base<T, N>::operator=(v));
};

template<typename T, size_t N>
Vector<T, N>::Vector(const std::array<T, N>& a, const matrix_base<T, N>& m) : basis_handler<T, N>(m){
	static_cast< std::array<T, N>&>(std::array<T, N>::operator=(a));
};

template<typename T, size_t N>
vect_base<T, N> Vector<T, N>::comp_at_basis(const matrix_base<T, N>& m) const{
	vect_base<T, N> res;
	if (this->same_basis(m)) {
		res = *this;
	}
	else {
		const matrix_base<T, N>& Rl = this->get_basis();
		const matrix_base<T, N>& Rr = m.transpose();
		const vect_base<T, N>& comp = static_cast<const vect_base<T, N>&> (*this);

		res = (comp * Rl) * Rr; // (*this) * this->basis() * m.transpose();
	}
	return res;
}

template<typename T, size_t N>
Vector<T, N> Vector<T, N>::operator - () {
	return  Vector<T, N>(vect_base<T, N>::operator-(), this->get_basis());
}

template<typename T, size_t N>
Vector<T, N>& Vector<T, N>::operator = (const Vector<T, N>& v) {
	return static_cast<Vector<T, N>&>(vect_base<T, N>::operator=(v.comp_at_basis(this->get_basis())));
}

template<typename T, size_t N>
Vector<T, N> Vector<T, N>::operator + (const Vector<T, N>& v) const {
	return  Vector<T, N>(vect_base<T, N>::operator+(v.comp_at_basis(this->get_basis())), this->get_basis());
}

template<typename T, size_t N>
Vector<T, N> Vector<T, N>::operator - (const Vector<T, N>& v) const {
	return  Vector<T, N>(vect_base<T, N>::operator-(v.comp_at_basis(this->get_basis())), this->get_basis());
}

template<typename T, size_t N>
Vector<T, N>& Vector<T, N>::operator +=(const Vector<T, N>& v) {
	vect_base<T, N>::operator+=(v.comp_at_basis(this->get_basis()));
	return *this;
}

template<typename T, size_t N>
Vector<T, N>& Vector<T, N>::operator -=(const Vector<T, N>& v) {
	vect_base<T, N>::operator-=(v.comp_at_basis(this->get_basis()));
	return *this;
}

template<typename T, size_t N>
T Vector<T, N>::operator * (const Vector<T, N>& v) const {
	return vect_base<T, N>::operator*(v.comp_at_basis(this->get_basis()));
}

template<typename T, size_t N>
Vector<T, N> Vector<T, N>::operator * (const T& mult) const {
	return Vector<T, N>(vect_base<T, N>::operator*(mult), this->get_basis());
}

template<typename T, size_t N>
Vector<T, N> Vector<T, N >::vector_product(const Vector& rhs) const{
	vect_base<T, N > comp = vect_base<T, N >::vector_product(rhs.comp_at_basis(this->get_basis()));
	return Vector<T, N>(comp, this->get_basis());
}
