#include <string.h>

#include <set>
#include <vector>
#include <string>
#include <iostream>

void show_zoo();

class Animal {
protected:
	std::string type;
public:
	Animal(const char *type) { this->type = type; }
	Animal(const Animal &src) { this->type = src.type; }
	std::string gettype();
	virtual void show(void) = 0;
	static int count;
};


class ZooAnimal : public Animal {
	int count;
public:
	static std::set<Animal*> zoo;
	ZooAnimal(const char *type, int count);
	void show();
	int get_count();
};
