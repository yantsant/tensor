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