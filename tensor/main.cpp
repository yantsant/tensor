//#include "tensor.h"

#include "vect.h"
#include "tensor.h"
#include "quat.h"

const size_t DIM = 3;
std::random_device rd;  // Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
extern std::uniform_real_distribution<double> unidistr = std::uniform_real_distribution<double>((double)0, (double)1);

int main()
{
	vect_base<double, DIM> v0;
	v0.set(0, 1.0);
	v0.set(1, 1.5);
	v0.set(2, -0.5);
	matrix_base<double, DIM> dR = quat<double>(0.01, v0, QUATFORM::ANGLEAXIS).get_ort_matrix();
	matrix_base<double, DIM> m0 (MATRIXINITTYPE::INDENT);
	dR.scal(TRANSPOSE::TRUE, m0, TRANSPOSE::TRUE);
	double x = dR.convolution(m0);
	Tensor<double, DIM> t0(MATRIXINITTYPE::INDENT, m0);
	Tensor<double, DIM> t1(MATRIXINITTYPE::INDENT, m0);
	matrix_base<double, DIM> R(matrix_generator::generate_rand_ort<double, DIM>());
	matrix_base<double, DIM> Q(matrix_generator::generate_rand_ort<double, DIM>());
	matrix_base<double, DIM> A(matrix_generator::generate_rand    <double, DIM>());
	matrix_base<double, DIM> I(MATRIXINITTYPE::INDENT);
	Tensor<double, DIM> t2(A, R);

	double y = t0.convolution(t1);
	std::cout << t2.get_comp_at_basis(GLOBAL_DEFAULT_BASIS<double, 3>);
	t2.change_basis(R);
	std::cout << t2.get_comp_at_basis(GLOBAL_DEFAULT_BASIS<double, 3>);
	t2.change_basis(I);
	std::cout << t2.get_comp_at_basis(GLOBAL_DEFAULT_BASIS<double, 3>);
	t2.change_basis(R);
	std::cout << t2.get_comp_at_basis(GLOBAL_DEFAULT_BASIS<double, 3>);
	t0 = 2.0;
	t1 = 3.0;
	t2 += t1;
	t2 += t0;
//
	Vector<double, 3> v1(-1.0, GLOBAL_DEFAULT_BASIS<double, 3>);
	Vector<double, 3> v2( 1.0, GLOBAL_DEFAULT_BASIS<double, 3>);
	//auto M = v1.outer_product(v2); 
	//std::cout << v1 << v1.get_comp_at_basis(GLOBAL_DEFAULT_BASIS<double, 3>) << v1.norm();
	//v1.change_basis(R);
	//std::cout << v1 << v1.get_comp_at_basis(GLOBAL_DEFAULT_BASIS<double, 3>) << v1.norm();
	//v1.change_basis(GLOBAL_DEFAULT_BASIS<double, 3>);
	//std::cout << v1 << v1.get_comp_at_basis(GLOBAL_DEFAULT_BASIS<double, 3>) << v1.norm();
	//v1.change_basis(R.transpose());
	//std::cout << v1 << v1.get_comp_at_basis(GLOBAL_DEFAULT_BASIS<double, 3>) << v1.norm();
	auto ff = outer_product(v1, v2);
	v1 = v1+v2;

//	Vector<double, 3> v2( 2, GLOBAL_DEFAULT_BASIS<double, 3>);
//	Vector<double, 3> v3(GLOBAL_DEFAULT_BASIS<double, 3>);
//	v1 *= 1.4;
//	v1 * 0.5;
//	v1 = -v2;
//	std::cout << v1;
//	auto x = v1* v2;
//	v3 = (v1 - v2*0.5) * 0.3;
//	Vector<double, 3> v5 ( v1 + v2);
//	v5.normalize();
//	//v5 = v1 + v2 * 0.5;
//	v3 = v1;
//	v3 = v1.vector_product(v2);
//	quat<double> q0;
//	quat<double> q1(0.1, v1);
//	quat<double> q2(-0.3, v2);
//	quat<double> q3(0.3, v2);
//	quat<double> q4(q3);
//	q2 = *q1;
//	q0 = q1;
//	q0 = q2 / 0.1;
//	q0 /= 0.1;
//	q3 = !q0;
//	q2 = q0 + q1;
//	q3 = q0 * q1;
//	q4 = q1 * 1.1 - q2;
	//q3 += q0 * 1.5;
//	//Vector<double, 4> v4;
//	//q1 = v4;
//	v1 = v2 + v2;
//	q3 = q1;
//	q2 = q1 * 0.5;
//	q3 = q1 + q2;

	return 0;
}