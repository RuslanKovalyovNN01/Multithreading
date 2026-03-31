#include <iostream>
#include <vector>
#include <thread>

void hello_function() {
	std::cout << "hello from function\n";
}

void hello_function_with_params(std::string name) {
	std::cout << "hello " + name << " from function with params\n";
}

class hello_class {
public:
	void operator() () {
		std::cout << "hello from class\n";
	}
};

class hello_struct {
public:
	void operator() () {
		std::cout << "hello from struct\n";
	}
};

int main() {
	hello_class hc;
	hello_struct hs;
	std::thread t1(hello_function);
	std::thread t2(hc);
	std::thread t3(hs);
	std::thread t4(hello_function_with_params, "Eva");
	t1.join();
	t2.join();
	t3.join();
	t4.detach();
}