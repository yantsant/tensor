#pragma once

#include <iostream>
#include "array.h"
#include "matrix.h"

template<typename T, size_t N>
class vect_base : public arr<T, N>
{
public:
	explicit vect_base() { };
	vect_base(T val ) : arr<T, N>(val) { };
	virtual ~vect_base() {};
	inline arr<T, N>& operator()() { return *this; };
	operator arr<T, N>() const { return *this; };
	inline virtual vect_base& operator = (const arr<T, N>& a);
	inline static  vect_base vector_product(const vect_base& lhs, const vect_base& rhs);
	inline virtual vect_base vector_product(const vect_base& rhs) const;
};


template<typename T, size_t N>
class vect : public vect_base<T, N>
{
	bool  basis_owner;
	const matrix_base<T, N>* basis = nullptr;
	void  create_basis(const matrix_base<T, N>* m = nullptr);
	vect_base<T, N> move_to_basis(const vect<T, N>& v) const; // move v[i] to this->basis
public:
	virtual ~vect();
	vect(const vect<T, N>& v);
	vect(const matrix_base<T, N>* m = nullptr);// { basis_owner = false; };// { basis_owner = create_basis(m); };
	vect(const vect_base<T, N>& v, const matrix_base<T, N>* m = nullptr);
	vect(const arr      <T, N>& a, const matrix_base<T, N>* m = nullptr);
	vect(T val, const matrix_base<T, N>* m = nullptr);// { basis_owner = false; };// { basis_owner = create_basis(m); };

	 vect& operator = (const vect<T, N>& v) ;
	 vect  operator + (const vect<T, N>& v) const;
	 vect  operator - (const vect<T, N>& v) const;
	 T     operator * (const vect<T, N>& v) const;
	 vect  vector_product(const vect& rhs) const;
};

template<typename T, size_t N>
inline vect_base<T, N>& vect_base<T, N>::operator = (const arr<T, N>& a) {
	return static_cast<vect_base<T, N>&>(arr<T, N>::operator=(a));
}

template<typename T, size_t N>
vect<T, N>::~vect() {
	if (basis_owner) delete basis;
};

template<typename T, size_t N>
vect<T, N>::vect(const matrix_base<T, N>* m) {
	create_basis(m);
};

template<typename T, size_t N>
vect<T, N>::vect(T val, const matrix_base<T, N>* m) : vect_base<T, N>(val) {
	create_basis(m);
};

template<typename T, size_t N>
vect<T, N>::vect(const vect<T, N>& v) { 
	static_cast<vect_base<T, N>&>(vect_base<T, N>::operator=(v));// (*this) = v;
	create_basis(v.basis);
};
template<typename T, size_t N>
vect<T, N>::vect(const vect_base<T, N>& v, const matrix_base<T, N>* m) 
{ 
	static_cast<vect_base<T, N>&>(vect_base<T, N>::operator=(v)); //static_cast<vect_base<T, N>> (*this) = v;
	create_basis(m);
};

template<typename T, size_t N>
vect<T, N>::vect(const arr<T, N>& a, const matrix_base<T, N>* m)
{
	static_cast<vect_base<T, N>&>(vect_base<T, N>::operator=(a));
	create_basis(m);
};

template<typename T, size_t N>
vect_base<T, N> vect<T, N>::move_to_basis(const vect<T, N>& v) const // move v[i] to this->basis
{
	vect_base<T, N> res;
	if (basis == v.basis)	{
		res = static_cast<vect_base<T, N>>(v);
	}
	else {
		const matrix_base<T, N>& Rl  = *(this->basis);
		const matrix_base<T, N>& Rr  = *(v.basis);
		const vect_base<T, N>& rcomp = static_cast<const vect_base<T, N>&> (v);
		res = (rcomp * Rr) * Rl.transpose();//res = rcomp* Rr.scal(TRANSPOSE::FALSE, Rl, TRANSPOSE::TRUE); /* rcomp. Rr.Rl^T */
	}
	return res;
}

template<typename T, size_t N>
vect<T, N>& vect<T, N>::operator = (const vect<T, N>& v) {
	return static_cast<vect<T, N>&>(vect_base<T, N>::operator=(move_to_basis(v)));
}


template<typename T, size_t N>
vect<T, N> vect<T, N>::operator + (const vect<T, N>& v) const {
	return vect<T, N>(static_cast<arr<T, N>>(*this) + (move_to_basis(v)), this->basis);
}

template<typename T, size_t N>
vect<T, N> vect<T, N>::operator - (const vect<T, N>& v) const {
	return vect<T, N>(static_cast<arr<T, N>>(*this) - (move_to_basis(v)), this->basis);
}

template<typename T, size_t N>
T vect<T, N>::operator * (const vect<T, N>& v) const {
	return static_cast<arr<T, N>>(*this) * (move_to_basis(v));
}

template<typename T, size_t N>
void vect<T, N >::create_basis(const matrix_base<T, N>* m){
	if (basis == nullptr)
	{
		if (m == nullptr)
		{
			basis = new matrix_base<T, N>(MATRIXINITTYPE::INDENT);
			basis_owner = true;
		}
		else
			basis = m;
	}
	basis_owner = false;
}


template<typename T, size_t N>
vect<T, N> vect<T, N >::vector_product(const vect& rhs) const
{
	vect_base<T, N > comp = vect_base<T, N >::vector_product(move_to_basis(rhs));
	return vect<T, N>(comp, basis);
}
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
inline vect_base<T, N> vect_base<T, N >::vector_product(const vect_base<T, N >& rhs) const
{
	return vector_product(*this, rhs);
}