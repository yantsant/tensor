#pragma once
#include "matrbase.h"

template<typename T, size_t N>
class shared_handler_basis : public std::shared_ptr<matrix_base<T, N>> {
	typedef  matrix_base<T, N> M;
	typedef  std::shared_ptr <M> _shared;

	static const shared_handler_basis<T, N> GLOBAL_DEFAULT_BASIS;
	bool owner = false;
protected:
	shared_handler_basis(const shared_handler_basis&  sh) : _shared(sh) {}; // to prevent multiply owning out of scope of Vector/Tensor
	shared_handler_basis(shared_handler_basis&& sh) noexcept  { move(static_cast<shared_handler_basis&&>(sh)); };
	shared_handler_basis& operator = (const shared_handler_basis& sh) { _shared::operator=(sh); return *this; };
	shared_handler_basis& operator = (shared_handler_basis&& sh) noexcept { return move(static_cast<shared_handler_basis&&>(sh));};
	shared_handler_basis& move(shared_handler_basis&& rhs) {
		_shared::reset();
		static_cast<_shared&>(*this) = std::move(static_cast<_shared&&>(rhs));
		this->owner = rhs.owner;
		return *this;
	};
	const void     set_basis(const shared_handler_basis& rhs) { *this = (rhs); }
public:
	explicit shared_handler_basis(const matrix_base<T, N>& m) : _shared(std::make_shared<matrix_base<T, N>>(m)) {
		assert(m.check_ort() && " basis matrix is not ortogonal"); // optionally checking of basis
		owner = true;
	}

	matrix_base<T, N>& as_matrix() { // only creator from matrix_base have access from outer scope, also Tensor/Vector have access  
		assert(owner && " access is not permitted: called object is not an owner");
		return *this->get();
	}; 
	/* WARNING : CHANGE Object: move to the target basis m, just basis changes*/
	void            move_to_basis(const shared_handler_basis& m) { set_basis(m); };
	/* not change object: just recalc components*/
	virtual void    change_basis (const shared_handler_basis& m) { set_basis(m); }; // must be overrided in chldren classes

	friend std::ostream& operator<< (std::ostream& out, const shared_handler_basis& t) { out << *(t.get()); return out; };
};

template<typename T, size_t N>
const shared_handler_basis<T, N> GLOBAL_DEFAULT_BASIS = shared_handler_basis<T, N>(matrix_base<T, N>(MATRIXINITTYPE::INDENT));