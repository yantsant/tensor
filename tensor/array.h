#pragma once
#include <array>
#include "matrix.h"

template<typename T, size_t N>
class arr : private std::array<T, N>
{
public:
	explicit arr() { *this = (T)0; };
	arr(T val) { *this = val; };
	arr(std::array<T, N>& _arr) {(*this) = _arr;};

	void set(size_t idx, const T& value);
	void set(const arr&);
	T    get(size_t idx) const;
	void get(std::array<T, N>&) const;
	const std::array<T, N>& get() const;

	friend std::ostream& operator<<(std::ostream& out, const arr& a)
	{
		for (auto row : a)
		{
			out << row << " ";
		}
		out << "\n";
		return out;
	}

	virtual inline T    operator * (const arr& v) const;
	virtual inline arr  operator * (const matrix_base<T,N>& m) const;
	virtual inline T    norm() const;

	inline std::array<T, N>& operator()() { return *this; };

	inline arr  operator- () const {
		arr<T, N> res;

		if (N > 0)
			for (size_t row = 0; row < N; row++)
				res[row] = -(*this)[row];

		return res;
	};

	inline arr  operator+ (const arr& rhs) const {
		arr<T, N> res;

		if (N > 0)
			for (size_t row = 0; row < N; row++)
				res[row] = (*this)[row] + rhs[row];

		return res;
	};


	inline arr  operator- (const arr& rhs) const {
		arr<T, N> res;

		if (N > 0)
			for (size_t row = 0; row < N; row++)
				res[row] = (*this)[row] - rhs[row];

		return res;
	};

	inline arr operator * (const  T& mult) const {
		arr<T, N> res;

		if (N > 0)
			for (size_t row = 0; row < N; row++)
				res[row] = (*this)[row] * mult;

		return res;
	};

	inline arr operator / (const  T& div) const {
		arr<T, N> res;

		if (N > 0)
			if (abs(div) > (T)0)
			{
				T mult = (T)1 / div;
				for (size_t row = 0; row < N; row++)
					res[row] = (*this)[row] * mult;
			}

		return res;
	};

	inline arr& operator /= (const  T& div) {
		arr<T, N> res;

		if (N > 0)
			if (abs(div) > (T)0)
			{
				T mult = (T)1 / div;
				for (size_t row = 0; row < N; row++)
					(*this)[row] *= mult;
			}

		return *this;
	};

	inline arr& operator = (const T& mult) {

		if (N > 0)
			for (size_t row = 0; row < N; row++)
				(*this)[row] = mult;

		return (*this);
	}

	inline arr& operator *=(const T& mult) {

		if (N > 0)
			for (size_t row = 0; row < N; row++)
				(*this)[row] *= mult;

		return (*this);
	};

	inline arr& operator +=(const arr& rhs) {

		if (N > 0)
			for (size_t row = 0; row < N; row++)
				(*this)[row] += rhs[row];
		return *this;
	}
	inline arr& operator -=(const arr& rhs) {

		if (N > 0)
			for (size_t row = 0; row < N; row++)
				(*this)[row] -= rhs[row];
		return *this;
	}


	inline arr  normalize() const;
	inline void normalize();
	inline static void normalize(arr&);
};


template<typename T, size_t N>
inline void arr<T, N >::set(size_t idx, const T& value)
{
	if (idx > N - 1) throw std::out_of_range("invalid index");

	(*this)[idx] = value;
}

template<typename T, size_t N>
inline void arr<T, N >::set(const arr<T, N>& v)
{
	*this = v;
}

template<typename T, size_t N>
T    arr<T, N >::get(size_t idx) const
{
	return (*this)[idx];
}
template<typename T, size_t N>
void arr<T, N >::get(std::array<T, N>& out) const
{
	out = *this;
}

template<typename T, size_t N>
const std::array<T, N>& arr<T, N >::get() const
{
	return *this;
}

template<typename T, size_t N>
inline T arr<T, N >::operator* (const arr<T, N >& v) const {
	if (N == 0) throw std::length_error("zero length vector!");

	T res = (T)0;
	for (size_t row = 0; row < N; row++)
		res += (*this)[row] * v[row];

	return res;
}


template<typename T, size_t N>
inline arr<T, N > arr<T, N >::operator* (const matrix_base<T, N >& m) const {
	arr <T, N> res = (T)0;
	std::array<std::array<T, N>, N> matr = m();
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			res[row] += (*this)[col] * matr[col][row];

	return res;
}

template<typename T, size_t N>
inline T    arr<T, N >::norm() const
{
	const arr<T, N >& v = (*this);
	return sqrt(v * v);
}

template<typename T, size_t N>
inline void arr<T, N>::normalize(arr<T, N>& v)
{
	T length = v.norm();
	if (length > (T)0)
		v *= (T)1 / length;
	else
		throw std::invalid_argument("length of vector is zero");
}


template<typename T, size_t N>
inline arr<T, N > arr<T, N>::normalize() const
{
	arr<T, N> res(*this);
	normalize(res);
	return res;
}

template<typename T, size_t N>
inline void arr<T, N >::normalize()
{
	normalize(*this);
}
