#pragma once
#include "matrix.h"


//template <typename T>
//using shrptr = std::shared_ptr<const Matrix<T, DIM>>;

template<typename T>
class Tensor : public Matrix<T, DIM>
{
	std::shared_ptr<const Tensor<T>> basis;
public:
	~Tensor() {
		basis.reset();
	}

	virtual Tensor& operator = (const Tensor& t);
	virtual Tensor  operator + (const Tensor& t) const;
	virtual Tensor  operator - (const Tensor& t) const;
	virtual Tensor  operator * (const Tensor& t) const;
	virtual Tensor& operator +=(const Tensor& t);
	virtual Tensor& operator -=(const Tensor& t);
	virtual Tensor  operator *=(const Tensor& t);
	//virtual bool  operator ==(const Tensor& t) const;

	std::shared_ptr<const Tensor<T>> get_basis() const { return basis; };

	Matrix<T, DIM> calc_comp_at_basis(/*Matrix<T, DIM>& comp_at_terget_basis,*/ const Tensor<T>& target_basis) const;
	void move_to_basis(/*Matrix<T, DIM>& comp_at_terget_basis,*/ const Tensor<T>& target_basis);

	Tensor(const Tensor<T>& _basis, MATRIXINITTYPE IT = MATRIXINITTYPE::ZERO) : Matrix<T, DIM>(IT)
	{
		basis = _basis.get_basis();
		const Tensor<double>* bs = basis.get();
	}

	Tensor(MATRIXINITTYPE IT = MATRIXINITTYPE::INDENT) : Matrix<T, DIM>(IT)
	{
		basis = nullptr;
	}

};


template<typename T>
Tensor<T>& Tensor<T>::operator = (const Tensor<T>& t)
{
	Matrix<T, DIM>::copy(t);
	basis = t.basis;
	return *this;
};


template<typename T>
Tensor<T>  Tensor<T>::operator + (const Tensor<T>& t) const
{
	t.calc_comp_at_basis(*this);
	Tensor<T> res;
	return res;
}
template<typename T>					
Tensor<T>  Tensor<T>::operator - (const Tensor<T>& t) const 
{
	Tensor<T> res;
	return res;

}
template<typename T>					
Tensor<T>  Tensor<T>::operator * (const Tensor<T>& t) const
{

	Tensor<T> res;
	return res;
}
template<typename T>					
Tensor<T>& Tensor<T>::operator +=(const Tensor<T>& t)
{
	return *this;
}
template<typename T>					
Tensor<T>& Tensor<T>::operator -=(const Tensor<T>& t)
{
	return *this;
}
template<typename T>					
Tensor<T>  Tensor<T>::operator *=(const Tensor<T>& t)
{
	Tensor<T> res;
	return res;
}

/* return components of this in target basis*/
template<typename T>
Matrix<T, DIM> Tensor<T>::calc_comp_at_basis(//(Matrix<T, DIM> & comp_at_terget_basis,
	    const Tensor<T>& target) const
{
	Matrix<T, DIM> comp_at_terget_basis = (T)0;

	const auto target_basis = target.get_basis();
	if (basis == target_basis)
		comp_at_terget_basis = *this;
	else
	{
		//comp_at_terget_basis = *this * 
	}

	return comp_at_terget_basis;
}