#pragma once

template<typename T, size_t N, typename comp_type>
class basis_handler
{
	const matrix_base<T, N>* basis_comp = nullptr;
protected:
	basis_handler(const matrix_base<T, N>& m) {
		if (!m.check_ort()) throw std::invalid_argument("basis matrix is not ortogonal");
		basis_comp = &m;
	}
	const matrix_base<T, N>& get_basis()        const { return *basis_comp; }
	const void               set_basis(const matrix_base<T, N>& m) {
		if (!m.check_ort()) throw std::invalid_argument("basis matrix is not ortogonal");
		if (!this->same_basis(m)) basis_comp = &m;
	};
	bool same_basis(const matrix_base<T, N>& m) const { return (basis_comp == &m) ? true : false; };
public:
	virtual comp_type get_comp_at_basis(const matrix_base<T, N>& m) const = 0;
	/* WARNING : CHANGE Object: move to the target basis m, just basis changes*/
	void            move_to_basis(const matrix_base<T, N>& m) { set_basis(m); };
	/* not change object: just recalc components*/
	virtual void    change_basis(const matrix_base<T, N>& m) = 0;
};
