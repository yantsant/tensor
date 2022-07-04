#include "tensor.h"


int main()
{
	//std::shared_ptr<Matrix<double, DIM>> basis (new Matrix<double, DIM>);
	Tensor<double> T1;

	Matrix<double, DIM> M1(MATRIXINITTYPE::INDENT);
	Tensor<double> T2(MATRIXINITTYPE::INDENT, T1.get_basis());

	T2 *= T1;
	Matrix<double, DIM> M2(2);
	Matrix<double, DIM> M3;
	M3 = M2 - M1;
	M1 += M2;
	M1 = M1 * M2;

	matr<double, DIM> c1 = { 0.0 }; std::cout << c1;
	matr<double, DIM> c2 = { 1.0 }; std::cout << c2;
	matr<double, DIM> c3 = c1 + c2; std::cout << c3;
	for (size_t i = 0; i < 1000000; i++)
	{
		c3 = c1 + c2;
	}
	c3 = std::move(c1);
	Matrix<double, DIM> m(c1);
	//matr c2 = { 0.0 };
	//matr c3 = c1 + c2;
	//Matrix m1(c1);
	return 0;
}