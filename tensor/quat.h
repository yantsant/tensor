#pragma once
#include "array.h"
#include "vect.h"

template<typename T>
class quat : public arr<T, 4>
{
public:
	quat();
	quat(const quat& q);
	quat(const arr<T, 4>& q);
	quat(const T& re, const arr<T, 3>& im);
	~quat() {};
	void set_im(const arr<T, 3>& im);
	inline vect<T, 3> get_im() const;
	inline T re() const;

	inline arr<T, 4>& operator()() { return *this; };
	inline quat& operator = (const arr<T, 4>& v);
	inline quat operator * (const quat& rhs) const;
	inline quat operator * (const T& mult) const;
	inline quat operator * () const;
	inline quat operator ! () const;
private:
};


template<typename T>
quat<T>::quat() : arr<T, 4> ((T)0) {
	this->set(0, 1);
}; 

template<typename T>
quat<T>::quat(const T& re, const arr<T, 3>& im) {
	this->set(0, re);
	set_im(im);
	//this->normalize();
};

template<typename T>
quat<T>::quat(const quat& q ) {;
	this->set(0, q.get(0));
	set_im(q.get_im());
	//this->normalize();
};


template<typename T>
quat<T>::quat(const arr<T, 4>& v) {
	(*this) = v;
	//this->normalize();
};

template<typename T>
inline void quat<T>::set_im(const arr<T, 3>& im)
{
	this->set(1, im.get(0));
	this->set(2, im.get(1));
	this->set(3, im.get(2));
}

template<typename T>
inline vect<T, 3> quat<T>::get_im() const
{
	vect<T, 3> res;
	const quat<T>& v = *this;
	res.set(0, v.get(1));
	res.set(1, v.get(2));
	res.set(2, v.get(3));
	return res;
}

template<typename T>
inline T quat<T>::re() const
{
	return this->get(0);
}


template<typename T>
inline quat<T>& quat<T>::operator = (const arr<T, 4>& v)
{
	return static_cast<quat<T>&>(arr<T, 4>::operator=(v));
}

template<typename T>
inline quat<T> quat<T>::operator * () const
{
	quat<T> res(*this);
	res.set_im(-this->get_im());
	return res;
}

template<typename T>
inline quat<T> quat<T>::operator ! () const
{
	quat<T> res(*(*this));
	T norm2 = static_cast<arr<T,4>>(res) * static_cast<arr<T, 4>>(res);
	return res / norm2;
}

template<typename T>
inline quat<T> quat<T>::operator * (const T& mult) const
{
	return static_cast<quat<T>>(arr<T, 4>::operator*(mult));
}

template<typename T>
inline quat<T>  quat<T>::operator * (const quat<T>& rhs) const
{
	quat<T> res;
	vect<T, 3> lv = get_im();
	vect<T, 3> rv = rhs.get_im();
	T lr = re();
	T rr = rhs.re();

	res.set(0, lr * rr - lv * rv);
	res.set_im(rv * lr + lv * rr + lv.vector_product(rv));
	return res;
}
