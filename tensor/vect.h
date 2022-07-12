#pragma once

#include <iostream>
#include "array.h"
#include "matrix.h"
template<typename T, size_t N>
const matrix_base<T, N> GLOBAL_DEFAULT_BASIS = matrix_base<T, N>(MATRIXINITTYPE::INDENT);


template<typename T, size_t N>
class basis_handler
{
	const matrix_base<T, N>* basis_comp = nullptr;
protected:
	bool same_basis(const matrix_base<T, N>& m) const { return (basis_comp == &m) ? true : false; };
	const matrix_base<T, N>& basis() const { return *basis_comp; };
public:
	basis_handler(const matrix_base<T, N>& m);
};


template<typename T, size_t N>
basis_handler<T, N >::basis_handler(const matrix_base<T, N>& m) {
	if (!m.check_ort()) throw std::invalid_argument("basis matrix is not ortogonal");
	basis_comp = &m;
}


template<typename T, size_t N>
class vect_base : public arr<T, N>
{
public:
	explicit vect_base() { };
	vect_base(const T& val ) : arr<T, N>(val) { };
	virtual ~vect_base() {};
	inline arr<T, N>& operator()() { return *this; };
	operator arr<T, N>() const { return *this; };
	inline virtual vect_base& operator = (const arr<T, N>& a);
	inline static  vect_base vector_product(const vect_base& lhs, const vect_base& rhs);
	inline virtual vect_base vector_product(const vect_base& rhs) const;
};


template<typename T, size_t N>
inline  vect_base<T, N> vect_base<T, N >::vector_product(const vect_base<T, N>& lhs, const vect_base<T, N >& rhs) {
	if (N == 3)
	{
		vect_base<T, N> res;
		const std::array<T, N>& lv = lhs.get();
		const std::array<T, N>& rv = rhs.get();
		res.set(0, lv[1] * rv[2] - lv[2] * rv[1]);
		res.set(1, lv[2] * rv[0] - lv[0] * rv[2]);
		res.set(2, lv[0] * rv[1] - lv[1] * rv[0]);
		return res;
	}
	else throw std::length_error("vector_product is implemented only for 3-dimensional vectors.");
}

template<typename T, size_t N>
inline vect_base<T, N> vect_base<T, N >::vector_product(const vect_base<T, N >& rhs) const {
	return vector_product(*this, rhs);
}


template<typename T, size_t N>
class vect : private basis_handler<T, N>, public vect_base<T, N>
{
public:
	vect_base<T, N> comp_at_basis(const matrix_base<T, N>& m) const; // calc comp of this at basis

	vect(const matrix_base<T, N>& m) : basis_handler<T, N>(m) {	};
	vect(const T& val            , const matrix_base<T, N>& m) : vect_base<T, N>(val), basis_handler<T, N>(m) {};
	vect(const vect_base<T, N>& v, const matrix_base<T, N>& m) : vect_base<T, N>(v), basis_handler<T, N>(m) {};
	vect(const vect<T, N>& v     , const matrix_base<T, N>& m);
	vect(const arr      <T, N>& a, const matrix_base<T, N>& m);

	inline vect& operator = (const vect<T, N>& v) ;
	inline vect  operator + (const vect<T, N>& v) const;
	inline vect  operator - (const vect<T, N>& v) const;
	inline T     operator * (const vect<T, N>& v) const;
	inline vect  operator * (const T& mult) const;
	inline vect  vector_product(const vect& rhs)  const;
};

template<typename T, size_t N>
inline vect_base<T, N>& vect_base<T, N>::operator = (const arr<T, N>& a) {
	return static_cast<vect_base<T, N>&>(arr<T, N>::operator=(a));
}

template<typename T, size_t N>
vect<T, N>::vect(const vect<T, N>& v, const matrix_base<T, N>& m) : basis_handler<T, N>(m) {
	static_cast<vect_base<T, N>&>(vect_base<T, N>::operator=(v));
};

template<typename T, size_t N>
vect<T, N>::vect(const arr<T, N>& a, const matrix_base<T, N>& m) : basis_handler<T, N>(m){
	static_cast<vect_base<T, N>&>(vect_base<T, N>::operator=(a));
};

template<typename T, size_t N>
vect_base<T, N> vect<T, N>::comp_at_basis(const matrix_base<T, N>& m) const{
	vect_base<T, N> res;
	if (this->same_basis(m)) {
		res = *this;
	}
	else {
		const matrix_base<T, N>& Rl = this->basis();
		const matrix_base<T, N>& Rr = m.transpose();
		const vect_base<T, N>& comp = static_cast<const vect_base<T, N>&> (*this);

		res = (comp * Rl) * Rr; // (*this) * this->basis() * m.transpose();
	}
	return res;
}

template<typename T, size_t N>
vect<T, N>& vect<T, N>::operator = (const vect<T, N>& v) {
	return static_cast<vect<T, N>&>(vect_base<T, N>::operator=(v.comp_at_basis(this->basis())));
}

template<typename T, size_t N>
vect<T, N> vect<T, N>::operator + (const vect<T, N>& v) const {
	return  vect<T, N>(static_cast<arr<T, N>>(*this) + (v.comp_at_basis(this->basis())), this->basis());
}

template<typename T, size_t N>
vect<T, N> vect<T, N>::operator - (const vect<T, N>& v) const {
	return vect<T, N>(static_cast<arr<T, N>>(*this) - (v.comp_at_basis(this->basis())), this->basis());
}

template<typename T, size_t N>
T vect<T, N>::operator * (const vect<T, N>& v) const {
	return static_cast<arr<T, N>>(*this) * (v.comp_at_basis(this->basis()));
}

template<typename T, size_t N>
vect<T, N> vect<T, N>::operator * (const T& mult) const {
	return vect<T, N>(vect_base<T, N>::operator*(mult), this->basis());
}

template<typename T, size_t N>
vect<T, N> vect<T, N >::vector_product(const vect& rhs) const{
	vect_base<T, N > comp = vect_base<T, N >::vector_product(rhs.comp_at_basis(this->basis()));
	return vect<T, N>(comp, this->basis());
}
