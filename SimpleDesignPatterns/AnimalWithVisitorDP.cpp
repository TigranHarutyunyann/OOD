#include <iostream>
#include <memory>
#include <vector>
#include <string>
using namespace std;

class Cat;
class Dog;

//-----IVisitor Abstract class-----
class IVisitor {
public:
	IVisitor() {}
	virtual ~IVisitor() {}
	virtual void visit(Cat& c)=0;
	virtual void visit(Dog& d)=0;
};

//-----Animal Abstract class-----
class Animal {
private: 
	string m_name;
public:
	Animal(const string& n) : m_name(n) {}
	virtual ~Animal() {}

   	virtual void accept(IVisitor& v)=0;
	virtual void makeNoise()=0;
	virtual string getName(){ return m_name; }
};

//-----Conrete Animals-----
class Cat : public Animal {
public:
	Cat(const string& name) : Animal(name) {}
	~Cat() {}

	void accept(IVisitor& v) override {
		v.visit(*this);
	}

	void makeNoise() override {
		cout << "saws Meau" << endl; 
	}                 

	string getName() override {
		return Animal::getName();
	}
};

class Dog : public Animal {
public:
	Dog(const string& name) : Animal(name) {}
 	~Dog() {} 
	
	void accept(IVisitor& v) override {
		v.visit(*this);
	}

	void makeNoise() override {
		cout << "saws Woof" << endl;
	}                 

	string getName() override {
		return Animal::getName();
	}
};


//-----Concrete IVistors-----
class YellowVisitor : public IVisitor {
public:
	void visit(Cat& c) override {
		cout << "Cat name is (yellow): "<< c.getName() << "| (yellow): "; c.makeNoise();
	}
	void visit(Dog& d) override {
		cout << "Dog name is (yellow): "<< d.getName() << "| (yellow): "; d.makeNoise();
	}
};

class BlueVisitor : public IVisitor {
public:
	void visit(Cat& c) override {
		cout << "Cat name is (blue): "<< c.getName() << "| (blue): "; c.makeNoise();
	}
	void visit(Dog& d) override {
		cout << "Dog name is (blue): "<< d.getName() << "| (blue): "; d.makeNoise();
	}
};

//-----Client-----
int main() {
    vector<unique_ptr<Animal>> animals;
	animals.push_back(make_unique<Cat>("Murka"));
	animals.push_back(make_unique<Dog>("Rex"));

    YellowVisitor setYellow;
	BlueVisitor setBlue;

    for(auto& pet : animals) {
		pet->accept(setYellow);
		pet->accept(setBlue);
	}
	return 0;
}








