#include <cstddef>
#include <cassert>
#include "llist.h"

#include <iostream>
LList::Node::Node(const ValueType& value, Node* next)
{
	this->value = value;
	this->next = next;
}

LList::Node::~Node()
{
	// ничего не удаляем, т.к. агрегация
}

void LList::Node::insert_atNext(const ValueType& value)
{
	Node* newNode = new Node(value, this->next);
	this->next = newNode;
}

void LList::Node::erase_atNext()
{
	Node* erase_atNode = this->next;
	Node* newNext = erase_atNode->next;
	delete erase_atNode;
	this->next = newNext;
}

LList::LList()
	: _head(nullptr), _size(0)
{

}

LList::LList(const LList& copyList)
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

LList& LList::operator=(const LList& copyList)//not okay
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
LList::~LList()
{
	while (_size)
		pop_front();
}

ValueType& LList::operator[](const size_t pos) const
{
	return getNode(pos)->value;
}

LList::Node* LList::getNode(const size_t pos) const
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

void LList::insert_at(const size_t pos, const ValueType& value)
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

void LList::insert_atAfterNode(Node* node, const ValueType& value)
{
		node->insert_atNext(value);
}

void LList::push_back(const ValueType& value)
{
	if (_size == 0) {
		push_front(value);
		return;
	}
	insert_at(_size, value);
}

void LList::push_front(const ValueType& value)
{
	_head = new Node(value, _head);
	++_size;
}

void LList::erase_at(const size_t pos)
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

void LList::erase_atNextNode(Node* node)
{
	if (node->next != nullptr)
	{
		node->erase_atNext();
		_size--;
	}
	return;
}

void LList::pop_back()
{
	Node* cur = getNode(_size - 2);
	delete cur->next;
	cur->next = nullptr;
	_size--;
}

void LList::pop_front()
{
	Node* newHead = _head->next;
	_head = newHead;
	_size--;
}

long long int LList::findIndex(const ValueType& value) const
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

LList::Node* LList::findNode(const ValueType& value) const
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

void LList::reverse()
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

LList LList::reverse() const
{
	LList* l = new LList;
	*l = *this;
	l->reverse();

	return *l;
}

LList LList::getReverseList()
{
	LList* l = new LList;
	*l = *this;
	l->reverse();

	return *l;
}

size_t LList::size() const
{
	return _size;
}





