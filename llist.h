#pragma once

// потом поменяем на шаблоны
using ValueType = double;
#include <cstddef>
#include <cassert>
#include <iostream>

class LList
{
	// класс узла списка
	// по своей сути, может содержать любые данные,
	// можно реализовать и ассоциативный массив, просто добавив 
	// поле с ключем в узел и, с учетом этого, поменять методы LList 
	// (доступ по ключу, поиск по ключу и т.д.)
	struct Node {
		Node(const ValueType& value, Node* next = nullptr);
		~Node();

		void insert_atNext(const ValueType& value);
		void erase_atNext();

		ValueType value;
		Node* next;
	};

public:
	////
	LList();
	LList(const LList& copyList);
	LList& operator=(const LList& copyList);

	LList(LList&& moveList) noexcept;
	LList& operator=(LList&& moveList) noexcept;

	~LList();
	////

	// доступ к значению элемента по индексу
	ValueType& operator[](const size_t pos) const;
	// доступ к узлу по индексу
	LList::Node* getNode(const size_t pos) const;
	
	// вставка элемента по индексу, сначала ищем, куда вставлять (О(n)), потом вставляем (O(1))
	void insert_at(const size_t pos, const ValueType& value);
	// вставка элемента после узла, (O(1))
	static void insert_atAfterNode(Node* node, const ValueType& value);
	// вставка в конец (О(n))
	void push_back(const ValueType& value);
	// вставка в начало (О(1))
	void push_front(const ValueType& value);

	// удаление
	void erase_at(const size_t pos);
	void erase_atNextNode(Node* node);
	void pop_front();
	void pop_back();
	
	// поиск, О(n)
	long long int findIndex(const ValueType& value) const;
	Node* findNode(const ValueType& value) const;

	// разворот списка
	void reverse();						// изменение текущего списка
	LList reverse() const;			// полчение нового списка (для константных объектов)
	LList getReverseList() ;	// чтобы неконстантный объект тоже мог возвращать новый развернутый список

	size_t size() const;
private:
	Node*	_head;
	size_t	_size;

	void forceNodeDelete(Node* node);
};

