#pragma once
#include <random>
#include <iostream>
enum class MATRIXINITTYPE{
	NOINIT,
	ZERO,
	INDENT,
};

enum class TRANSPOSE
{
	TRUE,
	FALSE
};
template<typename T> bool is_small_value(T value);

extern std::random_device rd;  // Will be used to obtain a seed for the random number engine
extern std::mt19937 gen;       // Standard mersenne_twister_engine seeded with rd()
extern std::uniform_real_distribution<double> unidistr;

extern const size_t DIM;

template<typename T, std::size_t N> class matrix_base;
template<typename T, std::size_t N> std::ostream& operator<< (std::ostream& o, const matrix_base<T,N>& m);
template<typename T, std::size_t N> matrix_base<T, N> generate_rand_ort();

namespace matrix_generator
{
	template<typename T, std::size_t N>
	matrix_base<T, N> generate_rand_ort();

	template<typename T, std::size_t N>
	matrix_base<T, N> generate_rand() {
		std::array<std::array<T, N>, N> a;
		for (size_t row = 0; row < N; row++)
			for (size_t col = 0; col < N; col++)
				a[row][col] = static_cast<T>(unidistr(gen));
		return matrix_base<T, N>(a);
	};
};



template<typename T, std::size_t N>
class matrix_moveable
{
	std::array<std::array<T, N>, N>*  _array;
public:
	matrix() {
		_array = new std::array<std::array<T, N>, N>;
		for (auto& row : *_array)
			for (auto& col : row)
				col = (T)3.3;
	}
	std::array<T, N>& operator [](size_t i) { return (*_array)[i]; };
};

template<typename T, std::size_t N>
class matrix_base : private std::array<std::array<T, N>, N>
{
	void set_zero();

	typedef  std::array<std::array<T, N>, N>  _array;
protected:
public:
	matrix_base(MATRIXINITTYPE IT);
	matrix_base(const matrix_base&  m) : _array(static_cast<const _array& >(m)){};
	matrix_base(const matrix_base&& m) : _array(static_cast<const _array&&>(m)) {};

	matrix_base(const std::array<std::array<T, N>, N>& a);
	inline const _array& operator()() const { return *this; };


	friend std::ostream& operator<< <>(std::ostream& out, const matrix_base& a);
	matrix_base& operator = (const T& vl);
	inline  matrix_base& operator = (const matrix_base& rhs);
	virtual matrix_base  operator + (const matrix_base& m) const;
	virtual matrix_base  operator - (const matrix_base& m) const;
	virtual matrix_base  operator * (const matrix_base& m) const;
	virtual matrix_base  operator * (const T& val) const;
	virtual matrix_base& operator +=(const matrix_base& m);
	virtual matrix_base& operator -=(const matrix_base& m);
	virtual matrix_base& operator *=(const matrix_base& m);
	virtual matrix_base& operator *=(const T& val);
	matrix_base& move(matrix_base&& rhs) {
		static_cast<_array&>(*this) = std::move((rhs));
		return *this;
	};

	matrix_base& operator = (matrix_base&& m) noexcept {
		*this = matrix_base::move(static_cast<matrix_base&&>(m));
		return *this;
	};

	bool        check_ort() const;
	matrix_base transpose() const;
	matrix_base scal(TRANSPOSE left, const matrix_base& rhs, TRANSPOSE right) const;
	matrix_base transform(TRANSPOSE left, const matrix_base& op, TRANSPOSE right) const;
	virtual T    convolution(const matrix_base<T, N>& rhs) const;

	friend void transpose(matrix_base& m)	{
		for (size_t row = 0; row < N; row++)
			for (size_t col = row + 1; col < N; col++)
				std::swap(m[row][col], m[col][row]);
	};
};

template<typename T, std::size_t N>
matrix_base<T, N>::matrix_base(const std::array<std::array<T, N>, N>& a) {
	static_cast<std::array<std::array<T, N>, N>&>(*this) = a;
}

template<typename T, std::size_t N>
void matrix_base<T, N>::set_zero() {
	for (auto& row : *this) row.fill((T)0);
}

template<typename T, std::size_t N>
matrix_base<T, N>::matrix_base(MATRIXINITTYPE IT) {
	switch (IT)
	{
	case MATRIXINITTYPE::NOINIT:		return;
	case MATRIXINITTYPE::ZERO  :		set_zero();
		return;
	case MATRIXINITTYPE::INDENT:
		set_zero();
		for (size_t row = 0; row < N; row++) 
			(*this)[row][row] = (T)1; 
		return;
	default:                     		set_zero();
		return;
	}
	return;
}

template<typename T, std::size_t N>
matrix_base<T, N> matrix_base<T, N>::transpose() const {
	matrix_base<T, N> res(*this);
	for (size_t row = 0; row < N; row++)
		for (size_t col = row + 1; col < N; col++)
			std::swap(res[row][col], res[col][row]);
	return res;
}

template<typename T, size_t N>
bool matrix_base<T, N >::check_ort() const {
	matrix_base<T, N> m = *this * this->transpose();
	T diag = 0;
	T nondiag = 0;
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			(row == col) ? diag += m[row][row] : nondiag += m[row][col];
	return (is_small_value<T>(abs(diag - (T)N) + abs(nondiag)) ? true : false);
}

// this^?l . rhs^?r
template<typename T, std::size_t N>
matrix_base<T, N> matrix_base<T, N>::scal(TRANSPOSE left, const matrix_base& rhs, TRANSPOSE right) const {
	if (left == right)	{
		if (left == TRANSPOSE::FALSE)	return (*this) * rhs;
		else		                	return (rhs * (*this)).transpose();
	} else	{
		if (left == TRANSPOSE::FALSE)	return (*this) * rhs.transpose();
		else                            return transpose() * rhs;
	}
}

//  op^?l . this . op^?r
template<typename T, std::size_t N>
matrix_base<T, N> matrix_base<T, N>::transform(TRANSPOSE left, const matrix_base& op, TRANSPOSE right) const {
	matrix_base<T, N> opt = op.transpose();
	if (left == right)	{
		if (left == TRANSPOSE::FALSE)	return op * (*this) * op;
		else			                return opt * (*this) * opt;
	} else	{
		if (left == TRANSPOSE::FALSE)	return op * (*this) * opt;
		else                			return opt * (*this) * op;
	}
}

template<typename T, std::size_t N>
matrix_base<T, N>& matrix_base<T, N>::operator = (const matrix_base<T, N>& rhs) {
	static_cast<std::array<std::array<T, N>, N>&>(*this) = static_cast<const std::array<std::array<T, N>, N>&>(rhs);
	return *this;
}

template<typename T, std::size_t N>
matrix_base<T, N>& matrix_base<T, N>::operator = (const T& vl) {
	T value = (T)vl;
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			(*this)[row][col] = value;
	return (*this);
};

template<typename T, std::size_t N>
matrix_base<T, N> matrix_base<T, N>::operator + (const matrix_base<T, N>& rhs) const {
	matrix_base<T, N> nhs(MATRIXINITTYPE::NOINIT);
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			nhs[row][col] = (*this)[row][col] + rhs[row][col];
	return nhs;
}

template<typename T, std::size_t N>
matrix_base<T, N> matrix_base<T, N>::operator - (const matrix_base<T, N>& rhs) const {
	matrix_base<T, N> nhs(MATRIXINITTYPE::NOINIT);
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			nhs[row][col] = (*this)[row][col] - rhs[row][col];
	return nhs;
}

template<typename T, std::size_t N>
matrix_base<T, N>& matrix_base<T, N>::operator += (const matrix_base<T, N>& rhs) {
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			(*this)[row][col] += rhs[row][col];
	return *this;
}

template<typename T, std::size_t N>
matrix_base<T, N>& matrix_base<T, N>::operator -= (const matrix_base<T, N>& rhs) {
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			(*this)[row][col] -= rhs[row][col];
	return *this;
}

template<typename T, std::size_t N>
matrix_base<T, N> matrix_base<T, N>::operator * (const T& val) const {
	matrix_base<T, N> nhs(MATRIXINITTYPE::NOINIT);
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			nhs[row][col] = (*this)[row][col] * val;
	return nhs;
}

template<typename T, std::size_t N>
matrix_base<T, N> matrix_base<T, N>::operator * (const matrix_base<T, N>& rhs) const {
	matrix_base<T, N> nhs(MATRIXINITTYPE::ZERO);
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			for (size_t i = 0; i < N; i++)
				nhs[row][col] += (*this)[row][i] * rhs[i][col];
	return nhs;
}

template<typename T, std::size_t N>
T   matrix_base<T, N>::convolution(const matrix_base<T, N>& rhs) const {
	T res = (T)0;
	const matrix_base<T, N>& lhs = *this;
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			res += lhs[row][col] * rhs[row][col];
	return res;
}

template<typename T, std::size_t N>
matrix_base<T, N>& matrix_base<T, N>::operator *=(const T& val)  {
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			(*this)[row][col] *= val;
	return *this;
}

template<typename T, std::size_t N>
matrix_base<T, N>& matrix_base<T, N>::operator *= (const matrix_base<T, N>& rhs) {
	matrix_base<T, N> nhs(MATRIXINITTYPE::ZERO);
	for (size_t row = 0; row < N; row++)
		for (size_t col = 0; col < N; col++)
			for (size_t i = 0; i < N; i++)
				nhs[row][col] += (*this)[row][i] * rhs[i][col];

	(*this) = nhs;
	return (*this);
}

template<typename T, std::size_t N>
std::ostream& operator<<(std::ostream& out, const matrix_base<T, N>& a) {
	for (auto row : a)
	{
		for (auto col : row)
			out << col << " ";
		out << "\n";
	}
	out << "\n";
	return out;
};
