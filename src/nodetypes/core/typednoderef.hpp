#pragma once

#include "noderef.hpp"

namespace StardustXRServer {

template <class T>
class TypedNodeRef : public NodeRef {
public:
	TypedNodeRef() : NodeRef(nullptr) {}
	TypedNodeRef(T *node) : NodeRef(node) {}
	TypedNodeRef(const TypedNodeRef &other) {
		this->id = other.id;
	}

	NodeRef &operator=(T *node) {
		this->id = node ? node->id : 0;
	}
	operator bool() const {
		return ptr();
	}

	T* operator ->() const {
		return ptr();
	}

	T *ptr() const {
		return NodeRef::ptr<T>();
	}
};

}
