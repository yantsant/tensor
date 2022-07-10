
#include <vector>
#include <utility>
#include <array>
#include <iostream>


template <typename T, size_t N>
using matr = std::array<std::array<T, N>, N>;


template<typename T, std::size_t N>
inline matr<T, N> operator+ (const matr<T, N>& lhs, const matr<T, N >& rhs) {
	
}

template<typename T, std::size_t N>
inline matr<T, N> operator- (const matr<T, N>& lhs, const matr<T, N >& rhs) {
}

template<typename T, std::size_t N>
inline matr<T, N> operator* (const matr<T, N>& lhs, const matr<T, N >& rhs) {

}

template<typename T, std::size_t N>
inline void operator+= (matr<T, N>& lhs, const matr<T, N >& rhs) {
}

template<typename T, std::size_t N>
inline void operator-= (matr<T, N>& lhs, const matr<T, N >& rhs) {
}


template<typename T, std::size_t N>
inline void operator*= (matr<T, N>& lhs, const matr<T, N >& rhs) {
	if (N == 0) throw std::length_error("zero length matrices!");

}

namespace matrix_operator
{

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
	template<typename T, std::size_t N>
	inline void dotdott(const matr<T, N>& lhs, const matr<T, N>& rhs, matr<T, N>& res)
	{
		res = lhs * rhs;
		res *= t(lhs);
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
	template<typename T, std::size_t N>
	inline void tdotdot(const matr<T, N>& lhs, const matr<T, N>& rhs, matr<T, N>& res)
	{
		res = t(lhs) * rhs;
		res *= lhs;
	}
}