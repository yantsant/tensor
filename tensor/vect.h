#pragma once

#include <iostream>
#include <array>


template<typename T, size_t N>
class arr : public std::array<T, N>
{
public:
	explicit arr() {};// { *this = (T)0; };
	arr(T val) { *this = val; };

	void set(size_t idx, const T& value);
	void set(const arr&);
	T    get(size_t idx) const;
	void get(arr&) const;
	arr  get() const;

	inline std::array<T, N>& operator()() { return *this; };
	//inline T     operator ()(size_t idx) const;
	inline arr  operator + (const arr& m) const;
	inline arr  operator - (const arr& m) const;
	inline arr  operator * (const  T& val) const;

	//vect& operator = (const vect& val);
	inline arr& operator = (const T& val);
	inline arr& operator *=(const T&);
	inline arr& operator +=(const arr& m);
	inline arr& operator -=(const arr& m);


	T    norm();
	inline arr normalize() const;
	inline static void normalize(arr&);
	inline void normalize();
};


template<typename T, size_t N>
class vect : private arr<T, N>
{
public:
	explicit vect() {};// { *this = (T)0; };
	vect(T val) { *this = val; };

	void set(size_t idx, const T& value);
	void set(const vect&);
	T    get(size_t idx) const;
	void get(vect&) const;
	vect get() const;

	inline std::array<T, N>& operator()() { return *this; };
	//inline T     operator ()(size_t idx) const;
	inline vect  operator + (const vect& m) const;
	inline vect  operator - (const vect& m) const;
	inline T     operator * (const vect& m) const;
	inline vect  operator * (const  T& val) const;

	//vect& operator = (const vect& val);
	inline vect& operator = (const T& val);
	inline vect& operator *=(const T&);
	inline vect& operator +=(const vect& m);
	inline vect& operator -=(const vect& m);

	inline static vect vector_product(const vect& lhs, const vect& rhs);
	inline vect vector_product(const vect& rhs) const;

	T    norm();
	inline vect normalize() const;
	inline static void normalize(vect&);
	inline void normalize();
};

template<typename T, size_t N>
inline T    vect<T, N >::norm()
{
	const vect<T, N >& v = (*this);
	return sqrt(v * v);
}

template<typename T, size_t N>
inline void vect<T, N>::normalize(vect<T, N>& v)
{
	T length = v.norm();
	if (length > (T)0)
		v *= (T)1 / length;
	else
		throw std::invalid_argument("length of vector is zero");
}


template<typename T, size_t N>
inline vect<T, N > vect<T, N>::normalize() const
{
	vect<T, N> res(*this);
	normalize(res);
	return res;
}

template<typename T, size_t N>
inline void vect<T, N >::normalize()
{
	normalize(*this);
}


template<typename T, std::size_t N>
std::ostream& operator<<(std::ostream& out, const std::array<T, N>& a)
{
	for (auto row : a)
	{
		out << row << " ";
	}
	out << "\n";
	return out;
}

//template<typename T, size_t N>
//inline T vect<T, N >:: operator () (size_t idx) const {
//	if (idx > N-1)
//		throw std::out_of_range("invalid index []");
//
//	return (*this)[idx];
//}


template<typename T, size_t N>
inline void vect<T, N >::set(size_t idx, const T& value)
{
	if (idx > N - 1) throw std::out_of_range("invalid index");

	(*this)[idx] = value;
}

template<typename T, size_t N>
inline void vect<T, N >::set(const vect<T, N> &v)
{
	*this = v;
}


template<typename T, size_t N>
T    vect<T, N >::get(size_t idx) const
{
	return (*this)[idx];
}
template<typename T, size_t N>
void vect<T, N >::get(vect& out) const
{
	out = *this;
}
template<typename T, size_t N>
vect<T, N> vect<T, N >::get() const
{
	return *this;
}


//template<typename T, size_t N>
//inline T vect<T, N >:: operator () (size_t idx) const {
//	if (idx > N - 1) throw std::out_of_range("invalid index ()");
//
//	return (*this)[idx];
//}

template<typename T, size_t N>
inline vect<T, N >& vect<T, N >:: operator = (const T& val) {
	if (N == 0)
		throw std::length_error("zero length vector!");

	for (size_t row = 0; row < N; row++)
		(*this)[row] = val;

	return *this;
}

//template<typename T, size_t N>
//inline vect<T, N >& vect<T, N >:: operator = (const vect<T, N >& rhs) {
//
//	*this = rhs;
//
//	return *this;
//}

template<typename T, size_t N>
inline vect<T, N> vect<T, N >::operator+ (const vect<T, N >& v) const
{
	if (N == 0) throw std::length_error("zero length vector!");

	vect<T, N> res;
	for (size_t row = 0; row < N; row++)
			res[row] = (*this)[row] + v[row];
	return res;
}

template<typename T, size_t N>
inline vect<T, N> vect<T, N >::operator- (const vect<T, N >& v) const {
	if (N == 0) throw std::length_error("zero length vector!");

	vect<T, N> res;
	for (size_t row = 0; row < N; row++)
		res[row] = (*this)[row] - v[row];
	return res;
}

template<typename T, size_t N>
inline T vect<T, N >::operator* (const vect<T, N >& v) const {
	if (N == 0) throw std::length_error("zero length vector!");

	T res = (T)0;
	for (size_t row = 0; row < N; row++)
		res += (*this)[row] * v[row];

	return res;
}

template<typename T, size_t N>
inline vect<T, N > vect<T, N >::operator* (const T& mult) const {
	if (N == 0) throw std::length_error("zero length vector!");

	vect<T, N> res;
	for (size_t row = 0; row < N; row++)
		res[row] = (*this)[row] * mult;

	return res;
}

template<typename T, size_t N>
inline vect<T, N >& vect<T, N >::operator*= (const T& mult)  {
	if (N == 0) throw std::length_error("zero length vector!");

	for (size_t row = 0; row < N; row++)
		(*this)[row] *= mult;

	return *this;
}

template<typename T, size_t N>
inline vect<T, N >& vect<T, N >:: operator+= ( const vect<T, N >& rhs) {
	if (N == 0) throw std::length_error("zero length vector!");

	for (size_t row = 0; row < N; row++)
		(*this)[row] += rhs[row];

	return *this;
}

template<typename T, size_t N>
inline vect<T, N >& vect<T, N >:: operator-= (const vect<T, N >& rhs) {
	if (N == 0)
		throw std::length_error("zero length vector!");

	for (size_t row = 0; row < N; row++)
		(*this)[row] -= rhs[row];

	return *this;
}


template<typename T, size_t N>
inline vect<T, N> vect<T, N >::vector_product (const vect<T, N>& lhs, const vect<T, N >& rhs) {
	if (N == 0) throw std::length_error("zero length vector!");
	if (N != 3) throw std::length_error("vector_product is implemented only for 3-dimensional vectors.");

	vect<T, N> res;

	res[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
	res[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
	res[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];

	return res;
}


template<typename T, size_t N>
inline vect<T, N> vect<T, N >::vector_product(const vect<T, N >& rhs) const {

	const vect<T, N>& lhs = *this;
	vect<T, N> res = vector_product(lhs, rhs);

	return res;
}