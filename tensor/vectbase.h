#pragma once
#include "container.h"
#include "matrbase.h"

template<typename T, std::size_t N> class vect_base;
template<typename T, std::size_t N> std::ostream& operator<< (std::ostream& o, const vect_base<T, N>& v);

template<typename T, size_t N>
class vect_base : public container <std::array<T, N>>
{
	typedef  std::array<T, N>  _array;
	typedef container <std::array<T, N>> _cont;
	vect_base() : _cont() {  };
protected:
public:
	~vect_base() { };
	explicit vect_base(T val)                 : _cont()  { this->_Elem->fill(val); };
	explicit vect_base(const _array& _arr)    : _cont(static_cast<const _cont&>(_arr)) { };
	explicit vect_base(const vect_base&  v)   : _cont(static_cast<const _cont& >(v))   { }; // copy constructor
	vect_base(vect_base&& v)noexcept : _cont(static_cast<_cont&&>(v))         { }; // move constructor

	      T& operator [](size_t i)          { return (*this->_Elem)[i]; };
	const T& operator [](size_t i) const    { return (*this->_Elem)[i]; };

	friend std::ostream& operator<< <>(std::ostream& out, const vect_base& a);

	inline vect_base& operator= (const vect_base& rhs)	{
		container<_array>::operator = (static_cast<const container<_array>&>(rhs));
		return *this;
	}
	inline vect_base& operator= (vect_base&& rhs) noexcept {
		container<_array>::operator = (static_cast<container<_array>&& >(rhs));
		return *this;
	}
	inline vect_base  operator- () const;
	inline vect_base  operator+ (const vect_base& rhs) const;
	inline vect_base  operator- (const vect_base& rhs) const;
	inline         T  operator* (const vect_base& v) const;
	inline vect_base  operator* (const T& mult) const;
	inline vect_base  operator/ (const T& div) const;
	inline vect_base& operator= (const T& val);
	inline vect_base& operator/=(const T& div);
	inline vect_base& operator*=(const T& mult);
	inline vect_base& operator+=(const vect_base& rhs);
	inline vect_base& operator-=(const vect_base& rhs);

	inline vect_base         vector_product(const vect_base& rhs) const;
	inline matrix_base<T, N>  outer_product(const vect_base& rhs) const;

	virtual inline T   norm() const;
	inline vect_base   get_normalize() const;
	inline void        normalize();
	inline static void normalize(vect_base&);

	friend vect_base<T, N > operator* (const vect_base<T, N >& v, const matrix_base<T, N >& m) {
		vect_base <T, N> res((T)0);
		for (size_t row = 0; row < N; row++)
			for (size_t col = 0; col < N; col++)
				res[row] += v[col] * m[col][row];

		return res;
	}

	friend vect_base<T, N > operator* (const matrix_base<T, N >& m, const vect_base<T, N >& v) {
		vect_base <T, N> res((T)0);
		for (size_t row = 0; row < N; row++)
			for (size_t col = 0; col < N; col++)
				res[row] += v[col] * m[row][col];

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
	static_assert(N == 3 && " vector_product is implemented only for 3-dimensional vectors.");

	const vect_base<T, N>& lhs = *this;
	vect_base<T, N> res;
	res[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
	res[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
	res[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
	return res;
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
inline T vect_base<T, N >::operator* (const vect_base<T, N >& v) const {
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
	assert(length > (T)0 && "length of vector is zero");
	v *= (T)1 / length;
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
