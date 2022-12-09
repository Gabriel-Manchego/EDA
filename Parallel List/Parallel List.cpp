#include <iostream>
#include <mutex>
#include <random>
#include <thread>

using namespace std;

mutex A;

template <class T>
struct Nodo {
	T valor;
	Nodo* next;
};

template <class T>
struct CList {
	CList() {
		head = 0;
	}
	bool Search(T x, Nodo<T>**& p);
	bool Insert(T x);
	bool Delete(T x);
	void print();
private:
	Nodo<T>* head;
};

template <class T>
bool CList<T>::Search(T x, Nodo<T>**& p) {
	for (p = &head; *p != nullptr && (*(*p)).valor < x; p = &((*(*p)).next));
	return (*p) != nullptr && (*(*p)).valor == x;
}

template <class T>
bool CList<T>::Insert(T x) {
	Nodo<T>** p;
	if (Search(x, p)) {
		return 0;
	}
	else {
		A.lock();
		Nodo<T>* n = new Nodo<T>;
		n->valor = x;
		n->next = *p;
		*p = n;
		A.unlock();
		return 1;
	}
}

template <class T>
bool CList<T>::Delete(T x) {
	Nodo<T>** p;
	if (!Search(x, p)) {
		return 0;
	}
	else {
		A.lock();
		Nodo<T>* t = *p;
		*p = (*p)->next;
		delete t;
		A.unlock();
		return 1;
	}
}

template <class T>
void CList<T>::print() {
	A.lock();
	for (Nodo<T>* a = head; a != nullptr; a = a->next) {
		cout << a->valor << " -> ";
	}
	A.unlock();
}

template <class T>
struct Insert {
	int min;
	int max;
	CList<T>* ptr;
	Insert(T min, T max, CList<T>& ptr_) {
		this->min = min;
		this->max = max;
		ptr = &ptr_;
	}
	int get_random(int low, int high) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distribution(low, high);
		return distribution(gen);
	}
	void operator()(int operaciones) {
		for (unsigned int i = 0; i < operaciones; i++) {
			ptr->Insert(get_random(min, max));
		}
	}
};

template <class T>
struct Delete {
	int min, max;
	CList<T>* ptr;
	Delete(T min, T max, CList<T>& ptr_) {
		this->min = min;
		this->max = max;
		ptr = &ptr_;
	}
	int get_random(int low, int high) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distribution(low, high);
		return distribution(gen);
	}
	void operator()(int operaciones) {
		for (unsigned int i = 0; i < operaciones; i++) {
			ptr->Delete(get_random(min, max));
		}
	}
};

int main() {
	CList<int> l;
	std::thread* oper[4];
	for (int i = 0; i < 2; i++) {
		oper[i] = new thread{ Insert<int>(1,100,l),100 };
	}
	for (int i = 2; i < 4; i++) {
		oper[i] = new thread{ Delete<int>(1,100,l),100 };
	}
	for (int i = 0; i < 4; i++) {
		oper[i]->join();
	}
	l.print();
}