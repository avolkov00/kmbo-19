	#pragma once

// потом поменяем на шаблоны
using ValueType = double;
#include <cstddef>
#include <cassert>
#include <iostream>
#include <iostream>
template <class ValueType>
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

template<class ValueType>
LList<ValueType>::Node::Node(const ValueType& value, Node* next)
{
	this->value = value;
	this->next = next;
}

template<class ValueType>
LList<ValueType>::Node::~Node()
{
	// ничего не удаляем, т.к. агрегация
}
template<class ValueType>
void LList<ValueType>::Node::insert_atNext(const ValueType& value)
{
	Node* newNode = new Node(value, this->next);
	this->next = newNode;
}

template<class ValueType>
void LList<ValueType>::Node::erase_atNext()
{
	Node* erase_atNode = this->next;
	Node* newNext = erase_atNode->next;
	delete erase_atNode;
	this->next = newNext;
}

template<class ValueType>
LList<ValueType>::LList()
	: _head(nullptr), _size(0)
{

}

template<class ValueType>
LList<ValueType>::LList(const LList& copyList)
{
	this->_size = copyList._size;
	this->_head = new Node(copyList._head->value);
	Node* curCopy = copyList._head;
	Node* cur = this->_head;

	while (curCopy->next != nullptr)
	{
		cur->next = new Node(curCopy->next->value);
		cur = cur->next;
		curCopy = curCopy->next;
	}
}
template<class ValueType>
LList<ValueType>& LList<ValueType>::operator=(const LList& copyList)//not okay
{
	if (this == &copyList) {
		return *this;
	}
	LList bufList(copyList);
	this->_size = bufList._size;
	this->_head = new Node(bufList._head->value);
	Node* cur = _head;
	Node* curCopy = bufList._head;

	while (curCopy->next != nullptr)
	{
		cur->next = new Node(curCopy->next->value);
		cur = cur->next;
		curCopy = curCopy->next;
	}

	return *this;
}
#if ENABLE_MOVE_SEMANTIC
LList::LList(LList&& moveList) noexcept
{
	this->_size = moveList._size;
	this->_head = moveList._head;

	moveList._size = 0;
	moveList._head = nullptr;
}

LList& LList::operator=(LList&& moveList) noexcept
{
	if (this == &moveList) {
		return *this;
	}
	forceNodeDelete(_head);
	this->_size = moveList._size;
	this->_head = moveList._head;

	moveList._size = 0;
	moveList._head = nullptr;

	return *this;
}
#endif
template<class ValueType>
LList<ValueType>::~LList()
{
	while (_size)
		pop_front();
}
template<class ValueType>
ValueType& LList<ValueType>::operator[](const size_t pos) const
{
	return getNode(pos)->value;
}

template<class ValueType>
typename LList<ValueType>::Node* LList<ValueType>::getNode(const size_t pos) const
{
	if (pos < 0) {
		assert(pos < 0);
	}
	else if (pos >= this->_size) {
		assert(pos >= this->_size);
	}

	Node* bufNode = this->_head;
	for (int i = 0; i < pos; ++i) {
		bufNode = bufNode->next;
	}

	return bufNode;
}
template<class ValueType>
void LList<ValueType>::insert_at(const size_t pos, const ValueType& value)
{
	if (pos < 0) {
		assert(pos < 0);
	}
	else if (pos > this->_size) {
		assert(pos > this->_size);
	}

	if (pos == 0) {
		push_front(value);
	}
	else {
		Node* bufNode = this->_head;
		for (size_t i = 0; i < pos - 1; ++i) {
			bufNode = bufNode->next;
		}
		bufNode->insert_atNext(value);
		++_size;
	}
}
template<class ValueType>
void LList<ValueType>::insert_atAfterNode(Node* node, const ValueType& value)
{
	node->insert_atNext(value);
}

template<class ValueType>
void LList<ValueType>::push_back(const ValueType& value)
{
	if (_size == 0) {
		push_front(value);
		return;
	}
	insert_at(_size, value);
}

template<class ValueType>
void LList<ValueType>::push_front(const ValueType& value)
{
	_head = new Node(value, _head);
	++_size;
}

template<class ValueType>
void LList<ValueType>::erase_at(const size_t pos)
{
	if (pos > _size - 1)
	{
		return;
	}

	if (pos == 0)
	{
		pop_front();
		return;
	}

	if (pos == _size - 1)
	{
		pop_back();
		return;
	}

	Node* cur = getNode(pos - 1);
	cur->erase_atNext();
	_size--;
	return;
}

template<class ValueType>
void LList<ValueType>::erase_atNextNode(Node* node)
{
	if (node->next != nullptr)
	{
		node->erase_atNext();
		_size--;
	}
	return;
}

template<class ValueType>
void LList<ValueType>::pop_back()
{
	Node* cur = getNode(_size - 2);
	delete cur->next;
	cur->next = nullptr;
	_size--;
}

template<class ValueType>
void LList<ValueType>::pop_front()
{
	Node* newHead = _head->next;
	_head = newHead;
	_size--;
}
template<class ValueType>

long long int LList<ValueType>::findIndex(const ValueType& value) const
{
	Node* curent = _head;
	for (long long int i = 0; i < _size; i++)
	{
		if (curent->value == value)
			return i;
		curent = curent->next;
	}
	return -1;
}

template<class ValueType>
typename LList<ValueType>::Node* LList<ValueType>::findNode(const ValueType& value) const
{
	Node* curent = _head;
	for (long long int i = 0; i < _size; i++)
	{
		if (curent->value == value)
			return curent;
		curent = curent->next;
	}
	return nullptr;
}

template<class ValueType>
void LList<ValueType>::reverse()
{
	Node* prev = NULL;
	Node* cur = _head;
	Node* next = _head->next;
	while (next != NULL)
	{
		cur->next = prev;
		prev = cur;
		cur = next;
		next = cur->next;
	}
	cur->next = prev;
	_head = cur;

	return;

}
template<class ValueType>
LList<ValueType> LList<ValueType>::reverse() const
{
	LList* l = new LList;
	*l = *this;
	l->reverse();

	return *l;
}

template<class ValueType>
LList<ValueType> LList<ValueType>::getReverseList()
{
	LList* l = new LList;
	*l = *this;
	l->reverse();

	return *l;
}

template<class ValueType>
size_t LList<ValueType>::size() const
{
	return _size;
}