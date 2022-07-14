//#include "tensor.h"

#include "vect.h"
//#include "matrix.h"
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
	Tensor<double, DIM> t0(MATRIXINITTYPE::INDENT, m0);
	Tensor<double, DIM> t1(MATRIXINITTYPE::INDENT, m0);
	matrix_base<double, DIM> R(matrix_generator::generate_rand_ort<double, DIM>());
	matrix_base<double, DIM> Q(matrix_generator::generate_rand_ort<double, DIM>());
	matrix_base<double, DIM> A(matrix_generator::generate_rand    <double, DIM>());
	matrix_base<double, DIM> I(MATRIXINITTYPE::INDENT);
	Tensor<double, DIM> t2(A, R);
	for (size_t i = 0; i < 100; i++)
	{
		R *= dR;
		t2.move_to_basis(R);
		std::cout << t2.comp_at_basis(I);
	}
	t2.move_to_basis(R);
	std::cout << t2.comp_at_basis(Q);
	std::cout << t2.comp_at_basis(I);
	t2.move_to_basis(I);
	std::cout << t2.comp_at_basis(Q);
	std::cout << t2.comp_at_basis(I);
	t2.move_to_basis(R);
	std::cout << t2.comp_at_basis(Q);
	std::cout << t2.comp_at_basis(I);
	t0 = 2.0;
	t1 = 3.0;
	t2 += t1;
	t2 += t0;
//
//	Vector<double, 3> v1(-1.0, GLOBAL_DEFAULT_BASIS<double, 3>);
//	t2* t1;
//	v1 = t1 * v1;
//	v1 = v1 * t1;
//	std::cout << v1;
//	//for (size_t row = 0; row < 1000000; row++)
//	{
//		matrix_base<double, DIM>* m0 = new matrix_base<double, DIM>(MATRIXINITTYPE::ZERO);
//		matrix_base<double, DIM>* m1 = new matrix_base<double, DIM>(MATRIXINITTYPE::INDENT);
//		matrix_base<double, DIM>* m2 = new matrix_base<double, DIM> (*m1);
//		transpose(*m1);
//		m1->transpose();
//		//std::cout << *m1;
//		*m2 = *m0;
//		*m2 = 1.0;
//		*m2 += *m1;
//		*m2 -= *m1;
//		*m1 *= 3.0;
//		matrix_base<double, DIM>* m3  = new matrix_base<double, DIM>((*m2) - (*m1) * 3);
//		*m3 = *m2 * *m2;
//		matrix_base<double, DIM> m4(MATRIXINITTYPE::INDENT);
//		m4 = m3->transform(TRANSPOSE::FALSE, *m2, TRANSPOSE::TRUE);
//		delete m0;
//		delete m1;
//		delete m2;
//		delete m3;
//	}
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