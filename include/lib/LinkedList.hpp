#pragma once

#include <lib/AbstractList.hpp>

namespace EXOS::Utils {
	template<typename T>
	class Node {
	public:
		T data;
		Node<T> *next = nullptr;

		Node(T value) {
			data = value;
		}

		~Node() {
			delete next;
		}
	};

	template<typename T>
	class LinkedList : public _AbstractList<T> {
	private:
		Node<T> *head = nullptr;
		size_t size = 0;
	public:
		~LinkedList() {
			delete head;
		}

		bool isEmpty() {
			return (head == nullptr);
		}
		
		uint32_t indexOf(T obj) {
			Node<T> *current = head;
			uint32_t i = 0;
			while(current != nullptr) {
				if (current->data == obj) {
					return i;
				}
				current = current->next;
				++i;
			}
			return 0xffffffff;
		}

		bool contains(T obj) {
			return (indexOf(obj) != 0xffffffff);
		}

		void add(T obj) {
			Node<T> *newNode = new Node<T>(obj);
			if (head == nullptr) {
				head = newNode;
			} else {
				Node<T> *current = head;
				while(current->next != nullptr) {
					current = current->next;
				}
				current->next = newNode;
			}
			++this->size;
		}

		void set(uint32_t index, T obj) {
			Node<T> *current = head;
			uint32_t i = 0;
			while(current != nullptr && i < index) {
				current = current->next;
				i++;
			}
			if (current != nullptr) {
				current->data = obj;
			} else return;
		}

		T get(uint32_t index) {
			Node<T> *current = head;
			uint32_t i = 0;
			while(current != nullptr && i < index) {
				current = current->next;
				i++;
			}
			if (current != nullptr) {
				return current->data;
			} else return (T)0;
		}

		void remove(uint32_t index) {
			Node<T> *current = head;
			Node<T> *prev = nullptr;
			uint32_t i = 0;
			while(current != nullptr && i < index) {
				prev = current;
				current = current->next;
				i++;
			}
			if (current != nullptr) {
				prev->next = current->next;
				delete current;
			}
			--this->size;
		}

		size_t getSize() {
			return size;
		}
	};
}