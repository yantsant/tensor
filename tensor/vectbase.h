#pragma once
#include <array>
#include "matrbase.h"

template<typename T, std::size_t N> class vect_base;
template<typename T, std::size_t N> std::ostream& operator<< (std::ostream& o, const vect_base<T, N>& v);

template<typename T, size_t N>
class vect_base : private std::array<T, N>
{
	typedef  std::array<T, N>  _array;
public:
	explicit vect_base()             { this->fill((T)0); };
	explicit vect_base(T val)        { this->fill(val); };
	explicit vect_base(_array& _arr) { *this = _arr;};
	inline const _array& operator()() const { return *this; };
	vect_base(const vect_base&  v) : _array(v) {}; // copy constructor
	vect_base(const vect_base&& v) : _array(v) {}; // move constructor

	void  set_comp(size_t idx, const T& value);
	void  set_comp(const vect_base&);
	T     get_comp(size_t idx) const;
	void  get_comp(_array&) const;
	const std::array<T, N>& get_comp() const;

	friend std::ostream& operator<< <>(std::ostream& out, const vect_base& a);

	inline vect_base  operator- () const;
	inline vect_base& operator= (const vect_base& rhs);
	inline vect_base  operator+ (const vect_base& rhs) const;
	inline vect_base  operator- (const vect_base& rhs) const;
	virtual inline T  operator* (const vect_base& v) const;
	inline vect_base  operator* (const T& mult) const;
	inline vect_base  operator/ (const T& div) const;
	inline vect_base& operator= (const T& val);
	inline vect_base& operator/=(const T& div);
	inline vect_base& operator*=(const T& mult);
	inline vect_base& operator+=(const vect_base& rhs);
	inline vect_base& operator-=(const vect_base& rhs);

	inline virtual vect_base         vector_product(const vect_base& rhs) const;
	inline virtual matrix_base<T, N>  outer_product(const vect_base& rhs) const;

	virtual inline T   norm() const;
	inline vect_base   get_normalize() const;
	inline void        normalize();
	inline static void normalize(vect_base&);

	friend vect_base<T, N > operator* (const vect_base<T, N >& v, const matrix_base<T, N >& m) {
		vect_base <T, N> res((T)0);
		const std::array<std::array<T, N>, N>& matr = m();
		for (size_t row = 0; row < N; row++)
			for (size_t col = 0; col < N; col++)
				res[row] += v[col] * matr[col][row];

		return res;
	}

	friend vect_base<T, N > operator* (const matrix_base<T, N >& m, const vect_base<T, N >& v) {
		vect_base <T, N> res((T)0);
		const std::array<std::array<T, N>, N>& matr = m();
		for (size_t row = 0; row < N; row++)
			for (size_t col = 0; col < N; col++)
				res[row] += v[col] * matr[row][col];

		return res;
	}

};

template<typename T, size_t N>
inline vect_base<T, N> vect_base<T,N>::operator- () const {
	vect_base<T, N> res;
	for (size_t row = 0; row < N; row++)
		res[row] = -(*this)[row];
	return res;
};

template<typename T, size_t N>
inline vect_base<T, N>& vect_base<T, N>::operator= (const vect_base& rhs) {
	vect_base<T, N> res;
	for (size_t row = 0; row < N; row++)
		(*this)[row] = rhs[row];
	return *this;
};

template<typename T, size_t N>
inline vect_base<T, N> vect_base<T, N>::operator+ (const vect_base& rhs) const {
	vect_base<T, N> res;
	for (size_t row = 0; row < N; row++)
		res[row] = (*this)[row] + rhs[row];
	return res;
};

template<typename T, size_t N>
inline vect_base<T, N> vect_base<T, N>::operator- (const vect_base& rhs) const {
	vect_base<T, N> res;
	for (size_t row = 0; row < N; row++)
		res[row] = (*this)[row] - rhs[row];
	return res;
};

template<typename T, size_t N>
inline vect_base<T, N> vect_base<T, N>::operator * (const  T& mult) const {
	vect_base<T, N> res;
	for (size_t row = 0; row < N; row++)
		res[row] = (*this)[row] * mult;
	return res;
};

template<typename T, size_t N>
inline vect_base<T, N> vect_base<T, N>::operator / (const  T& div) const {
	vect_base<T, N> res;
	if (abs(div) > (T)0) {
		T mult = (T)1 / div;
		for (size_t row = 0; row < N; row++)
			res[row] = (*this)[row] * mult;
	}
	return res;
};

template<typename T, size_t N>
inline vect_base<T, N>& vect_base<T, N>::operator /= (const  T& div) {
	vect_base<T, N> res;
	if (abs(div) > (T)0) {
		T mult = (T)1 / div;
		for (size_t row = 0; row < N; row++)
			(*this)[row] *= mult;
	}
	return *this;
};

template<typename T, size_t N>
inline vect_base<T, N>& vect_base<T, N>::operator = (const T& val) {
	fill(val);
	return (*this);
}

template<typename T, size_t N>
inline vect_base<T, N>& vect_base<T, N>::operator *=(const T& mult) {
	for (size_t row = 0; row < N; row++)
		(*this)[row] *= mult;
	return (*this);
};

template<typename T, size_t N>
inline vect_base<T, N>& vect_base<T, N>::operator +=(const vect_base& rhs) {
	for (size_t row = 0; row < N; row++)
		(*this)[row] += rhs[row];
	return *this;
}

template<typename T, size_t N>
inline vect_base<T, N>& vect_base<T, N>::operator -=(const vect_base& rhs) {
	for (size_t row = 0; row < N; row++)
		(*this)[row] -= rhs[row];
	return *this;
}

template<typename T, size_t N>
std::ostream& operator<<(std::ostream& out, const vect_base<T,N>& a){
	for (auto row : a)
		out << row << " ";
	out << "\n";
	return out;
}

template<typename T, size_t N>
inline vect_base<T, N> vect_base<T, N >::vector_product(const vect_base<T, N >& rhs) const {
	if (N == 3){
		const vect_base<T, N>& lhs = *this;
		vect_base<T, N> res;
		res[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
		res[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
		res[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
		return res;
	}
	else throw std::length_error("vector_product is implemented only for 3-dimensional vectors.");
}

template<typename T, size_t N>
inline matrix_base<T, N> vect_base<T, N >::outer_product(const vect_base<T, N >& rhs) const {
	const vect_base<T, N>& lhs = *this;
	std::array< std::array<T, N>,N> res;
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			res[row][col] = lhs[row] * rhs[col];
	return static_cast<matrix_base<T, N>> (res);
}

template<typename T, size_t N>
inline void vect_base<T, N >::set_comp(size_t idx, const T& value){
	if (idx > N - 1 || N < 0) throw std::out_of_range("invalid index");
	(*this)[idx] = value;
}

template<typename T, size_t N>
inline void vect_base<T, N >::set_comp(const vect_base<T, N>& v) {
	*this = v;
}

template<typename T, size_t N>
T    vect_base<T, N >::get_comp(size_t idx) const{
	return (*this)[idx];
}
template<typename T, size_t N>
void vect_base<T, N >::get_comp(std::array<T, N>& out) const {
	out = *this;
}

template<typename T, size_t N>
const std::array<T, N>& vect_base<T, N >::get_comp() const {
	return *this;
}

template<typename T, size_t N>
inline T vect_base<T, N >::operator* (const vect_base<T, N >& v) const {
	if (N == 0) throw std::length_error("zero length vector!");

	T res = (T)0;
	for (size_t row = 0; row < N; row++)
		res += (*this)[row] * v[row];

	return res;
}

template<typename T, size_t N>
inline T    vect_base<T, N >::norm() const {
	const vect_base<T, N >& v = (*this);
	return sqrt(v * v);
}

template<typename T, size_t N>
inline void vect_base<T, N>::normalize(vect_base<T, N>& v) {
	T length = v.norm();
	if (length > (T)0)
		v *= (T)1 / length;
	else
		throw std::invalid_argument("length of vector is zero");
}


template<typename T, size_t N>
inline vect_base<T, N > vect_base<T, N>::get_normalize() const {
	vect_base<T, N> res(*this);
	normalize(res);
	return res;
}

template<typename T, size_t N>
inline void vect_base<T, N >::normalize() {
	normalize(*this);
}
