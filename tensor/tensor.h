#pragma once
#include "matrix.h"


template<typename T>
class Tensor
{
public:
//	Tensor(double** componets, double** ort_basis); 
//	Tensor(double** componets); 
//	void MultToScal(double a);
//	void AddTensor(Tensor* T);
//	void SubTensor(Tensor* T);
//	void ScalarMultiplication(Tensor* T, Tensor* T_res);
//	void CalcCompTensorInBasis(Tensor* T, double** R);
//	void ToEigenSpace();
//	void RotateBasis(double** R, bool transp);
//	void GoToBasis(double** basis);
//	void Sqrt(bool save_basis);
//	void Sqr (bool save_basis);
//	void Exp (bool save_basis);
//	void Log (bool save_basis);
//	~Tensor(void);
private:
	std::pair<Matrix<T,DIM>, Matrix<T, DIM>>& body;
	//matr* c; // компоненты
	//matr* e; // базис, можно указывать ссылку на базис

};