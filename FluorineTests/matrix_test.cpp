#include "stdafx.h"
#include "CppUnitTest.h"
#include <fluorine_math.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FluorineTests
{	
const float TOLERANCE = 1e-6f;

	TEST_CLASS(MatrixTest)
	{
	public:
		
		TEST_METHOD(TestMatrixCreateAndAccess1)
		{
			using flumath::Matrix4;

			Matrix4 mat({ {1, 2, 1.3f },{ 1, 4, 2, 5 } });
			Assert::AreEqual(1.0f, mat[0][0], TOLERANCE, L"Value test", LINE_INFO());
			Assert::AreEqual(2.0f, mat[0][1], TOLERANCE, L"Value test", LINE_INFO());
			Assert::AreEqual(1.3f, mat[0][2], TOLERANCE, L"Value test", LINE_INFO());
			Assert::AreEqual(0.0f, mat[0][3], TOLERANCE, L"Value test", LINE_INFO());

			Assert::AreEqual(1.0f, mat[1][0], TOLERANCE, L"Value test", LINE_INFO());
			Assert::AreEqual(4.0f, mat[1][1], TOLERANCE, L"Value test", LINE_INFO());
			Assert::AreEqual(2.0f, mat[1][2], TOLERANCE, L"Value test", LINE_INFO());
			Assert::AreEqual(5.0f, mat[1][3], TOLERANCE, L"Value test", LINE_INFO());
		}

		TEST_METHOD(TestMatrixMultiply1)
		{	
			using Matrix2 = flumath::Matrix<float, 2, 2>;

			Matrix2 mat1({ { 1.0f, 2.0f },{ 3.0f, 4.0f } });
			Matrix2 mat2({ { 3.0f, 1.0f },{ 5.0f, 1.0f } });

			Matrix2 result = mat1 * mat2;
			Assert::AreEqual(13.0f, result[0][0], TOLERANCE, L"Mult test", LINE_INFO());
			Assert::AreEqual(3.0f, result[0][1], TOLERANCE, L"Mult test", LINE_INFO());

			Assert::AreEqual(29.0f, result[1][0], TOLERANCE, L"Mult test", LINE_INFO());
			Assert::AreEqual(7.0f, result[1][1], TOLERANCE, L"Mult test", LINE_INFO());
		}

		TEST_METHOD(TestMatrixTranspose1)
		{
			using Matrix23 = flumath::Matrix<float, 2, 3>;
			using Matrix32 = flumath::Matrix<float, 3, 2>;

			Matrix23 mat1({ { 1.0f, 2.0f, 4.0f },{ 3.0f, 5.0f, 6.0f } });
			Matrix32 result = mat1.T();

			Assert::AreEqual(1.0f, result[0][0], TOLERANCE, L"T 0,0", LINE_INFO());
			Assert::AreEqual(3.0f, result[0][1], TOLERANCE, L"T 0,1", LINE_INFO());

			Assert::AreEqual(2.0f, result[1][0], TOLERANCE, L"T 1,0", LINE_INFO());
			Assert::AreEqual(5.0f, result[1][1], TOLERANCE, L"T 1,1", LINE_INFO());

			Assert::AreEqual(4.0f, result[2][0], TOLERANCE, L"T 2,0", LINE_INFO());
			Assert::AreEqual(6.0f, result[2][1], TOLERANCE, L"T 2,1", LINE_INFO());			
		}

		TEST_METHOD(TestMatrixTranspose2)
		{
			using Matrix2 = flumath::Matrix<float, 2, 2>;			

			Matrix2 mat1({ { 1.0f, 2.0f },{ 3.0f, 4.0f } });
			Matrix2 result = mat1.T();

			Assert::AreEqual(1.0f, result[0][0], TOLERANCE, L"T 0,0", LINE_INFO());
			Assert::AreEqual(3.0f, result[0][1], TOLERANCE, L"T 0,1", LINE_INFO());

			Assert::AreEqual(2.0f, result[1][0], TOLERANCE, L"T 1,0", LINE_INFO());
			Assert::AreEqual(4.0f, result[1][1], TOLERANCE, L"T 1,1", LINE_INFO());
		}

		TEST_METHOD(TestScalar1)
		{
			using Scalarf = flumath::Scalar<float>;
			Scalarf one = 1.0f;
			Assert::AreEqual(1.0f, one, TOLERANCE, L"Scalar test", LINE_INFO());
		}

		TEST_METHOD(TestVectorCreateAndAccess1)
		{
			using Scalarf = flumath::Scalar<float>;
			using Vectorf = flumath::Vector<float, 2>;

			Vectorf v({ 1.0f, 2.0f });
			Assert::AreEqual(1.0f, v[0], TOLERANCE, L"Mult test", LINE_INFO());
			Assert::AreEqual(2.0f, v[1], TOLERANCE, L"Mult test", LINE_INFO());
		}

		TEST_METHOD(TestVectorScalarMultiply1)
		{
			using Scalarf = flumath::Scalar<float>;
			using Vectorf = flumath::Vector<float, 2>;

			Vectorf v({ 1.0f, 2.0f });
			Vectorf result = v * 3.0f;
			Assert::AreEqual(3.0f, result[0], TOLERANCE, L"Mult test", LINE_INFO());
			Assert::AreEqual(6.0f, result[1], TOLERANCE, L"Mult test", LINE_INFO());

			Vectorf v2({ 3.0f, 1.0f });
			result = 2.0f * v2;
			Assert::AreEqual(6.0f, result[0], TOLERANCE, L"Mult test", LINE_INFO());
			Assert::AreEqual(2.0f, result[1], TOLERANCE, L"Mult test", LINE_INFO());
		}

		TEST_METHOD(TestWedgeProductVectorScalar1)
		{
			using Scalarf = flumath::Scalar<float>;
			using Vectorf = flumath::Vector<float, 2>;

			Vectorf v({ 1.0f, 2.0f });
			Vectorf result1 = 2.0f ^ v;
			Assert::AreEqual(2.0f, result1[0], TOLERANCE, L"Vector-scalar left test", LINE_INFO());
			Assert::AreEqual(4.0f, result1[1], TOLERANCE, L"Vector-scalar left test", LINE_INFO());

			Vectorf result2 = v ^ 3.0f;
			Assert::AreEqual(3.0f, result2[0], TOLERANCE, L"Vector-scalar right test", LINE_INFO());
			Assert::AreEqual(6.0f, result2[1], TOLERANCE, L"Vector-scalar right test", LINE_INFO());
		}

		TEST_METHOD(TestWedgeProductVectorVector1)
		{
			using Scalarf = flumath::Scalar<float>;
			using Vectorf = flumath::Vector<float, 3>;
			using Bivectorf = flumath::Bivector<float, 3>;

			Vectorf v1({ 1.0f, 2.0f, 3.0f });
			Vectorf v2({ 4.0f, 1.0f, 2.0f });
			Bivectorf result = v1 ^ v2;
			Assert::AreEqual(v1[1] * v2[2] - v1[2] * v2[1], result[0], TOLERANCE, L"Component e23", LINE_INFO());
			Assert::AreEqual(v1[2] * v2[0] - v1[0] * v2[2], result[1], TOLERANCE, L"Component e31", LINE_INFO());
			Assert::AreEqual(v1[0] * v2[1] - v1[1] * v2[0], result[2], TOLERANCE, L"Component e12", LINE_INFO());
		}



		TEST_METHOD(TestFactorialTemplate)
		{
			size_t v0 = flumath::factorial<0>::value;
			size_t v1 = flumath::factorial<1>::value;
			size_t v2 = flumath::factorial<2>::value;
			size_t v3 = flumath::factorial<3>::value;
			size_t v4 = flumath::factorial<4>::value;
			size_t v5 = flumath::factorial<5>::value;


			Assert::AreEqual<size_t>(1, v0, L"fac test 1", LINE_INFO());
			Assert::AreEqual<size_t>(1, v1, L"fac test 2", LINE_INFO());
			Assert::AreEqual<size_t>(2, v2, L"fac test 3", LINE_INFO());
			Assert::AreEqual<size_t>(6, v3, L"fac test 4", LINE_INFO());
			Assert::AreEqual<size_t>(24, v4, L"fac test 5", LINE_INFO());
			Assert::AreEqual<size_t>(120, v5, L"fac test 6", LINE_INFO());
		}

		TEST_METHOD(TestNChooseKTemplate)
		{
			size_t v0 = flumath::n_choose_k<5, 0>::value;
			size_t v1 = flumath::n_choose_k<5, 1>::value;
			size_t v2 = flumath::n_choose_k<5, 2>::value;
			size_t v3 = flumath::n_choose_k<5, 3>::value;
			size_t v4 = flumath::n_choose_k<5, 4>::value;
			size_t v5 = flumath::n_choose_k<5, 5>::value;


			Assert::AreEqual<size_t>(1, v0, L"nk test 1", LINE_INFO());
			Assert::AreEqual<size_t>(5, v1, L"nk test 2", LINE_INFO());
			Assert::AreEqual<size_t>(10, v2, L"nk test 3", LINE_INFO());
			Assert::AreEqual<size_t>(10, v3, L"nk test 4", LINE_INFO());
			Assert::AreEqual<size_t>(5, v4, L"nk test 5", LINE_INFO());
			Assert::AreEqual<size_t>(1, v5, L"nk test 6", LINE_INFO());
		}
	};
}