#pragma once

#include <iostream>
#include "array.h"


template<typename T, size_t N>
class vect : public arr<T, N>
{
public:
	explicit vect() {};
	vect(T val) : arr<T, N>(val) {};
	inline arr<T, N>& operator()() { return *this; };
	inline vect& operator = (const arr<T, N>& m);
	inline static vect vector_product(const vect& lhs, const vect& rhs);
	inline vect vector_product(const vect& rhs) const;
};

template<typename T, size_t N>
inline vect<T, N>& vect<T, N>::operator = (const arr<T, N>& v) {
	return static_cast<vect<T,N>&>(arr<T, N>::operator=(v));
}



template<typename T, size_t N>
inline  vect<T, N> vect<T, N >::vector_product (const vect<T, N>& lhs, const vect<T, N >& rhs) {
	if (N == 3)
	{
		vect<T, N> res;
		const std::array<T, N> &lv = lhs.get();
		const std::array<T, N> &rv = rhs.get();
		res.set(0, lv[1] * rv[2] - lv[2] * rv[1]);
		res.set(1, lv[2] * rv[0] - lv[0] * rv[2]);
		res.set(2, lv[0] * rv[1] - lv[1] * rv[0]); 
		return res;
	} else throw std::length_error("vector_product is implemented only for 3-dimensional vectors.");
}


template<typename T, size_t N>
inline vect<T, N> vect<T, N >::vector_product(const vect<T, N >& rhs) const 
{
	return vector_product(*this, rhs);
}