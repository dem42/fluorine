// FlourineTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <xmmintrin.h>
#include <cstdio>
#include <fluorine_math.h>

/*_m128 addWithAssembly(
__m128 a,
__m128 b)
{
__m128 r;
__asm
{
movaps xmm0,
xmmword ptr[a]
movaps xmm1,
xmmword ptr[b]
addps xmm0, xmm1
movaps xmmword ptr[r],
xmm0
}
return r;
}
*/
__m128 addWithIntrinsics(
	__m128 a,
	__m128 b)
{
	__m128 r =
		_mm_add_ps(a, b);
	return r;
}

// ... function definitions from above ...
__declspec(align(16)) float A[] = { 2.0f,-1.0f,3.0f,4.0f };
__declspec(align(16)) float B[] = { -1.0f,3.0f,4.0f,2.0f };
__declspec(align(16)) float C[] = { 0.0f,0.0f,0.0f,0.0f };
__declspec(align(16)) float D[] = { 0.0f,0.0f,0.0f,0.0f };
int main(int argc, char* argv[])
{
	//// load a and b from floating-point data arrays above
	//__m128 a = _mm_load_ps(&A[0]);
	//__m128 b = _mm_load_ps(&B[0]);

	//// test the two functions
	//__m128 c = addWithIntrinsics(a, b);
	//__m128 d = addWithIntrinsics(a, b);
	//// store the original values back to check that they
	//// weren’t overwritten
	//_mm_store_ps(&A[0], a);
	//_mm_store_ps(&B[0], b);
	//// store results into float arrays so we can print
	//// them
	//_mm_store_ps(&C[0], c);
	//_mm_store_ps(&D[0], d);
	//// inspect the results
	//printf("hello %d\n", flumath::test);
	//printf("%g %g %g %g\n", A[0], A[1], A[2], A[3]);
	//printf("%g %g %g %g\n", B[0], B[1], B[2], B[3]);
	//printf("%g %g %g %g\n", C[0], C[1], C[2], C[3]);
	//printf("%g %g %g %g\n", D[0], D[1], D[2], D[3]);

	using namespace flumath;
	float data[1][4] = { {1,2} };	
	Vector4 vec(data);
	Vector4 result = vec * 3;

	Matrix4 mat1({ { 3, 4, 1 }, { 2, 3, 5, 4 } });
	Matrix4 mat2({ { 1, 2, 1 },{ 1, 4, 1, 4 } });
	
	Matrix4 mat3 = mat1 * mat2;
	std::cout << mat3 << std::endl;

	return 0;
}

