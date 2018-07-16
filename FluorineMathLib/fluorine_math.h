#pragma once

#include <cstdio>
#include <cstring>
#include <type_traits>
#include <iostream>
#include <array>

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
template <typename CRType, typename Type, size_t R, size_t C, typename ColType>
struct MatrixBase {

	// zero the memory
	Type mData[R][C] = {};

	MatrixBase() {}
	MatrixBase(const Type input[R][C]) {
		memcpy(mData, input, R*C * sizeof(Type));
	}
	~MatrixBase() {
		std::cout << "is this getting called?" << std::endl;
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
	
	/*todo: if these are never accessed through matrix base then they probably don't need to be virtual (better performance?)*/
	virtual ColType& operator[](size_t rowIdx) = 0;

	virtual const ColType& operator[](size_t rowIdx) const = 0;
};

// Matrix classes and partial specializations
template <typename Type, size_t R, size_t C>
struct Matrix : public MatrixBase<Matrix<Type, R, C>, Type, R, C, Type[C]> {
	Matrix() : MatrixBase<Matrix<Type, R, C>, Type, R, C, Type[C]>() {}
	Matrix(const Type(&input)[R][C]) : MatrixBase<Matrix<Type, R, C>, Type, R, C, Type[C]>(input) {
	}

	typedef Type ColumnType[C];
	ColumnType& operator[](size_t rowIdx) override {
		return mData[rowIdx];
	}

	const ColumnType& operator[](size_t rowIdx) const override {
		return mData[rowIdx];
	}

	Matrix<Type, C, R> T() {
		Matrix<Type, C, R> result;
		for (size_t r = 0; r < R; ++r) {
			for (size_t c = 0; c < C; ++c) {
				result[c][r] = mData[r][c];
			}
		}
		return result;
	}
};

template <typename T, size_t A, size_t B, size_t C, typename CR1, typename CR2>
Matrix<T, A, C> operator*(MatrixBase<CR1, T, A, B, T[B]>& left, MatrixBase<CR2, T, B, C, T[C]>& right) {
	Matrix<T, A, C> result;
	for (size_t i = 0; i < A; ++i) {
		for (size_t j = 0; j < C; ++j) {
			for (size_t k = 0; k < B; ++k) {
				result[i][j] += left[i][k] * right[k][j];
			}
		}
	}
	return result;
}


// Common functionality for vectors, bivectors and antivectors
template <typename CRType, typename Type, size_t C>
struct VectorBase : public MatrixBase<CRType, Type, 1, C, Type> {
	VectorBase() : MatrixBase<CRType, Type, 1, C, Type>() {}
	VectorBase(const Type(&input)[1][C]) : MatrixBase<CRType, Type, 1, C, Type>(input) {}
	VectorBase(const Type(&input)[C]) : MatrixBase<CRType, Type, 1, C, Type>() {
		memcpy(this->mData, input, C * sizeof(Type));
	}

	Type& operator[](size_t rowIdx) override {
		return mData[0][rowIdx];
	}

	const Type& operator[](size_t rowIdx) const override {
		return mData[0][rowIdx];
	}
};

// Vector class for better initialization and accessors
template <typename Type, size_t C>
struct Vector : public VectorBase<Vector<Type, C>, Type, C> {
	Vector() : VectorBase<Vector<Type, C>, Type, C>() {}
	Vector(const Type(&input)[1][C]) : VectorBase<Vector<Type, C>, Type, C>(input) {}
	Vector(const Type(&input)[C]) : VectorBase<Vector<Type, C>, Type, C>(input) {}
};

// Bivector class for better initialization and accessors
template <typename Type, size_t C>
struct Bivector : public VectorBase<Bivector<Type, C>, Type, C> {
	Bivector() : VectorBase<Bivector<Type, C>, Type, C>() {}
	Bivector(const Type(&input)[1][C]) : VectorBase<Bivector<Type, C>, Type, C>(input) {}
	Bivector(const Type(&input)[C]) : VectorBase<Bivector<Type, C>, Type, C>(input) {}
};


using Matrix4 = Matrix<float, 4, 4>;
using Matrix2 = Matrix<float, 2, 2>;


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

/*
* I think this wedge product is right handed (equivalent to cross product in RHS) .. to make it LHS it would be e13 instead of e31 
* which in implementation i think could be achieved by subtracting to i instead of adding (going left instead of right)
*/
template <typename Type, size_t C>
Bivector<Type, n_choose_k<C, 2>::value> operator ^(const Vector<Type, C>& left, const Vector<Type, C>& right) {
	Bivector<Type, n_choose_k<C, 2>::value> result;	
	for (size_t i = 0; i < C; ++i) {
		size_t idx1 = (i + 1) % C;
		size_t idx2 = (i + 2) % C;
		result[i] = left[idx1] * right[idx2] - left[idx2] * right[idx1];		
	}
	return result;
}

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
