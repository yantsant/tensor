
#include <vector>
#include <utility>
#include <array>
#include <iostream>

const char DIM = 3;

template <typename T, size_t N>
using matr = std::array<std::array<T, N>, N>;

template<typename T, std::size_t N>
std::ostream& operator<<(std::ostream& out, const matr<T, N>& a)
{
	for (auto row : a)
	{
		for (auto col : row)
			out << col << " ";
		out << "\n";
	}
	out << "\n";
	return out;
}


template<typename T, std::size_t N>
inline matr<T, N> operator+ (const matr<T, N>& lhs, const matr<T, N >& rhs) {
	if (N == 0) throw std::length_error("zero length matrices!");

	matr<T, N> nhs;
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			nhs[row][col] = lhs[row][col] + rhs[row][col];
	return nhs;
}

template<typename T, std::size_t N>
inline matr<T, N> operator- (const matr<T, N>& lhs, const matr<T, N >& rhs) {
	if (N == 0) throw std::length_error("zero length matrices!");

	matr<T, N> nhs;
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			nhs[row][col] = lhs[row][col] - rhs[row][col];
	return nhs;
}

template<typename T, std::size_t N>
inline matr<T, N> operator* (const matr<T, N>& lhs, const matr<T, N >& rhs) {
	if (N == 0) throw std::length_error("zero length matrices!");

	matr<T, N> nhs;
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
		{
			nhs[row][col] = (T)0;
			for (size_t i = 0; i < N; i++)
				nhs[row][col] += lhs[row][i] * rhs[i][col];
		}
	return nhs;
}

template<typename T, std::size_t N>
inline void operator+= (matr<T, N>& lhs, const matr<T, N >& rhs) {
	if (N == 0) throw std::length_error("zero length matrices!");

	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			lhs[row][col] += rhs[row][col];
}

template<typename T, std::size_t N>
inline void operator-= (matr<T, N>& lhs, const matr<T, N >& rhs) {
	if (N == 0)
		throw std::length_error("zero length matrices!");

	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			lhs[row][col] -= rhs[row][col];
}


template<typename T, std::size_t N>
inline void operator*= (matr<T, N>& lhs, const matr<T, N >& rhs) {
	if (N == 0) throw std::length_error("zero length matrices!");

	matr<T, N> nhs;
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
		{
			nhs[row][col] = (T)0;
			for (size_t i = 0; i < N; i++)
				nhs[row][col] += lhs[row][i] * rhs[i][col];
		}
	lhs = nhs;
}

namespace matrix_operator
{
	//  m^T
	template<typename type, std::size_t N>
	inline void t(matr<type, N>& m)
	{
		for (size_t row = 0; row < N; row++)
			for (size_t col = row + 1; col < N; col++)
				std::swap(m[row][col], m[col][row]);
	}

	// return m^T
	template<typename type, std::size_t N>
	inline matr<type, N> t(const matr<type, N>& m)
	{
		matr<type, N> newm(m);
		T(newm);
		return newm;
	}


	// lhs . rhs^T
	template<typename T, std::size_t N>
	inline void dott(const matr<T, N>& lhs, const matr<T, N>& rhs, matr<T, N>& res)
	{
		for (size_t row = 0; row < N; row++)
			for (size_t col = 0; col < N; col++)
			{
				res[row][col] = (T)0;
				for (size_t i = 0; i < N; i++)
					res[row][col] += lhs[row][i] * rhs[col][i];
			}
	}


	// lhs^T . rhs
	template<typename T, std::size_t N>
	inline void tdot(const matr<T, N>& lhs, const matr<T, N>& rhs, matr<T, N>& res)
	{
		for (size_t row = 0; row < N; row++)
			for (size_t col = 0; col < N; col++)
			{
				res[row][col] = (T)0;
				for (size_t i = 0; i < N; i++)
					res[row][col] += lhs[i][row] * rhs[i][col];
			}
	}


	// lhs^T . rhs
	template<typename T, std::size_t N>
	inline matr<T, N> dott(const matr<T, N>& lhs, const matr<T, N>& rhs)
	{
		matr<T, N> res;

		dott(lhs, rhs, res);
	}

	// lhs^T . rhs
	template<typename T, std::size_t N>
	inline matr<T, N> tdot(const matr<T, N>& lhs, const matr<T, N>& rhs)
	{
		matr<T, N> res;

		tdot(lhs, rhs, res);
	}

	// lhs . rhs . lhs^T
	template<typename T, std::size_t N>
	inline matr<T, N> dotdott(const matr<T, N>& lhs, const matr<T, N>& rhs)
	{
		matr<T, N> res;

		res = lhs * rhs;
		res *= t(lhs);

		return res;
	}

	// lhs^T . rhs . lhs
	template<typename T, std::size_t N>
	inline matr<T, N> tdotdot(const matr<T, N>& lhs, const matr<T, N>& rhs)
	{
		matr<T, N> res;

		res = t(lhs) * rhs;
		res *= lhs;

		return res;
	}
}