#include "animal.hpp"


std::string Animal::gettype() {
	return this->type;
}


std::set<Animal*> ZooAnimal::zoo;

ZooAnimal::ZooAnimal(const char *type, int count) : Animal(type) {
		this->count = count;
		this->zoo.insert(this);
}


void ZooAnimal::show() {
	std::cout << this->type << ": " << this->count << std::endl;
}


int ZooAnimal::get_count(){
	return this->count;
}


void show_zoo() {
	for (auto animal : ZooAnimal::zoo){
		animal->show();
	}
}
