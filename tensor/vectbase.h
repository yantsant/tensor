#pragma once
#include <array>
#include "matrix.h"

template<typename T, size_t N>
class vect_base : private std::array<T, N>
{
public:
	explicit vect_base() { *this = (T)0; };
	vect_base(T val) { *this = val; };
	vect_base(std::array<T, N>& _arr) {(*this) = _arr;};

	void set(size_t idx, const T& value);
	void set(const vect_base&);
	T    get(size_t idx) const;
	void get(std::array<T, N>&) const;
	const std::array<T, N>& get() const;

	inline static  vect_base vector_product(const vect_base& lhs, const vect_base& rhs);
	inline virtual vect_base vector_product(const vect_base& rhs) const;

	friend std::ostream& operator<<(std::ostream& out, const vect_base& a)
	{
		for (auto row : a)
		{
			out << row << " ";
		}
		out << "\n";
		return out;
	}

	virtual inline T    operator * (const vect_base& v) const;
	virtual inline vect_base  operator * (const matrix_base<T,N>& m) const;
	virtual inline T    norm() const;

	inline std::array<T, N>& operator()() { return *this; };

	inline vect_base  operator- () const {
		vect_base<T, N> res;

		if (N > 0)
			for (size_t row = 0; row < N; row++)
				res[row] = -(*this)[row];

		return res;
	};

	inline vect_base  operator+ (const vect_base& rhs) const {
		vect_base<T, N> res;

		if (N > 0)
			for (size_t row = 0; row < N; row++)
				res[row] = (*this)[row] + rhs[row];

		return res;
	};


	inline vect_base  operator- (const vect_base& rhs) const {
		vect_base<T, N> res;

		if (N > 0)
			for (size_t row = 0; row < N; row++)
				res[row] = (*this)[row] - rhs[row];

		return res;
	};

	inline vect_base operator * (const  T& mult) const {
		vect_base<T, N> res;

		if (N > 0)
			for (size_t row = 0; row < N; row++)
				res[row] = (*this)[row] * mult;

		return res;
	};

	inline vect_base operator / (const  T& div) const {
		vect_base<T, N> res;

		if (N > 0)
			if (abs(div) > (T)0)
			{
				T mult = (T)1 / div;
				for (size_t row = 0; row < N; row++)
					res[row] = (*this)[row] * mult;
			}

		return res;
	};

	inline vect_base& operator /= (const  T& div) {
		vect_base<T, N> res;

		if (N > 0)
			if (abs(div) > (T)0)
			{
				T mult = (T)1 / div;
				for (size_t row = 0; row < N; row++)
					(*this)[row] *= mult;
			}

		return *this;
	};

	inline vect_base& operator = (const T& mult) {

		if (N > 0)
			for (size_t row = 0; row < N; row++)
				(*this)[row] = mult;

		return (*this);
	}

	inline vect_base& operator *=(const T& mult) {

		if (N > 0)
			for (size_t row = 0; row < N; row++)
				(*this)[row] *= mult;

		return (*this);
	};

	inline vect_base& operator +=(const vect_base& rhs) {

		if (N > 0)
			for (size_t row = 0; row < N; row++)
				(*this)[row] += rhs[row];
		return *this;
	}
	inline vect_base& operator -=(const vect_base& rhs) {

		if (N > 0)
			for (size_t row = 0; row < N; row++)
				(*this)[row] -= rhs[row];
		return *this;
	}


	inline vect_base  normalize() const;
	inline void normalize();
	inline static void normalize(vect_base&);
};


template<typename T, size_t N>
inline  vect_base<T, N> vect_base<T, N >::vector_product(const vect_base<T, N>& lhs, const vect_base<T, N >& rhs) {
	if (N == 3)
	{
		vect_base<T, N> res;
		res.set(0, lhs[1] * rhs[2] - lhs[2] * rhs[1]);
		res.set(1, lhs[2] * rhs[0] - lhs[0] * rhs[2]);
		res.set(2, lhs[0] * rhs[1] - lhs[1] * rhs[0]);
		return res;
	}
	else throw std::length_error("vector_product is implemented only for 3-dimensional vectors.");
}

template<typename T, size_t N>
inline vect_base<T, N> vect_base<T, N >::vector_product(const vect_base<T, N >& rhs) const {
	return vector_product(*this, rhs);
}


template<typename T, size_t N>
inline void vect_base<T, N >::set(size_t idx, const T& value)
{
	if (idx > N - 1) throw std::out_of_range("invalid index");

	(*this)[idx] = value;
}

template<typename T, size_t N>
inline void vect_base<T, N >::set(const vect_base<T, N>& v)
{
	*this = v;
}

template<typename T, size_t N>
T    vect_base<T, N >::get(size_t idx) const
{
	return (*this)[idx];
}
template<typename T, size_t N>
void vect_base<T, N >::get(std::array<T, N>& out) const
{
	out = *this;
}

template<typename T, size_t N>
const std::array<T, N>& vect_base<T, N >::get() const
{
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
inline vect_base<T, N > vect_base<T, N >::operator* (const matrix_base<T, N >& m) const {
	vect_base <T, N> res = (T)0;
	std::array<std::array<T, N>, N> matr = m();
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			res[row] += (*this)[col] * matr[col][row];

	return res;
}

template<typename T, size_t N>
inline T    vect_base<T, N >::norm() const
{
	const vect_base<T, N >& v = (*this);
	return sqrt(v * v);
}

template<typename T, size_t N>
inline void vect_base<T, N>::normalize(vect_base<T, N>& v)
{
	T length = v.norm();
	if (length > (T)0)
		v *= (T)1 / length;
	else
		throw std::invalid_argument("length of vector is zero");
}


template<typename T, size_t N>
inline vect_base<T, N > vect_base<T, N>::normalize() const
{
	vect_base<T, N> res(*this);
	normalize(res);
	return res;
}

template<typename T, size_t N>
inline void vect_base<T, N >::normalize()
{
	normalize(*this);
}
