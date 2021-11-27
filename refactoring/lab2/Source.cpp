#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int size = 8;

class FOOD {
public:
	int price;
	FOOD() : price(0) {}
	FOOD(int price) { setprice(price); }
	~FOOD() {}
	void setprice(int);
};

void FOOD::setprice(int price) {
	this->price = price;
}

class ANIMAL {
protected:
	char *name;
public:
	ANIMAL() : name(NULL) {}
	ANIMAL(char *name) { setname(name); }
	ANIMAL(const ANIMAL &src) { setname(src.name); }
	virtual ~ANIMAL() {};
	char * getname();
	void setname(char *);
	virtual void show(void) = 0;
	static void print(void);
	void add();
	static int count;
	static ANIMAL **animals;
	static ANIMAL *begin;
};

char * ANIMAL::getname() { return this->name; }

void ANIMAL::setname(char *name) {
	int n = strlen(name);
	if (n > 0) {
		this->name = (char *)calloc(strlen(name), sizeof(char));
		if (this->name != NULL) strcpy(this->name, name);
	}
}

void ANIMAL::add() {
	if (count < size) {
		animals[count++] = this;
		if (count < size) animals[count] = NULL;
	}
}

void ANIMAL::print() {
	printf("\nCount of animals types: %d\n", count);
	for (int i = 0; i < count; i++)
		if (animals[i] != NULL) animals[i]->show();
}

int ANIMAL::count = 0;

ANIMAL ** ANIMAL::animals = new ANIMAL *[size];
ANIMAL * ANIMAL::begin = ANIMAL::animals[0];

class MAMMAL : public ANIMAL {
	int amount;
public:
	MAMMAL() : ANIMAL(NULL) {}
	MAMMAL(char *name, int amount) : ANIMAL(name) { this->amount = amount; }
	~MAMMAL() { free(name); }
	void show(void);
};

class BIRD : public ANIMAL {
	int amount;
public:
	BIRD() : ANIMAL(NULL) {}
	BIRD(char *name, int amount) : ANIMAL(name) { this->amount = amount; }
	~BIRD() { free(name); }
	void show(void);
};

class HOOFED : public MAMMAL {
	int amount;
public:
	HOOFED() : MAMMAL(NULL, NULL) {}
	HOOFED(char *name, int amount) : MAMMAL(name, amount) { this->amount = amount; }
	~HOOFED() { free(name); }
	void show(void);
};


void MAMMAL::show() { printf("MAMMAL: %s\n", name); }

void BIRD::show() { printf("BIRD: %s\n", name); }

void HOOFED::show() { printf("HOOFED: %s\n", name); }


void main() {
	char* animal = new char[10];
	strcpy(animal, "Cat");
	MAMMAL *cat = new MAMMAL(animal, 10);
	cat->show();

	strcpy(animal, "Crow");
	BIRD *crow = new BIRD(animal, 25);
	crow->show();

	strcpy(animal, "Elk");
	HOOFED *elk = new HOOFED(animal, 2);
	elk->show();

	FOOD mammals_food(200);
	FOOD birds_food(50);
	FOOD hoofeds_fod(mammals_food.price * 5);

	cat->add();
	crow->add();
	elk->add();

	ANIMAL::print(); 

	// ѕодсчет стоимости содержани€ животных
	printf("\nRation value: %d\n", cat->count * mammals_food.price +
		crow->count * birds_food.price + elk->count * hoofeds_fod.price);

	delete elk;
	delete crow;
	delete cat;
	delete animal;

	system("pause");
}