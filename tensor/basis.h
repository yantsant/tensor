#pragma once
#include "matrbase.h"

template<typename T, size_t N>
class shared_handler_basis : public std::shared_ptr<matrix_base<T, N>>
{
	static const shared_handler_basis<T, N> GLOBAL_DEFAULT_BASIS;
	typedef  matrix_base<T, N> M;
	typedef  shared_handler_basis<T,N> _shared;
	bool owner = false;
protected:
	explicit shared_handler_basis(const shared_handler_basis&  sh) : std::shared_ptr<matrix_base<T, N>>(sh) {}; // to prevent multiply owning out of scope of Vector/Tensor
	shared_handler_basis& operator = (const shared_handler_basis& sh) { std::shared_ptr<M>::operator=(sh); return *this; };

	shared_handler_basis& move(shared_handler_basis&& rhs) {
		static_cast<_shared&>(*this) = std::move(static_cast<_shared&&>(rhs));
		return *this;
	};
	const void     set_basis(const _shared& rhs) { *this = (rhs); }
public:
	shared_handler_basis(const shared_handler_basis&& sh) : std::shared_ptr<matrix_base<T, N>>(sh) { owner = std::move(sh.owner); };
	explicit shared_handler_basis(const matrix_base<T, N>& m) : std::shared_ptr<matrix_base<T, N>>(std::make_shared<matrix_base<T, N>>(m)) {
		if (!m.check_ort()) throw std::invalid_argument("basis matrix is not ortogonal"); // optionally checking of basis
		owner = true;
	}

	matrix_base<T, N>& as_matrix() { // only creator from matrix_base have access from outer scope, also Tensor/Vector have access  
		if (!owner) throw std::domain_error("access is not  permitted: called object is not an owner");
		return *this->get();
	}; 
	/* WARNING : CHANGE Object: move to the target basis m, just basis changes*/
	void            move_to_basis(const _shared& m) { set_basis(m); };
	/* not change object: just recalc components*/
	virtual void    change_basis (const _shared& m) { set_basis(m); }; // must be overrided in chldren classes

	friend std::ostream& operator<< (std::ostream& out, const shared_handler_basis& t) { out << *(t.get()); return out; };
};

template<typename T, size_t N>
const shared_handler_basis<T, N> GLOBAL_DEFAULT_BASIS = shared_handler_basis<T, N>(matrix_base<T, N>(MATRIXINITTYPE::INDENT));