#include "hayai\hayai.hpp"
#include <fluorine_math.h>
#include "stdafx.h"

BENCHMARK(FluorineMatrix, MultBenchmark, 5, 20)
{
	using Matrix2 = flumath::Matrix<float, 2, 2>;

	Matrix2 mat1({ { 1.0f, 2.0f },{ 3.0f, 4.0f } });
	Matrix2 mat2({ { 3.0f, 1.0f },{ 5.0f, 1.0f } });

	Matrix2 result = mat1 * mat2;
}

int main()
{
	hayai::ConsoleOutputter consoleOutputter;

	hayai::Benchmarker::AddOutputter(consoleOutputter);
	hayai::Benchmarker::RunAllTests();
	return 0;
}