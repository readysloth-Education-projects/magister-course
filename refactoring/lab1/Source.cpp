#include <ctime>
#include <cstdlib>

#include <tuple>
#include <vector>
#include <iostream>
#include <functional>


template<typename IT, int Count>
std::vector<IT> get_user_input(const std::string &out_text,
															 const std::string &failure_text,
															 std::function<bool(IT)> &failure_condition){
	std::vector<IT> input_array;

	std::cout << out_text << std::endl;
	for(int i = 0; i < Count; i++){
		while(true){
			IT elem;
			std::cin >> elem;
			if (failure_condition(elem)){
				std::cout << failure_text << std::endl;
				continue;
			}
			input_array.push_back(elem);
			break;
		}
	}
	return input_array;
}


std::tuple<int, int> get_dimensions(const std::string &out_text){
	std::function<bool(int)> failure_condition = [](int dim){return dim <= 0;};
	auto dimensions = get_user_input<int, 2>(out_text,
																					 "Вводить значения меньше 0 запрещено",
																					 failure_condition);
	int x_dim = dimensions[0];
	int y_dim = dimensions[1];
	return std::make_tuple(x_dim, y_dim);
}


int main()
{
	srand(time(NULL));
	int l = 2;
	system("chcp 1251");
	std::cout << "Вас приветствует программа\nбыстрого перемножения матриц методом Штрассена\n\n";
	auto first_matrix_dimensions = get_dimensions("Введите размеры первой матрицы");
	auto second_matrix_dimensions = get_dimensions("Введите размеры второй матрицы");

	auto first_matrix = new std::vector<std::vector<int>>();
	auto second_matrix = new std::vector<std::vector<int>>();


	do
	{
		std::cout << "Выберите способ заполнения матриц\n" <<
			"1 - Вручную \n2 - Случайным образом\n";
		std::cin >> k;
	} while (k < 1 || k > 2);
	switch (k)
	{
	case 1:
		for (int i = 0; i < n1; i++)
			for (int j = 0; j < m1; j++)
				std::cin >> M1[i][j];
		for (int i = 0; i < n2; i++)
			for (int j = 0; j < m2; j++)
				std::cin >> M2[i][j];
		std::cout << "\nМатрица 1\n\n";
		for (int i = 0; i < n1; i++)
		{
			for (int j = 0; j < m1; j++)
				std::cout << M1[i][j] << " ";
			std::cout << endl;
		}
		std::cout << "\nМатрица 2\n\n";
		for (int i = 0; i < n2; i++)
		{
			for (int j = 0; j < m2; j++)
				std::cout << M2[i][j] << " ";
			std::cout << endl;
		}
		break;
	case 2:
		for (int i = 0; i < n1; i++)
			for (int j = 0; j < m1; j++)
				M1[i][j] = rand() % 10;
		for (int i = 0; i < n2; i++)
			for (int j = 0; j < m2; j++)
				M2[i][j] = rand() % 10;
		std::cout << "\nМатрица 1\n\n";
		for (int i = 0; i < n1; i++)
		{
			for (int j = 0; j < m1; j++)
				std::cout << M1[i][j] << " ";
			std::cout << endl;
		}
		std::cout << "\nМатрица 2\n\n";
		for (int i = 0; i < n2; i++)
		{
			for (int j = 0; j < m2; j++)
				std::cout << M2[i][j] << " ";
			std::cout << endl;
		}
		break;
	}

	///////////////////////////////////////////////////////////////////////////////
	/////////////////Приведение матриц к требуемому размеру////////////////////////
	///////////////////////////////////////////////////////////////////////////////

	while (l < n1 || l < n2 || l < m1 || l < m2)
		l *= 2;
	int** M3 = new int* [l];
	for (int i = 0; i < l; i++)
	{
		M3[i] = new int[l];
		for (int j = 0; j < l; j++)
			M3[i][j] = 0;
	}
	int** M4 = new int* [l];
	for (int i = 0; i < l; i++)
	{
		M4[i] = new int[l];
		for (int j = 0; j < l; j++)
			M4[i][j] = 0;
	}
	for (int i = 0; i < n1; i++)
	{
		for (int j = 0; j < m1; j++)
			M3[i][j] = M1[i][j];
	}
	for (int i = 0; i < n2; i++)
	{
		for (int j = 0; j < m2; j++)
			M4[i][j] = M2[i][j];
	}
	std::cout << "Приведенные матрицы\n";
	std::cout << "\nМатрица 1\n\n";
	for (int i = 0; i < l; i++)
	{
		for (int j = 0; j < l; j++)
			std::cout << M3[i][j] << " ";
		std::cout << endl;
	}
	std::cout << "\nМатрица 2\n\n";
	for (int i = 0; i < l; i++)
	{
		for (int j = 0; j < l; j++)
			std::cout << M4[i][j] << " ";
		std::cout << endl;
	}

	///////////////////////////////////////////////////////////////////////////////
	///////////////Разбиение матриц на подматрицы и их заполнение//////////////////
	///////////////////////////////////////////////////////////////////////////////

	int** mat1 = new int* [l / 2];
	for (int i = 0; i < l / 2; i++)
	{
		mat1[i] = new int[l / 2];
		for (int j = 0; j < l / 2; j++)
			mat1[i][j] = M3[i][j];
	}
	int** mat2 = new int* [l / 2];
	for (int i = 0; i < l / 2; i++)
	{
		mat2[i] = new int[l / 2];
		for (int j = 0; j < l / 2; j++)
			mat2[i][j] = M3[i][j + l / 2];
	}
	int** mat3 = new int* [l / 2];
	for (int i = 0; i < l / 2; i++)
	{
		mat3[i] = new int[l / 2];
		for (int j = 0; j < l / 2; j++)
			mat3[i][j] = M3[i + l / 2][j];
	}
	int** mat4 = new int* [l / 2];
	for (int i = 0; i < l / 2; i++)
	{
		mat4[i] = new int[l / 2];
		for (int j = 0; j < l / 2; j++)
			mat4[i][j] = M3[i + l / 2][j + l / 2];
	}
	int** mat5 = new int* [l / 2];
	for (int i = 0; i < l / 2; i++)
	{
		mat5[i] = new int[l / 2];
		for (int j = 0; j < l / 2; j++)
			mat5[i][j] = M4[i][j];
	}
	int** mat6 = new int* [l / 2];
	for (int i = 0; i < l / 2; i++)
	{
		mat6[i] = new int[l / 2];
		for (int j = 0; j < l / 2; j++)
			mat6[i][j] = M4[i][j + l / 2];
	}
	int** mat7 = new int* [l / 2];
	for (int i = 0; i < l / 2; i++)
	{
		mat7[i] = new int[l / 2];
		for (int j = 0; j < l / 2; j++)
			mat7[i][j] = M4[i + l / 2][j];
	}
	int** mat8 = new int* [l / 2];
	for (int i = 0; i < l / 2; i++)
	{
		mat8[i] = new int[l / 2];
		for (int j = 0; j < l / 2; j++)
			mat8[i][j] = M4[i + l / 2][j + l / 2];
	}

	///////////////////////////////////////////////////////////////////////////////
	////////////////////////Создание промежуточных матриц//////////////////////////
	///////////////////////////////////////////////////////////////////////////////

	int** p1 = new int* [l / 2];
	for (int i = 0; i < l / 2; i++)
	{
		p1[i] = new int[l / 2];
	}
	int** p2 = new int* [l / 2];
	for (int i = 0; i < l / 2; i++)
	{
		p2[i] = new int[l / 2];
	}
	int** p3 = new int* [l / 2];
	for (int i = 0; i < l / 2; i++)
	{
		p3[i] = new int[l / 2];
	}
	int** p4 = new int* [l / 2];
	for (int i = 0; i < l / 2; i++)
	{
		p4[i] = new int[l / 2];
	}
	int** p5 = new int* [l / 2];
	for (int i = 0; i < l / 2; i++)
	{
		p5[i] = new int[l / 2];
	}
	int** p6 = new int* [l / 2];
	for (int i = 0; i < l / 2; i++)
	{
		p6[i] = new int[l / 2];
	}
	int** p7 = new int* [l / 2];
	for (int i = 0; i < l / 2; i++)
	{
		p7[i] = new int[l / 2];
	}

	///////////////////////////////////////////////////////////////////////////////
	////////////////////Вычисление значений промежуточных матриц///////////////////
	///////////////////////////////////////////////////////////////////////////////

	for (int i = 0; i < l / 2; i++)
	{
		for (int j = 0; j < l / 2; j++)
		{
			p1[i][j] = 0;
			for (int z = 0; z < l / 2; z++)
			{
				p1[i][j] += (mat1[i][z] + mat4[i][z]) * (mat5[z][j] + mat8[z][j]);
			}

			p2[i][j] = 0;
			for (int z = 0; z < l / 2; z++)
			{
				p2[i][j] += (mat3[i][z] + mat4[i][z]) * mat5[z][j];
			}

			p3[i][j] = 0;
			for (int z = 0; z < l / 2; z++)
			{
				p3[i][j] += mat1[i][z] * (mat6[z][j] - mat8[z][j]);
			}

			p4[i][j] = 0;
			for (int z = 0; z < l / 2; z++)
			{
				p4[i][j] += mat4[i][z] * (mat7[z][j] - mat5[z][j]);
			}

			p5[i][j] = 0;
			for (int z = 0; z < l / 2; z++)
			{
				p5[i][j] += (mat1[i][z] + mat2[i][z]) * mat8[z][j];
			}

			p6[i][j] = 0;
			for (int z = 0; z < l / 2; z++)
			{
				p6[i][j] += (mat3[i][z] - mat1[i][z]) * (mat5[z][j] + mat6[z][j]);
			}

			p7[i][j] = 0;
			for (int z = 0; z < l / 2; z++)
			{
				p7[i][j] += (mat2[i][z] - mat4[i][z]) * (mat7[z][j] + mat8[z][j]);
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////Создание вспомогательных матриц/////////////////////////
	///////////////////////////////////////////////////////////////////////////////

	int** mat9 = new int* [l / 2];
	for (int i = 0; i < l / 2; i++)
	{
		mat9[i] = new int[l / 2];
	}
	int** mat10 = new int* [l / 2];
	for (int i = 0; i < l / 2; i++)
	{
		mat10[i] = new int[l / 2];
	}
	int** mat11 = new int* [l / 2];
	for (int i = 0; i < l / 2; i++)
	{
		mat11[i] = new int[l / 2];
	}
	int** mat12 = new int* [l / 2];
	for (int i = 0; i < l / 2; i++)
	{
		mat12[i] = new int[l / 2];
	}

	///////////////////////////////////////////////////////////////////////////////
	////////////Подсчет значений вспомогательных матриц из промежуточных///////////
	///////////////////////////////////////////////////////////////////////////////

	for (int i = 0; i < l / 2; i++)
	{
		for (int j = 0; j < l / 2; j++)
		{
			mat9[i][j] = p1[i][j] + p4[i][j] - p5[i][j] + p7[i][j];
			mat10[i][j] = p3[i][j] + p5[i][j];
			mat11[i][j] = p2[i][j] + p4[i][j];
			mat12[i][j] = p1[i][j] - p2[i][j] + p3[i][j] + p6[i][j];
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	///////////////////Создание результирующей матрицы/////////////////////////////
	///////////////////////////////////////////////////////////////////////////////

	int** M5 = new int* [l];
	for (int i = 0; i < l; i++)
	{
		M5[i] = new int[l];
	}

	///////////////////////////////////////////////////////////////////////////////
	///////Занесение информации из вспомогательных матриц в результирующую/////////
	///////////////////////////////////////////////////////////////////////////////

	for (int i = 0; i < l / 2; i++)
	{
		for (int j = 0; j < l / 2; j++)
		{
			M5[i][j] = mat9[i][j];
			M5[i][j + l / 2] = mat10[i][j];
			M5[i + l / 2][j] = mat11[i][j];
			M5[i + l / 2][j + l / 2] = mat12[i][j];
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	////////////////Выравнивание границ результирующей матрицы/////////////////////
	///////////////////////////////////////////////////////////////////////////////

	int x = 0, f = 100, s = 100;
	for (int i = 0; i < l; i++)
	{
		x = 0;
		for (int j = 0; j < l; j++)
		{
			if (M5[i][j] != 0)
			{
				x++;
				f = 100;
			}
		}
		if (x == 0 && i < f)
		{
			f = i;
		}
	}
	for (int i = 0; i < l; i++)
	{
		x = 0;
		for (int j = 0; j < l; j++)
		{
			if (M5[j][i] != 0)
			{
				x++;
				s = 100;
			}
		}
		if (x == 0 && i < s)
		{
			s = i;
		}
	}

	int** M6 = new int* [f];
	for (int i = 0; i < f; i++)
	{
		M6[i] = new int[s];
		for (int j = 0; j < s; j++)
			M6[i][j] = M5[i][j];
	}

	///////////////////////////////////////////////////////////////////////////////
	///////////////////Вывод результирующей матрицы////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////

	std::cout << "\nРезультирующая матрица\n\n";
	for (int i = 0; i < f; i++)
	{
		for (int j = 0; j < s; j++)
			std::cout << M6[i][j] << " ";
		std::cout << endl;
	}

	system("pause");

	///////////////////////////////////////////////////////////////////////////////
	/////////////////////Очистка динамической памяти///////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	
	for (int i = 0; i < n1; i++)
		delete[] M1[i];
	for (int i = 0; i < n2; i++)
		delete[] M2[i];
	for (int i = 0; i < l; i++)
	{
		delete[] M3[i];
		delete[] M4[i];
		delete[] M5[i];
	}
	for (int i = 0; i < f; i++)
		delete[] M6[i];
	for (int i = 0; i < l / 2; i++)
	{
		delete[] mat1[i];
		delete[] mat2[i];
		delete[] mat3[i];
		delete[] mat4[i];
		delete[] mat5[i];
		delete[] mat6[i];
		delete[] mat7[i];
		delete[] mat8[i];
		delete[] mat9[i];
		delete[] mat10[i];
		delete[] mat11[i];
		delete[] mat12[i];
		delete[] p1[i];
		delete[] p2[i];
		delete[] p3[i];
		delete[] p4[i];
		delete[] p5[i];
		delete[] p6[i];
		delete[] p7[i];
	}
	delete[] M1, M2, M3, M4, M5, M6;
	delete[] mat1, mat2, mat3, mat4, mat5, mat6, mat7, mat8, mat9, mat10, mat11, mat12;
	delete[] p1, p2, p3, p4, p5, p6, p7;

