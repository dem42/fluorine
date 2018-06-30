#pragma once

#include <cstdio>
#include <cstring>
#include <type_traits>
#include <iostream>

namespace flumath {

extern int test;

// Scalar type that has an implicit constructor
template <typename T>
struct Scalar {
	Scalar(T value) : mValue(value) {}

	T mValue;

	operator T() {
		return mValue;
	}
};

// utilities
template <typename T>
constexpr bool check_list0(const std::initializer_list<T>* i, size_t colNum, size_t num) {
	return (num > 0) ? check_list0(i++, colNum, num - 1) && i->size() == colNum : true;
}
template <typename T>
constexpr bool check_list(size_t size, const std::initializer_list<T>* begin, size_t rowNum, size_t colNum) {	
	return size == rowNum && check_list0(begin, colNum, rowNum);
}

template <size_t N>
struct factorial {
	static constexpr size_t value = N * factorial<N-1>::value;
};

template <>
struct factorial<0> {
	static constexpr size_t value = 1;
};

template <size_t N, size_t K>
struct n_choose_k {
	static constexpr size_t kn = factorial<K>::value;
	static constexpr size_t nn = factorial<N>::value;
	static constexpr size_t zn = factorial<N-K>::value;
	static constexpr size_t value = nn / (kn * zn);
};


// MatrixBase type using CRTP to facilitate code sharing
template <typename CRType, typename Type, size_t R, size_t C>
struct MatrixBase {

	// zero the memory
	Type mData[R][C] = {};

	MatrixBase() {}
	MatrixBase(const Type input[R][C]) {
		memcpy(mData, input, R*C * sizeof(Type));
	}

	typedef Type type;
	const static size_t RowNum = R;
	const static size_t ColNum = C;

	// shared friend functions Barton-Nackman trick
	friend CRType operator*(const CRType& left, const Scalar<Type>& right) {
		CRType result(left.mData);
		for (int i = 0; i < R; ++i) {
			for (int j = 0; j < C; ++j) {
				result.mData[i][j] *= right.mValue;
			}
		}
		return result;
	}
	friend  CRType operator*(const Scalar<Type>& left, const CRType& right) {
		return right * left;
	}

	CRType T() {
		CRType result(mData);
		return result;
	}

	friend std::ostream& operator<<(std::ostream& out, const CRType& obj) {
		for (size_t i = 0; i < R; ++i) {
			for (size_t j = 0; j < C; ++j) {
				out << obj[i][j];
				if (j != C - 1) {
					out << ", ";
				}
				if (i != R - 1) {
					out << std::endl;
				}
			}
		}
		return out;
	}

	typedef Type typeColumn[C];

	typeColumn& operator[](size_t rowIdx) {
		return mData[rowIdx];
	}

	const typeColumn& operator[](size_t rowIdx) const {
		return mData[rowIdx];
	}
};

// Matrix classes and partial specializations
template <typename Type, size_t R, size_t C>
struct Matrix : public MatrixBase<Matrix<Type, R, C>, Type, R, C> {
	Matrix() : MatrixBase<Matrix<Type, R, C>, Type, R, C>() {}
	Matrix(const Type(&input)[R][C]) : MatrixBase<Matrix<Type, R, C>, Type, R, C>(input) {
	}
};

template <typename T, size_t A, size_t B, size_t C, typename CR1, typename CR2>
Matrix<T, A, C> operator*(MatrixBase<CR1, T, A, B>& haha, MatrixBase<CR2, T, B, C>& hoho) {
	Matrix<T, A, C> result;
	for (size_t i = 0; i < A; ++i) {
		for (size_t j = 0; j < C; ++j) {
			for (size_t k = 0; k < B; ++k) {
				result[i][j] += haha[i][k] * hoho[k][j];
			}
		}
	}
	return result;
}

// Vector class for better initialization and accessors
template <typename Type, size_t C>
struct Vector : public MatrixBase<Vector<Type, C>, Type, 1, C> {
	Vector() : MatrixBase<Vector<Type, C>, Type, 1, C>() {}
	Vector(const Type(&input)[1][C]) : MatrixBase<Vector<Type, C>, Type, 1, C>(input) {}
	Vector(const Type(&input)[C]) : MatrixBase<Vector<Type, C>, Type, 1, C>({ input }) {}
};

// Bivector class for better initialization and accessors
template <typename Type, size_t C>
struct Bivector : public MatrixBase<Vector<Type, C>, Type, 1, C> {
	Bivector() : MatrixBase<Bivector<Type, C>, Type, 1, C>() {}
	Bivector(const Type(&input)[1][C]) : MatrixBase<Bivector<Type, C>, Type, 1, C>(input) {}
	Bivector(const Type(&input)[C]) : MatrixBase<Bivector<Type, C>, Type, 1, C>({ input }) {}
};


using Matrix4 = Matrix<float, 4, 4>;
using Matrix2 = Matrix<float, 2, 2>;

/*
template <typename Type, size_t C>
using Vector = Matrix<Type, 1, C>;
*/

using Vector3 = Vector<float, 3>;
using Vector4 = Vector<float, 4>;
using Bivector4 = Vector<float, 6>;

// Wedge operators that form the grassman algebra
inline Scalar<float> operator ^(const Scalar<float>& left, const Scalar<float>& right) {
	return left.mValue * right.mValue;
}

// how is this copying working? our type isnt copyable .. copy elision?
template <typename Type, size_t C>
Vector<Type, C> operator ^(float left, const Vector<Type, C>& right) {
	return right * left;
}

template <typename Type, size_t C>
Vector<Type, C> operator ^(const Vector<Type, C>& left, float right) {
	return right ^ left;
}

//template <typename Type, size_t C>
//Bivector<Type, n_choose_k<C, 2>::value> operator ^(const Vector4& left, const Vector4& right) {
//	return Bivector<Type, C>{};
//}

/*
Plucker coordinates (intersection of two planes (wedge of two planes) gives us a line)
plucker respresentation of a line in 3d space
	a ^ (b + c) = (a ^ b) + (a ^ c)
	e1 ^ e2 = - e2 ^ e1
	a = a1e1 + a2e2 + a3e3 + a4e4
	b = b1e1 + b2e2 + b3e3 + b4e4
	a ^ b = (a1e1 + a2e2 + a3e3 + a4e4) ^ (b1e1 + b2e2 + b3e3 + b4e4)
		  = (a1b2e12 + a1b3e13 + a1b4e14) + (a2b1e21 + a2b3e23 + a2b4e24) + (a3b1e31 + a3b2e32 + a3b4e34) + (a4b1e41 + a4b2e42 + a4b3e43)
		  = (a1b2 - a2b1)e12 + (a2b3 - a3b2)e23 + (a3b1 - a1b3)e31 + (a4b1 - a1b4)e41 + (a4b2 - a2b4)e42 + (a4b3 - a3b4)e43
		  // e12,e23,e31,e41,e42,e43 are our 6 basis vectors (note the order of 4-1 vs 1-4 for example is for handedness i think)
*/




}
