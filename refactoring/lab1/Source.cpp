#include <ctime>
#include <cstdlib>

#include <tuple>
#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

typedef std::vector<int> row_t;
typedef std::vector<row_t> matrix_t;

void* MEMORY_POOL[512] = {nullptr};
int MEMORY_POOL_HEAD = 0;


#define NEW(TYPE, WRITE_VAL) auto ptr ## TYPE ## WRITE_VAL = new TYPE();\
														 MEMORY_POOL[MEMORY_POOL_HEAD] = ptr ## TYPE ## WRITE_VAL; \
														 MEMORY_POOL_HEAD++; \
														 WRITE_VAL = *(ptr ## TYPE ## WRITE_VAL);

#define FREE() for(int i = 0; i < MEMORY_POOL_HEAD; i ++) {delete MEMORY_POOL[i];}


#define INIT_MATRIX(word, num, size) \
	matrix_t word ## num; \
	NEW(matrix_t, word ## num);\
	word ## num.resize(size);\
	    for (auto &row : word ## num){\
			row.resize(size);\
    	}

template<typename IT, int Count>
std::vector<IT> get_user_input(const std::string &out_text,
															 const std::string &failure_text,
															 std::function<bool(IT)> &failure_condition){
	std::vector<IT> input_array;

	std::cout << out_text << std::endl;
	for(int x_dim= 0; x_dim< Count; x_dim++){
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
									std::function<int()> &populate_function){
	for(int x_dim= 0; x_dim< count; x_dim++){
		row.push_back(populate_function());
	}
}


void populate_matrix(matrix_t &matrix,
										 std::tuple<int,int> dimensions,
										 std::function<int()> &populate_function){
	int x_dim = std::get<0>(dimensions);
	int y_dim = std::get<1>(dimensions);
	for(int i = 0; i < x_dim; i++){
		row_t row;
		NEW(row_t, row);
		populate_row(row, y_dim, populate_function);
		matrix.push_back(row);
	}
}

void print_matrix(matrix_t &matrix){
	for(auto &row : matrix){
		for(auto &elem : row){
			std::cout << elem << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


auto get_tuple_of_partitioned_matrix(matrix_t &default_mat, int dimension)
{
	INIT_MATRIX(blockMatrix, 11, dimension/2);
	INIT_MATRIX(blockMatrix, 12, dimension/2);
	INIT_MATRIX(blockMatrix, 21, dimension/2);
	INIT_MATRIX(blockMatrix, 22, dimension/2);
    for (int x_dim = 0; x_dim < dimension/2; x_dim++)
    {
        for (int y_dim = 0; y_dim < dimension/2; y_dim++)
        {
			blockMatrix11[x_dim][y_dim] = default_mat[x_dim][y_dim];
			blockMatrix12[x_dim][y_dim] = default_mat[x_dim][y_dim + dimension / 2];
			blockMatrix21[x_dim][y_dim] = default_mat[x_dim+ dimension / 2][y_dim];
			blockMatrix22[x_dim][y_dim] = default_mat[x_dim+ dimension / 2][y_dim+ dimension / 2];
        }
    }
	return std::make_tuple(blockMatrix11, blockMatrix12, blockMatrix21, blockMatrix22);
}

int main()
{
	srand(time(NULL));
	int extended_size = 2;
	std::cout << "Вас приветствует программа\nбыстрого перемножения матриц методом Штрассена\n\n";
	auto first_matrix_dimensions = get_dimensions("Введите размеры первой матрицы");
	auto second_matrix_dimensions = get_dimensions("Введите размеры второй матрицы");

	matrix_t first_matrix, second_matrix;
	NEW(matrix_t, first_matrix);
	NEW(matrix_t, second_matrix);


	std::function<bool(int)> failure_choice = [](int choice){return !(1 == choice || choice == 2);};
	int choice = get_user_input<int, 1>("Выберите способ заполнения матриц\n1 - Вручную \n2 - Случайным образом\n",
																			"Введите 1 или 2",
																			failure_choice)[0];

	std::function<int()> populate_function = nullptr;

	switch (choice)
	{
		case 1:
			populate_function = [](){int value; std::cin >> value; return value;};
		break;
	case 2:
			populate_function = [](){return rand()%10;};
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


	auto first_matrix_max_dimension = std::max(std::get<0>(first_matrix_dimensions),
																						 std::get<1>(first_matrix_dimensions));
	auto second_matrix_max_dimension =std::max(std::get<0>(second_matrix_dimensions),
																						 std::get<1>(second_matrix_dimensions));
	while (extended_size < first_matrix_max_dimension || extended_size < second_matrix_max_dimension){
		extended_size *= 2;
	}

	first_matrix.resize(extended_size);
	for (auto &row : first_matrix){
			row.resize(extended_size);
	}
	second_matrix.resize(extended_size);
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

	auto first_part_matrix_tuple = get_tuple_of_partitioned_matrix(first_matrix, extended_size);
	auto second_part_matrix_tuple = get_tuple_of_partitioned_matrix(second_matrix, extended_size);
	std::cout << "Блочные матрицы 1:" << std::endl;
    std::apply([](auto&&... matrix) {(print_matrix(matrix), ...);}, first_part_matrix_tuple);
	std::cout << "Блочные матрицы 2:" << std::endl;
    std::apply([](auto&&... matrix) {(print_matrix(matrix), ...);}, second_part_matrix_tuple);


	/////////////////////////////////////////////////////////////////////////////////
	//////////////////////////Создание промежуточных матриц//////////////////////////
	/////////////////////////////////////////////////////////////////////////////////

	INIT_MATRIX(p, 1, extended_size/2);
	INIT_MATRIX(p, 2, extended_size/2);
	INIT_MATRIX(p, 3, extended_size/2);
	INIT_MATRIX(p, 4, extended_size/2);
	INIT_MATRIX(p, 5, extended_size/2);
	INIT_MATRIX(p, 6, extended_size/2);
	INIT_MATRIX(p, 7, extended_size/2);
	
	/////////////////////////////////////////////////////////////////////////////////
	//////////////////////Вычисление значений промежуточных матриц///////////////////
	/////////////////////////////////////////////////////////////////////////////////

	for (int x_dim= 0; x_dim< extended_size / 2; x_dim++)
	{
		for (int y_dim = 0; y_dim < extended_size / 2; y_dim++)
		{
			for (int z = 0; z < extended_size / 2; z++)
			{
				#define GET_ELEMENT_XDIM(TUPLE, I) std::get<I>(TUPLE)[x_dim][z]
				#define GET_ELEMENT_YDIM(TUPLE, I) std::get<I>(TUPLE)[z][y_dim]

				p1[x_dim][y_dim] += (GET_ELEMENT_XDIM(first_part_matrix_tuple, 0) + GET_ELEMENT_XDIM(first_part_matrix_tuple, 3)) \
														* (GET_ELEMENT_YDIM(second_part_matrix_tuple, 0) + GET_ELEMENT_YDIM(second_part_matrix_tuple, 3));

				p2[x_dim][y_dim] += (GET_ELEMENT_XDIM(first_part_matrix_tuple, 2) + GET_ELEMENT_XDIM(first_part_matrix_tuple, 3)) \
														* GET_ELEMENT_YDIM(second_part_matrix_tuple, 0);

				p3[x_dim][y_dim] += GET_ELEMENT_XDIM(first_part_matrix_tuple, 0) \
														* (GET_ELEMENT_YDIM(second_part_matrix_tuple, 1) - GET_ELEMENT_YDIM(second_part_matrix_tuple, 3));

				p4[x_dim][y_dim] += GET_ELEMENT_XDIM(first_part_matrix_tuple, 3) \
														* (GET_ELEMENT_YDIM(second_part_matrix_tuple, 2) - GET_ELEMENT_YDIM(second_part_matrix_tuple, 0));

				p5[x_dim][y_dim] += (GET_ELEMENT_XDIM(first_part_matrix_tuple, 0) + GET_ELEMENT_XDIM(first_part_matrix_tuple, 1)) \
														* GET_ELEMENT_YDIM(second_part_matrix_tuple, 3);

				p6[x_dim][y_dim] += (GET_ELEMENT_XDIM(first_part_matrix_tuple, 2) - GET_ELEMENT_XDIM(first_part_matrix_tuple, 0)) \
														* (GET_ELEMENT_YDIM(second_part_matrix_tuple, 0) + GET_ELEMENT_YDIM(second_part_matrix_tuple, 1));

				p7[x_dim][y_dim] += (GET_ELEMENT_XDIM(first_part_matrix_tuple, 1) - GET_ELEMENT_XDIM(first_part_matrix_tuple, 3)) \
														* (GET_ELEMENT_YDIM(second_part_matrix_tuple, 2) + GET_ELEMENT_YDIM(second_part_matrix_tuple, 3));

			}
		}
	}
    INIT_MATRIX(result, _matrix, extended_size);
	for (int x_dim= 0; x_dim< extended_size / 2; x_dim++)
	{
		for (int y_dim = 0; y_dim < extended_size / 2; y_dim++)
		{
			result_matrix[x_dim][y_dim] = p1[x_dim][y_dim] + p4[x_dim][y_dim] - p5[x_dim][y_dim] + p7[x_dim][y_dim];
			result_matrix[x_dim][y_dim + extended_size / 2] = p3[x_dim][y_dim] + p5[x_dim][y_dim];
			result_matrix[x_dim+ extended_size / 2][y_dim] = p2[x_dim][y_dim] + p4[x_dim][y_dim];
			result_matrix[x_dim+ extended_size / 2][y_dim + extended_size / 2] = p1[x_dim][y_dim] - p2[x_dim][y_dim] + p3[x_dim][y_dim] + p6[x_dim][y_dim];
		}
	}
	/////////////////////////////////////////////////////////////////////////////////
	//////////////////Выравнивание границ результирующей матрицы/////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	int x = 0, y=0, sizeResMatrixX = extended_size, sizeResMatrixY = extended_size;
	for (int x_dim= 0; x_dim< extended_size; x_dim++)
	{
		x = 0;
		y = 0;
		for (int y_dim = 0; y_dim < extended_size; y_dim++)
		{
			if (result_matrix[x_dim][y_dim] != 0)
			{
				x++;
				sizeResMatrixX = extended_size;
			}
			if (result_matrix[y_dim][x_dim] != 0)
			{
				y++;
				sizeResMatrixY = extended_size;
			}
		}
		if (x == 0 && x_dim< sizeResMatrixX)
		{
			sizeResMatrixX = x_dim;
		}
		if (y == 0 && x_dim< sizeResMatrixY)
		{
			sizeResMatrixY = x_dim;
		}
	}

	std::cout << "Результирующая матрица" << std::endl;
	print_matrix(result_matrix);
	FREE();

}
