#pragma once
#include <array>
#include <random>
#include <iostream>
#include <cassert>


template<typename container_type>
class container
{
	void _alloc() { assert(_Elem.get() == nullptr && " vect_base alloc may be a cause of a leaking memory."); _Elem = std::make_unique<container_type>(); };
	void _reset() {  { _Elem.reset(nullptr); } };
	container& _move(container&& rhs) {
		assert(rhs._Elem.get() != nullptr && " move from empty container.");
		_reset();
		std::swap(_Elem, rhs._Elem);
		return *this;
	};
	container& _copy(const container& rhs) {
		if (_Elem.get() == nullptr) _alloc();
		assert(rhs._Elem.get() != nullptr && " copy from empty container.");
		*this->_Elem.get() = *rhs._Elem.get();
		return *this;
	};
protected:
	std::unique_ptr<container_type> _Elem;
	~container()                           { _reset(); };
	container ()                           { _alloc(); };
	container(const container_type& _data) { _alloc(); *this->_Elem.get() = _data; };
	container(const container& rhs)        { _alloc(); _copy(rhs); };
	container(container&& c)noexcept       { _move(static_cast<container&&>(c)); }; // move constructor
public:
	inline const container_type& operator()() const       { return *_Elem.get(); };
	inline container& operator= (const container& rhs)    { _copy(rhs); return *this; };
	inline container& operator= (container&& c) noexcept  { return _move(static_cast<container&&>(c)); };
};


template<typename container_type>
class containerN : std::vector<container_type>
{
	containerN<container_type>* _Elem = nullptr;
	size_t RANK;
	size_t DIM;
public:
	~containerN() {};
	containerN() {};
	containerN(const containerN& cont) {
		*this->_Elem = *cont._Elem;
	};
	containerN& operator = (const containerN& rhs) {
		this->_Elem = rhs._Elem;
		return *this;
	}
	containerN(const std::vector<size_t>& DIMENSIONS) {
		RANK = DIMENSIONS.size();
		DIM  = DIMENSIONS.back();
		if (RANK > 1) {
			std::vector<size_t> NASTED_DIM(DIMENSIONS);
			NASTED_DIM.pop_back();
			_Elem = new containerN<container_type>[RANK];
			for (size_t i = 0; i < DIM; i++)
				_Elem[i] = containerN<container_type>(NASTED_DIM);
		}
		else {
			_Elem = (containerN<container_type>*) (new std::vector<container_type>[DIM]);
			_Elem->resize(DIM);
			std::cout << "create vector size of " << DIM << "\n";
			return;
		}
	};
};


template<typename container_type, size_t N>
class container1d
{
	void _alloc() { assert(_Elem.get() == nullptr && " vect_base alloc may be a cause of a leaking memory."); _Elem = std::make_unique<container_type[]>(N); };
	void _reset() { { _Elem.reset(nullptr); } };
	container1d& _move(container1d&& rhs) {
		assert(rhs._Elem.get() != nullptr && " move from empty container1d.");
		_reset();
		std::swap(_Elem, rhs._Elem);
		return *this;
	};
	container1d& _copy(const container1d& rhs) {
		if (_Elem.get() == nullptr) _alloc();
		assert(rhs._Elem.get() != nullptr && " copy from empty container1d.");
		*this->_Elem.get() = *rhs._Elem.get();
		return *this;
	};
protected:
	std::unique_ptr<container_type[]> _Elem;
public:
	~container1d() { _reset(); };
	container1d()  { _alloc(); };
	      container_type& operator [](size_t i)          { return _Elem.get()[i]; };
	const container_type& operator [](size_t i) const    { return _Elem.get()[i]; };
	inline const container_type& operator()() const { return *_Elem.get(); };
	inline container1d& operator= (const container1d& rhs) { _copy(rhs); return *this; };
	inline container1d& operator= (container1d&& c) noexcept { return _move(static_cast<container1d&&>(c)); };

	container1d(const container_type& _data) { _alloc(); *this->_Elem.get() = _data; };
	container1d(const container1d& rhs) { _alloc(); _copy(rhs); };
	container1d(container1d&& c)noexcept { _move(static_cast<container1d&&>(c)); }; // move constructor
};

template<typename container_type, size_t N, size_t M>
class container2d : container1d<container_type, N * M>
{
	typedef container1d < container_type, M> _subcontainer;
	//std::unique_ptr<_subcontainer[]> _Row;
	_subcontainer _Row[N];
public:
	container2d() : container1d<container_type, N* M>() {
		//_Row = std::make_unique<_subcontainer[]>(N);
	};
	container1d<container_type, M>& operator []  (size_t i){ return _Row[i]; };
};