#include <ctime>
#include <cstdlib>

#include <tuple>
#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

typedef std::vector<int> row_t
typedef std::vector<row_t> matrix_t

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


void populate_row(row_t &row,
									int count,
									std::function<int> &populate_function){
	for(int i = 0; i < count; i++){
		row.push_back(populate_function());
	}
}


void populate_matrix(matrix_t &matrix,
										 std::tuple<int,int> dimensions,
										 std::function<int> &populate_function){
	int x_dim = std::get<0>(dimensions);
	int y_dim = std::get<1>(dimensions);
	for(int i = 0; i < x_dim; i++){
		auto row = new row_t();
		matrix.push_back(row);
		populate_row(row, y_dim, populate_function);
	}
}

void print_matrix(matrix_t &matrix){
	for(auto &row : matrix){
		for(auto &elem : row){
			std::cout << elem << " ";
		}
		std::cout << std::endl;
	}
}


int main()
{
	srand(time(NULL));
	int extended_size = 2;
	std::cout << "Вас приветствует программа\nбыстрого перемножения матриц методом Штрассена\n\n";
	auto first_matrix_dimensions = get_dimensions("Введите размеры первой матрицы");
	auto second_matrix_dimensions = get_dimensions("Введите размеры второй матрицы");

	auto first_matrix = new matrix_t();
	auto second_matrix = new matrix_t();


	std::function<bool(int)> failure_choice = [](int choice){return !(1 == choice || choice == 2);};
	int choice = get_user_input<int, 1>("Выберите способ заполнения матриц\n1 - Вручную \n2 - Случайным образом\n",
																			"Введите 1 или 2",
																			failure_choice)[0];

	std::function<int> populate_function = null;

	switch (choice)
	{
		case 1:
			std::function<int> populate_function = [](){int value; std::cin >> value; return value;};
		break;
	case 2:
			std::function<int> populate_function = [](){return rand()%10;};
		break;
	}
	std::cout << "Заполнение матрицы 1" << std::endl;
	populate_matrix(first_matrix,
									first_matrix_dimensions,
									populate_function);
	std::cout << "Матрица 1" << std::endl;
	print_matrix(first_matrix);

	std::cout << "Заполнение матрицы 2" << std::endl;
	populate_matrix(second_matrix,
									second_matrix_dimensions,
									populate_function);
	std::cout << "Матрица 2" << std::endl;
	print_matrix(second_matrix);


	auto first_matrix_max_dimension = std::max_element(first_matrix_dimensions.begin(),
																										 first_matrix_dimensions.end())
	auto second_matrix_max_dimension = std::max_element(second_matrix_dimensions.begin(),
																										  second_matrix_dimensions.end())
	while (extended_size < first_matrix_dimensions || extended_size < second_matrix_dimensions){
		extended_size *= 2;
	}

	first_matrix.resize(extended_size, row_t);
	for (auto &row : first_matrix){
			row.resize(extended_size);
	}
	second_matrix.resize(extended_size, row_t);
	for (auto &row : second_matrix){
			row.resize(extended_size);
	}

	std::cout << "Матрица 1" << std::endl;
	print_matrix(first_matrix);
	std::cout << "Матрица 2" << std::endl;
	print_matrix(second_matrix);

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

