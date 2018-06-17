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

		TEST_METHOD(TestScalar1)
		{
			using Scalarf = flumath::Scalar<float>;
			Scalarf one = 1.0f;
			Assert::AreEqual(1.0f, one, TOLERANCE, L"Scalar test", LINE_INFO());
		}
	};
}