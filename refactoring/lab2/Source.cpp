#include <iostream>

#include "animal.hpp"

typedef int FoodPrice;


int main() {
	auto cats = ZooAnimal("Cat", 10);
	auto crows = ZooAnimal("Crow", 25);
	auto elks = ZooAnimal("Elk", 2);
	cats.show();
	crows.show();
	elks.show();

	FoodPrice cat_food(200);
	FoodPrice crow_food(50);
	FoodPrice elk_food(cat_food * 5);

	show_zoo();

	// Подсчет стоимости содержания животных
	std::cout << "Ration expenses " \
					  << cats.get_count() * cat_food + \
							 crows.get_count() * crow_food + \
							 elks.get_count() * elk_food;
	return 0;
}
