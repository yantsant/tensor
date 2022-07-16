#include "vectbase.h"
#include "vect.h"
#include "quat.h"
#include "matrix.h"
#include "tensor.h"

template <> bool is_small_value(long double value) { return (abs(value) < 1e-13) ? true : false;}
template <> bool is_small_value(double      value) { return (abs(value) < 1e-13) ? true : false;}
template <> bool is_small_value(float       value) { return (abs(value) < 1e-7f) ? true : false;}
template <> bool is_small_value(int         value) { return (abs(value) == 0) ? true : false; }

template matrix_base<double, 3> matrix_generator::generate_rand_ort();

template<typename T, std::size_t N>
matrix_base<T, N> matrix_generator::generate_rand_ort() {
	vect_base<T, 4> qvalue;
	for (size_t row = 0; row < 4; row++) qvalue.set(row, static_cast<T>(unidistr(gen)));
	quat<T> q(qvalue);
	return static_cast<matrix_base<T, 3>>(q.get_ort_matrix());
}

template matrix_base<double, 3> matrix_generator::generate_rand_ort();