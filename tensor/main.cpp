//#include "tensor.h"

#include "vect.h"
#include "quat.h"
int main()
{
	vect<double, 3> v1 = -1;
	vect<double, 3> v2 = 2;
	vect<double, 3> v3;
	v1 *= 1.4;
	std::cout << v1;
	//v3 = v1 + v2;
	v3 = v1 + v2*0.5;
	v3 = v1;
	v1.vector_product(v2);
	quat<double> q0;
	quat<double> q1(0.1, v1);
	quat<double> q2(-0.3, v2);
	quat<double> q3(0.3, v2);
	quat<double> q4(q3);
	q2 = *q1;
	q0 = q1;
	q0 = q2 / 0.1;
	q0 /= 0.1;
	q3 = !q0;
	q2 = q0 + q1;
	q3 = q0 * q1;
	q4 = q1 * 1.1 - q2;
	//q3 += q0 * 1.5;
//	//vect<double, 4> v4;
//	//q1 = v4;
//	v1 = v2 + v2;
//	q3 = q1;
//	q2 = q1 * 0.5;
//	q3 = q1 + q2;
//	//auto v4 = q.quat_as_4_vector();
//
//	vect<double, 3> v3 = 0;
//	double value = v1.get(1);
//	v3 = v1 + v2;
//	v2 = v1;
	//std::cout << v;
//	std::array<double, 3> x;
//	std::array<double, 3> y;
//	//x*=2.0;
//	//std::mt19937 mn(rd());
//	//double x = std::uniform_real_distribution<double>((double)0, (double)1)(mn);
//	//double x = get_uniform_value<double>;
//	//unidistr<double>(random_engine);
//	//std::shared_ptr<Matrix<double, DIM>> basis (new Matrix<double, DIM>);
//	Tensor<double> T1(MATRIXINITTYPE::INDENT);
//	Tensor<double> T2(MATRIXINITTYPE::ZERO);// (T1, MATRIXINITTYPE::INDENT);
//
//	T2 = T1;
//	T2 = T2 + T1;
//
//	Matrix<double, DIM> M1(MATRIXINITTYPE::INDENT);
//	Matrix<double, DIM> M2(2);
//	Matrix<double, DIM> M3;
//	M3 = M2 - M1;
//	M1 += M2;
//	M1 = M1 * M2;
//
//	matr<double, DIM> c1 = { 0.0 }; std::cout << c1;
//	matr<double, DIM> c2 = { 1.0 }; std::cout << c2;
//	matr<double, DIM> c3 = c1 + c2; std::cout << c3;
//	for (size_t i = 0; i < 1000000; i++)
//	{
//		c3 = c1 + c2;
//	}
//	c3 = std::move(c1);
//	Matrix<double, DIM> m(c1);
//	//matr c2 = { 0.0 };
//	//matr c3 = c1 + c2;
//	//Matrix m1(c1);
	return 0;
}