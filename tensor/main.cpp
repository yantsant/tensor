//#include "tensor.h"

#include "vect.h"
#include "tensor.h"
#include "quat.h"

const size_t DIM = 3;
std::random_device rd;  // Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
extern std::uniform_real_distribution<double> unidistr = std::uniform_real_distribution<double>((double)0, (double)1);


void quternion_unit_test()
{
	//Vector<double, 3> v2( 1.0, GLOBAL_DEFAULT_BASIS<double, 3>);
	//Vector<double, 3> v2( 2, GLOBAL_DEFAULT_BASIS<double, 3>);
	//Vector<double, 3> v3(GLOBAL_DEFAULT_BASIS<double, 3>);
	//v1 *= 1.4;
	//v1 * 0.5;
	//v1 = -v2;
	//std::cout << v1;
	//auto x = v1* v2;
	//v3 = (v1 - v2*0.5) * 0.3;
	//Vector<double, 3> v5 ( v1 + v2);
	//v5.normalize();
	////v5 = v1 + v2 * 0.5;
	//v3 = v1;
	//v3 = v1.vector_product(v2);
	//quat<double> q0;
	//quat<double> q1(0.1, v1);
	//quat<double> q2(-0.3, v2);
	//quat<double> q3(0.3, v2);
	//quat<double> q4(q3);
	//q2 = *q1;
	//q0 = q1;
	//q0 = q2 / 0.1;
	//q0 /= 0.1;
	//q3 = !q0;
	//q2 = q0 + q1;
	//q3 = q0 * q1;
	//q4 = q1 * 1.1 - q2;
	//q3 += q0 * 1.5;
	////Vector<double, 4> v4;
	////q1 = v4;
	//v1 = v2 + v2;
	//q3 = q1;
	//q2 = q1 * 0.5;
	//q3 = q1 + q2;
}
void matrix_unit_test()
{
	//matrix<double, 3> m;
	//std::array<double, 3>& a = m[0];
	//double el = m[0][1];
	//m[0][2] = 1;
	//return;
}
void tensor_unit_test()
{
	auto M1 = matrix_base<double, DIM>(MATRIXINITTYPE::INDENT);
	auto M2 = matrix_base<double, DIM>(MATRIXINITTYPE::INDENT);
	M1 = std::move(M2);
	std::vector<std::vector<double>> V1(10);
	std::vector<std::vector<double>> V2(20);
	V1 = std::move(V2);

	//q1.vector_product(q2);
	auto mm = matrix_base<double, DIM>(MATRIXINITTYPE::INDENT);
	shared_handler_basis<double, DIM> rr(mm);
	//typedef std::shared_ptr<> shared_handler;
	std::shared_ptr<double> xx = std::make_shared<double>();
	matrix_base<double, DIM>* R = new matrix_base<double, DIM>(matrix_generator::generate_rand_ort<double, DIM>());
	shared_handler_basis<double, DIM> MM0(*R);
	for (size_t i = 0; i < 10000000; i++)
	{
		matrix_base<double, DIM>* R = new matrix_base<double, DIM>(matrix_generator::generate_rand_ort<double, DIM>());
		matrix_base<double, DIM>* Q = new matrix_base<double, DIM>(matrix_generator::generate_rand_ort<double, DIM>());
		matrix_base<double, DIM>* P = new matrix_base<double, DIM>(matrix_generator::generate_rand_ort<double, DIM>());
		auto sm1 = new shared_handler_basis<double, DIM>(*R); 
		auto sm2 = new shared_handler_basis<double, DIM>(*Q); 
		auto sm3 = new shared_handler_basis<double, DIM>(*P); 
		Tensor<double, DIM>* t0 = new Tensor<double, DIM>(*Q, *sm1); bool ort = sm1->as_matrix().check_ort();
		Tensor<double, DIM>* t1 = new Tensor<double, DIM>(*P, *sm2);  ort = sm2->as_matrix().check_ort();
		delete R;
		delete Q;
		Tensor<double, DIM> t4(std::move(*t1)); delete t1;
		t1 = new Tensor<double, DIM>(*P, *sm2);
		delete P;

		Tensor<double, DIM> t3(*t0);
		*t0 =  t4;
		*t1 = t4 + *t0;
		t4 -= *t1;
		t4 *= *t0;
		t4 += *t0;
		t4.change_basis(*sm2);
		t1->change_basis(*sm3);
		delete sm1;
		delete sm2;
		delete sm3;
		delete t0;
		delete t1;
	}
}

void tens_vect_unit_test()
{
	matrix_base<double, DIM>* R = new matrix_base<double, DIM>(matrix_generator::generate_rand_ort<double, DIM>());
	auto sm1 = new  shared_handler_basis<double, DIM>(*R);

	vect_base<double, DIM> v0(0.0);
	v0[0] = 1.0;
	v0[1] = 1.5;
	v0[2] = -0.5;
	Vector<double, 3> V0(v0, *sm1);
	Vector<double, 3> V1(v0, *sm1);
	Vector<double, 3> V3 = V0 + V1;
	Tensor<double, DIM>* t0 = new Tensor<double, DIM>(*R, *sm1);
	Tensor<double, DIM>* t1 = new Tensor<double, DIM>(*R, *sm1);
	Tensor<double, DIM>  t2 = outer_product(V0, V3); std::cout << t2;
	Tensor<double, DIM>  t3 = outer_product(V3, V0); std::cout << t3;
}

void vector_unit_test()
{
	matrix_base<double, DIM>* R1 = new matrix_base<double, DIM>(matrix_generator::generate_rand_ort<double, DIM>());
	matrix_base<double, DIM>* R2 = new matrix_base<double, DIM>(matrix_generator::generate_rand_ort<double, DIM>());
	auto sm1 = new  shared_handler_basis<double, DIM>(*R1);
	auto sm2 = new  shared_handler_basis<double, DIM>(*R2);
	delete R1;
	delete R2;
	vect_base<double, DIM> v0(0.0);
	v0[0] = 1.0;
	v0[1] = 1.5;
	v0[2] = -0.5;
	Vector<double, 3> V0(v0, *sm1);
	Vector<double, 3> V1(v0, *sm2);
	{
		Vector<double, 3> V3 = V0 + V1;
		Vector<double, 3> V4 = V0 - V1;
		auto V5 = V0 * V1;
	}
	Vector<double, 3> V3 = std::move(V0);
	//V3 = std::move(V1);
	delete sm1;
	std::cout << V3 << " " << V1;
	//Vector<double, DIM>  V4 = vector_product(V3, V1);
	std::cout << vector_product(V3, V1);
	//vect_base<double, 40> VV(0.0);
	//VV.normalize();
	return;
}
int main()
{
	tens_vect_unit_test();
	vector_unit_test();
	tensor_unit_test();
	matrix_unit_test();

	return 0;
}