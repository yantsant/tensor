#pragma once

template<typename T>
class quat : public vect<T, 4>
{
public:
	quat();// { *this = (T)0;  this->set(0, 1); };
	quat(const quat& q);
	quat(const vect<T, 4>& q);
	quat(const T& re, const vect<T, 3>& im);// { this->set(0, re); Im = im; Im.normalize(); };
	~quat() {};
	void set_im(const vect<T, 3>& im);


	inline vect<T, 4>& operator()() { return *this; };
	inline quat& operator = (const vect<T, 4>& v);
	inline quat operator * (const quat& rhs) const;
	inline quat operator * (const T& mult) const;
	inline quat& operator *=(const T& mult);
private:
	inline vect<T, 3> im() const;
	inline T re() const;
};



template<typename T>
quat<T>::quat() : vect<T, 4> ((T)0) { 
	this->set(0, 1);
}; 

template<typename T>
quat<T>::quat(const T& re, const vect<T, 3>& im) {
	this->set(0, re);
	set_im(im);
	this->normalize();
};

template<typename T>
quat<T>::quat(const quat& q ) {;
	this->set(0, q.get(0));
	set_im(q.im());
	this->normalize();
};


template<typename T>
quat<T>::quat(const vect<T, 4>& v) {
	(*this) = v;
	this->normalize();
};

template<typename T>
inline void quat<T>::set_im(const vect<T, 3>& im)
{
	vect<T, 3> res;
	const vect<T, 4>& v = (*this);
	this->set(1, im.get(0));
	this->set(2, im.get(1));
	this->set(3, im.get(2));
}

template<typename T>
inline vect<T, 3> quat<T>::im() const
{
	vect<T, 3> res;
	const vect<T, 4>& v = (*this);

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
inline quat<T>& quat<T>::operator = (const vect<T, 4>& v)
{
	//vect<T, 4> vv = (*this)();
	return static_cast<quat<T>&>(vect<T, 4>::operator=(v));
}


template<typename T>
inline quat<T> quat<T>::operator * (const T& mult) const
{ 
	//vect<T, 4>& lhs = static_cast<quat<T>&>(*this);
	//lhs *= mult;
	quat<T> res;// = lhs;
	return res;
}

template<typename T>
inline quat<T>  quat<T>::operator * (const quat<T>& rhs) const
{
	quat<T> res;
	vect<T, 3> lv = im();
	vect<T, 3> rv = rhs.im();
	T lr = re();
	T rr = rhs.re();

	res.set(0, lr * rr - lv * rv);
	res.set_im(rv * lr + lv * rr + lv.vector_product(rv));
	return res;
}
