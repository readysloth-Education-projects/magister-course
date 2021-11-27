#include <string.h>

#include <vector>

class Animal {
protected:
	char *name;
public:
	Animal() : name(NULL) {}
	Animal(char *name) { setname(name); }
	Animal(const Animal &src) { setname(src.name); }
	virtual ~Animal() {};
	char * getname();
	void setname(char *);
	virtual void show(void) = 0;
	static void print(void);
	void add();
	static int count;
	static std::vector<Animal>
};

char * Animal::getname() { return this->name; }

void Animal::setname(char *name) {
	int n = strlen(name);
	if (n > 0) {
		this->name = (char *)calloc(strlen(name), sizeof(char));
		if (this->name != NULL) strcpy(this->name, name);
	}
}

void Animal::add() {
	if (count < size) {
		animals[count++] = this;
		if (count < size) animals[count] = NULL;
	}
}

void Animal::print() {
	printf("\nCount of animals types: %d\n", count);
	for (int i = 0; i < count; i++)
		if (animals[i] != NULL) animals[i]->show();
}

int Animal::count = 0;

Animal ** Animal::animals = new Animal *[size];
Animal * Animal::begin = Animal::animals[0];

class Mammal : public Animal {
	int amount;
public:
	Mammal() : Animal(NULL) {}
	Mammal(char *name, int amount) : Animal(name) { this->amount = amount; }
	~Mammal() { free(name); }
	void show(void);
};

class Bird : public Animal {
	int amount;
public:
	Bird() : Animal(NULL) {}
	Bird(char *name, int amount) : Animal(name) { this->amount = amount; }
	~Bird() { free(name); }
	void show(void);
};

class Hoofed : public Mammal {
	int amount;
public:
	Hoofed() : Mammal(NULL, NULL) {}
	Hoofed(char *name, int amount) : Mammal(name, amount) { this->amount = amount; }
	~Hoofed() { free(name); }
	void show(void);
};


void Mammal::show() { printf("Mammal: %s\n", name); }

void Bird::show() { printf("Bird: %s\n", name); }

void Hoofed::show() { printf("Hoofed: %s\n", name); }


