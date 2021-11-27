#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include "animal.hpp"

const int size = 8;

class Food {
public:
	int price;
	Food() : price(0) {}
	Food(int price) { setprice(price); }
	~Food() {}
	void setprice(int);
};

void Food::setprice(int price) {
	this->price = price;
}

void main() {
	char* animal = new char[10];
	strcpy(animal, "Cat");
	Mammal *cat = new Mammal(animal, 10);
	cat->show();

	strcpy(animal, "Crow");
	Bird *crow = new Bird(animal, 25);
	crow->show();

	strcpy(animal, "Elk");
	Hoofed *elk = new Hoofed(animal, 2);
	elk->show();

	Food mammals_food(200);
	Food birds_food(50);
	Food hoofeds_fod(mammals_food.price * 5);

	cat->add();
	crow->add();
	elk->add();

	Animal::print();

	// Подсчет стоимости содержания животных
	printf("\nRation value: %d\n", cat->count * mammals_food.price +
		crow->count * birds_food.price + elk->count * hoofeds_fod.price);

	delete elk;
	delete crow;
	delete cat;
	delete animal;

	system("pause");
}
