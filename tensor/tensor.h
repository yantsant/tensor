#pragma once
#include "matrix.h"


//template <typename T>
//using shrptr = std::shared_ptr<const Matrix<T, DIM>>;

template<typename T>
class Tensor : public Matrix<T, DIM>
{
	std::shared_ptr<const Matrix<T, DIM>> basis;
	bool BASISOWNER;
public:
	~Tensor() {
		basis.reset();
	}

	std::shared_ptr<const Matrix<T, DIM>> get_basis() { return basis; };

	Matrix<T, DIM>& get_component_basis(const std::shared_ptr<const Matrix<T, DIM>>& _basis) const;

	Tensor(MATRIXINITTYPE IT = MATRIXINITTYPE::ZERO) : Matrix<T, DIM>(IT)
	{
		BASISOWNER = true;
		basis = std::shared_ptr<const Matrix<T, DIM>>(new Matrix<double, DIM>(MATRIXINITTYPE::INDENT));
	}

	Tensor(MATRIXINITTYPE IT, const std::shared_ptr<const Matrix<T, DIM>>& _basis) : Matrix<T, DIM>(IT)
	{
		BASISOWNER = false;
		basis = _basis;
		//const Matrix<double, DIM>* bs = basis.get();
	}
};


template<typename T>
Matrix<T, DIM>& Tensor<T>::get_component_basis(const std::shared_ptr<const Matrix<T, DIM>>& target_basis_shared) const
{
//	if (target_basis_shared == nullptr)        throw std::invalid_argument("null shared pointer to basis");

	const Matrix<T, DIM>* target_basis = target_basis_shared.get();
	if (target_basis == nullptr) throw std::invalid_argument("null  pointer to basis");

	Matrix<T, DIM> comp_at_terget_basis((T)0);
	const Matrix<T, DIM>* current_basis = basis->get();
	if (current_basis == target_basis)
		comp_at_terget_basis = *this;
	else
	{
		//comp_at_terget_basis = *this * 
	}
}