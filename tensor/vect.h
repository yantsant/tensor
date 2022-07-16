#pragma once

#include <iostream>
#include "vectbase.h"
#include "matrix.h"
template<typename T, size_t N>
const matrix_base<T, N> GLOBAL_DEFAULT_BASIS = matrix_base<T, N>(MATRIXINITTYPE::INDENT);


template<typename T, size_t N>
class basis_handler
{
	const matrix_base<T, N>* basis_comp = nullptr;
	const void               set_basis(const matrix_base<T, N>& m) { if (!this->same_basis(m)) basis_comp = &m; };
protected:
	basis_handler() {};
	bool same_basis(const matrix_base<T, N>& m) const { return (basis_comp == &m) ? true : false; };
	const matrix_base<T, N>& get_basis()        const { return *basis_comp; }
	basis_handler(const matrix_base<T, N>& m);
	/*  WARNING : CHANGE Object: recalc components*/
	virtual void recalc_comp_at_basis(const matrix_base<T, N>& m) { 
		_get_comp_at_basis(m, this); 
	};
	virtual void _get_comp_at_basis(const matrix_base<T, N>& m, basis_handler* v) const = 0 ;
public:
	/* WARNING : CHANGE Object: move to the target basis m, just basis changes*/
	void            move_to_basis(const matrix_base<T, N>& m) { set_basis(m); };
	/* not change object: just recalc components*/
	virtual void    change_basis(const matrix_base<T, N>& m);
};


template<typename T, size_t N>
basis_handler<T, N>::basis_handler(const matrix_base<T, N>& m) {
	if (!m.check_ort()) throw std::invalid_argument("basis matrix is not ortogonal");
	basis_comp = &m;
}

template<typename T, size_t N>
void basis_handler<T, N>::change_basis(const matrix_base<T, N>& m) {
	if (!m.check_ort()) throw std::invalid_argument("basis matrix is not ortogonal");
	recalc_comp_at_basis(m);
	set_basis(m);
}

template<typename T, size_t N>
class Vector : public basis_handler<T, N>, public vect_base<T, N>
{
	Vector() {};
	void _get_comp_at_basis(const matrix_base<T, N>& m, basis_handler<T,N>* v) const override;
public:
	vect_base<T, N> get_comp_at_basis(const matrix_base<T, N>& m) const; // calc comp of this at basis

	Vector(const matrix_base<T, N>& m) : basis_handler<T, N>(m) {	};
	Vector(const T& val             , const matrix_base<T, N>& m) : vect_base<T, N>(val), basis_handler<T, N>(m) {};
	Vector(const vect_base<T, N>& v , const matrix_base<T, N>& m) : vect_base<T, N>(v)  , basis_handler<T, N>(m) {};
	Vector(const Vector<T, N>& v    , const matrix_base<T, N>& m);
	Vector(const std::array<T, N>& a, const matrix_base<T, N>& m);

	inline Vector  operator - ();
	inline Vector& operator = (const Vector<T, N>& v);
	inline Vector& operator +=(const Vector<T, N>& v);
	inline Vector& operator -=(const Vector<T, N>& v);
	inline Vector  operator + (const Vector<T, N>& v) const;
	inline Vector  operator - (const Vector<T, N>& v) const;
	inline T       operator * (const Vector<T, N>& v) const;
	inline Vector  operator * (const T& mult) const;
	inline Vector  vector_product(const Vector& rhs)  const;
};


template<typename T, size_t N>
Vector<T, N>::Vector(const Vector<T, N>& v, const matrix_base<T, N>& m) : basis_handler<T, N>(m) {
	static_cast<vect_base<T, N>&>(vect_base<T, N>::operator=(v));
};

template<typename T, size_t N>
Vector<T, N>::Vector(const std::array<T, N>& a, const matrix_base<T, N>& m) : basis_handler<T, N>(m){
	static_cast< std::array<T, N>&>(std::array<T, N>::operator=(a));
};

template<typename T, size_t N>
void Vector<T, N>::_get_comp_at_basis(const matrix_base<T, N>& m, basis_handler<T, N>* v) const {
	vect_base<T, N> res;
	if (this->same_basis(m)) {
		*static_cast<Vector<T, N>*>(v) = *this;
		//*(Vector<T, N>*)(v) = *this;
	}
	else {
		const matrix_base<T, N>& Rl = this->get_basis();
		const matrix_base<T, N>& Rr = m.transpose();
		const vect_base<T, N>& comp = static_cast<const vect_base<T, N>&> (*this);
		res = (comp * Rl) * Rr; // (*this) * this->basis() * m.transpose();
		//vect_base<T, N>& _v = *static_cast<vect_base<T, N>*>((void*)((T*)v + 2));
		//size_t x = sizeof(vect_base<T, N>);
		////*static_cast<vect_base<T, N>*>(v) = res;
		//_v = res;
		//std::cout << *static_cast<vect_base<T, N>*>(v) << typeid(v).name() << "\n";
		//std::cout << _v << typeid(v).name() << "\n";
		//v = &_v;
	}
	//return res;
}

template<typename T, size_t N>
vect_base<T, N> Vector<T, N>::get_comp_at_basis(const matrix_base<T, N>& m) const{
	Vector<T, N> res;
	_get_comp_at_basis(m, &res);
	return static_cast<vect_base<T, N>>(res);
}

//template<typename T, size_t N>
//void Vector<T, N>::recalc_comp_at_basis(const matrix_base<T, N>& m) {
//	get_comp_at_basis(m, this);
//}
template<typename T, size_t N>
Vector<T, N> Vector<T, N>::operator - () {
	return  Vector<T, N>(vect_base<T, N>::operator-(), this->get_basis());
}

template<typename T, size_t N>
Vector<T, N>& Vector<T, N>::operator = (const Vector<T, N>& v) {
	return static_cast<Vector<T, N>&>(vect_base<T, N>::operator=(v.get_comp_at_basis(this->get_basis())));
}

template<typename T, size_t N>
Vector<T, N> Vector<T, N>::operator + (const Vector<T, N>& v) const {
	return  Vector<T, N>(vect_base<T, N>::operator+(v.get_comp_at_basis(this->get_basis())), this->get_basis());
}

template<typename T, size_t N>
Vector<T, N> Vector<T, N>::operator - (const Vector<T, N>& v) const {
	return  Vector<T, N>(vect_base<T, N>::operator-(v.get_comp_at_basis(this->get_basis())), this->get_basis());
}

template<typename T, size_t N>
Vector<T, N>& Vector<T, N>::operator +=(const Vector<T, N>& v) {
	vect_base<T, N>::operator+=(v.comp_at_basis(this->get_basis()));
	return *this;
}

template<typename T, size_t N>
Vector<T, N>& Vector<T, N>::operator -=(const Vector<T, N>& v) {
	vect_base<T, N>::operator-=(v.get_comp_at_basis(this->get_basis()));
	return *this;
}

template<typename T, size_t N>
T Vector<T, N>::operator * (const Vector<T, N>& v) const {
	return vect_base<T, N>::operator*(v.get_comp_at_basis(this->get_basis()));
}

template<typename T, size_t N>
Vector<T, N> Vector<T, N>::operator * (const T& mult) const {
	return Vector<T, N>(vect_base<T, N>::operator*(mult), this->get_basis());
}

template<typename T, size_t N>
Vector<T, N> Vector<T, N >::vector_product(const Vector& rhs) const{
	vect_base<T, N > comp = vect_base<T, N >::vector_product(rhs.get_comp_at_basis(this->get_basis()));
	return Vector<T, N>(comp, this->get_basis());
}
