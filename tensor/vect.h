#pragma once

#include <iostream>
#include <array>

template<typename T, size_t N>
class arr : private std::array<T, N>
{
public:
	explicit arr() { *this = (T)0; };
	arr(T val) { *this = val; };

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

	inline std::array<T, N>& operator()() { return *this; };

	//inline T     operator ()(size_t idx) const { return (*this)[]; };
	inline arr  operator+ (const arr& rhs)	{
		arr<T, N> res;

		if (N > 0) 
			for (size_t row = 0; row < N; row++)
				res[row] = (*this)[row] + rhs[row];

		return res;
	};

	inline arr  operator- ( const arr& rhs)	{
		arr<T, N> res;

		if (N > 0)
		for (size_t row = 0; row < N; row++)
			res[row] = (*this)[row] - rhs[row];

		return res;
	};

	inline arr operator * ( const  T& mult) {
		arr<T, N> res;

		if (N > 0) 
			for (size_t row = 0; row < N; row++)
				res[row] = (*this)[row] * mult;

		return res;
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


	T    norm();
	inline arr normalize() const;
	inline static void normalize(arr&);
	inline void normalize();
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
inline T    arr<T, N >::norm()
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




template<typename T, size_t N>
class vect : public arr<T, N>
{
public:
	explicit vect() {};
	vect(T val) : arr<T, N>(val) {};
	inline arr<T, N>& operator()() { return *this; };
	inline T     operator * (const vect& m) const;
	inline static vect vector_product(const vect& lhs, const vect& rhs);
	inline vect vector_product(const vect& rhs) const;
};


template<typename T, size_t N>
inline T vect<T, N >::operator* (const vect<T, N >& v) const {
	if (N == 0) throw std::length_error("zero length vector!");

	T res = (T)0;
	for (size_t row = 0; row < N; row++)
		res += this->get(row) * v.get(row);

	return res;
}


template<typename T, size_t N>
inline vect<T, N> vect<T, N >::vector_product (const vect<T, N>& lhs, const vect<T, N >& rhs) {
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