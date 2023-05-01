#pragma once

#include <types.hpp>

namespace EXOS::Utils {
	template<typename T>
	class Queue {
	private:
		T *data;
		uint32_t front = 0;
		uint32_t rear = 0;
		size_t maxSize;
	public:
		Queue(size_t n) {
		  this->maxSize = n;
		  this->data = new T[n];
	  }

		bool isFull() {
			return ((rear + 1) % maxSize == front);
		}

		bool isEmpty() {
			return (front == rear);
		}

		void push(T item) {
			if (!isFull()) {
				rear = (rear + 1) % maxSize;
				data[rear] = item;
			}
		}

		T pop() {
			if (isEmpty()) {
				return (T)0;
			} else {
				front = (front + 1) % maxSize;
				return data[front];
			}
		}

		size_t size() {
		  return (front >= rear ? front - rear : rear - front);
		}

		size_t capacity() {
			return this->maxSize;
		}
	};

  template<typename T>
	class RefQueue : public Queue<T&> {
		 RefQueue(size_t size) : Queue<T&>(size) {

		 }
	};
}