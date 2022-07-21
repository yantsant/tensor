#pragma once
#include "vectbase.h"

enum class QUATFORM
{
	REALIMAGE,
	ANGLEAXIS
};

template<typename T>
class quat : public vect_base<T, 4>
{
public:
	quat();
	explicit quat(const quat& q);
	explicit quat(const vect_base<T, 4>& q);
	explicit quat(const T& re, const vect_base<T, 3>& im, QUATFORM type);

	void set_im(const vect_base<T, 3>& im);
	inline vect_base<T, 3> get_im() const;
	inline T re() const;

	inline quat& operator = (const vect_base<T, 4>& v);
	inline quat operator * (const quat& rhs) const;
	inline quat operator * (const T& mult) const;
	inline quat operator * () const;
	inline quat operator ! () const;

	inline matrix_base<T, 3> get_ort_matrix() const;
	static inline matrix_base<T, 3> get_ort_matrix(const T& angle, const vect_base<T, 3>& axis);
private:
};


template<typename T>
quat<T>::quat() : vect_base<T, 4> ((T)0) {
	this->set(0, (T)1);
}; 

template<typename T>
quat<T>::quat(const T& re, const vect_base<T, 3>& im, QUATFORM type) {
	T angleto2 = (T)0;
	switch (type)
	{
	case QUATFORM::REALIMAGE:
		this->set(0, re);
		set_im(im);
		break;
	case QUATFORM::ANGLEAXIS:
		angleto2 = re * (T)0.5;
		this->set(0, cos(angleto2));
		set_im(im.get_normalize() * sin(angleto2));
		break;
	default:
		break;
	}
};

template<typename T>
quat<T>::quat(const quat& q ) {
	this->set(0, q.get(0));
	set_im(q.get_im());
};

template<typename T>
quat<T>::quat(const vect_base<T, 4>& v) {
	(*this) = v;
};

template<typename T>
inline void quat<T>::set_im(const vect_base<T, 3>& im){
	this->set(1, im.get(0));
	this->set(2, im.get(1));
	this->set(3, im.get(2));
}

template<typename T>
inline vect_base<T, 3> quat<T>::get_im() const{
	vect_base<T, 3> res;
	const std::array<T, 4>& v = (*this)();
	res.set(0, v.get(1));
	res.set(1, v.get(2));
	res.set(2, v.get(3));
	return res;
}

template<typename T>
inline T quat<T>::re() const{
	return this->get(0);
}

template<typename T>
inline quat<T>& quat<T>::operator = (const vect_base<T, 4>& v){
	return static_cast<quat<T>&>(vect_base<T, 4>::operator=(v));
}

template<typename T>
inline quat<T> quat<T>::operator * () const{
	quat<T> res(*this);
	res.set_im(-this->get_im());
	return res;
}

template<typename T>
inline quat<T> quat<T>::operator ! () const{
	quat<T> res(*(*this));
	T norm2 = static_cast<vect_base<T,4>>(res) * static_cast<vect_base<T, 4>>(res);
	return res / norm2;
}

template<typename T>inline quat<T> quat<T>::operator * (const T& mult) const{
	return static_cast<quat<T>>(vect_base<T, 4>::operator*(mult));
}

template<typename T>
inline quat<T>  quat<T>::operator * (const quat<T>& rhs) const{
	quat<T> res;
	vect_base<T, 3> lv = get_im();
	vect_base<T, 3> rv = rhs.get_im();
	T lr = re();
	T rr = rhs.re();

	res.set(0, lr * rr - lv * rv);
	res.set_im(rv * lr + lv * rr + lv.vector_product(rv));
	return res;
}

template<typename T>
inline matrix_base<T, 3> quat<T>  ::get_ort_matrix() const{
	T wx, wy, wz, xx, yy, yz, xy, xz, zz;
	std::array<std::array<T, 3>, 3> m;
	std::array<T,4> q = this->get_normalize()();

	xx = 2.0 * q[1] * q[1];   xy = 2.0 * q[1] * q[2];   xz = 2.0 * q[1] * q[3];
	yy = 2.0 * q[2] * q[2];   yz = 2.0 * q[2] * q[3];   zz = 2.0 * q[3] * q[3];
	wx = 2.0 * q[0] * q[1];   wy = 2.0 * q[0] * q[2];   wz = 2.0 * q[0] * q[3];

	m[0][0] = 1.0 - (yy + zz); /*+*/ m[0][1] = xy - wz;               /*+*/  m[0][2] = xz + wy;				 /*+*/
	m[1][0] = xy + wz;         /*+*/ m[1][1] = 1.0 - (xx + zz);       /*+*/	 m[1][2] = yz - wx;				 /*+*/
	m[2][0] = xz - wy;         /*+*/ m[2][1] = yz + wx;		          /*+*/  m[2][2] = 1.0 - (xx + yy);/*+*/
	return matrix_base<T,3>(m);
}

template<typename T>
inline matrix_base<T, 3> quat<T>::get_ort_matrix(const T& angle, const vect_base<T, 3>& axis){
	return quat<T>(angle, axis, QUATFORM::ANGLEAXIS).get_ort_matrix();
}
