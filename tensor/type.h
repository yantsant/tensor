
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
matr<T, N> operator+ (const matr<T, N>& lhs, const matr<T, N >& rhs) {
	if (N == 0) throw std::length_error("zero length matrices!");

	matr<T, N> nhs;
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			nhs[row][col] = lhs[row][col] + rhs[row][col];
	return nhs;// std::move(nhs);
}

template<typename T, std::size_t N>
matr<T, N> operator- (const matr<T, N>& lhs, const matr<T, N >& rhs) {
	if (N == 0) throw std::length_error("zero length matrices!");

	matr<T, N> nhs;
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			nhs[row][col] = lhs[row][col] - rhs[row][col];
	return nhs;// std::move(nhs);
}

template<typename T, std::size_t N>
matr<T, N> operator* (const matr<T, N>& lhs, const matr<T, N >& rhs) {
	if (N == 0) throw std::length_error("zero length matrices!");

	matr<T, N> nhs;
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
		{
			nhs[row][col] = (T)0;
			for (size_t i = 0; i < N; i++)
				nhs[row][col] += lhs[row][i] * rhs[i][col];
		}
	return nhs;// std::move(nhs);
}

template<typename T, std::size_t N>
void operator+= (matr<T, N>& lhs, const matr<T, N >& rhs) {
	if (N == 0) throw std::length_error("zero length matrices!");

	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			lhs[row][col] += rhs[row][col];
}

template<typename T, std::size_t N>
void operator-= (matr<T, N>& lhs, const matr<T, N >& rhs) {
	if (N == 0)
		throw std::length_error("zero length matrices!");

	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			lhs[row][col] -= rhs[row][col];
}