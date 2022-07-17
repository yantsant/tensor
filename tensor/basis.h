#pragma once
template<typename M>
class shared_handler_object
{
	std::shared_ptr<const M> ptr;
protected:
	const M* get_ptr()                              const       { return ptr.get(); }
	void     set_ptr(const shared_handler_object<const M>& sh)  { ptr = sh.ptr;}
public:
	shared_handler_object(const shared_handler_object& sh) { ptr = sh.ptr; }
	shared_handler_object(const M& m) { ptr = std::make_shared<const M>(m); };

	bool same_basis(const shared_handler_object<const M>& sh) const { return (this->get_ptr() == sh.get_ptr()) ? true : false; };

	M&       get_stored_object()       { return *ptr.get(); };
	const M& get_stored_object() const { return *ptr.get(); };
};

template<typename T, size_t N, typename comp_type, typename comp_type_2>
class basis_handler : public shared_handler_object<const comp_type>
{
	typedef  shared_handler_object<const comp_type> _shared;
protected:
	basis_handler(const matrix_base<T, N>& m) : _shared(m) {
		if (!m.check_ort()) throw std::invalid_argument("basis matrix is not ortogonal");
	}
	basis_handler(const _shared& m) : _shared(m) {
		if (!m.get_stored_object().check_ort()) throw std::invalid_argument("basis matrix is not ortogonal");
	}

	const _shared& get_basis()           const { return (*this)(); }
	const void     set_basis(const _shared& p) {
		if (!p.get_stored_object().check_ort()) throw std::invalid_argument("basis matrix is not ortogonal");
		this->set_ptr(p);
	};
public:
	const  _shared& operator()() const { return *this; };

	virtual comp_type_2 get_comp_at_basis(const _shared& bh) const = 0;
	/* WARNING : CHANGE Object: move to the target basis m, just basis changes*/
	void            move_to_basis(const _shared& m) { set_basis(m); };
	/* not change object: just recalc components*/
	virtual void    change_basis(const _shared& m) = 0;
};
