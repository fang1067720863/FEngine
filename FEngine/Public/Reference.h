#pragma once
#pragma once

#include<atomic>
#include<iostream>

class FReference
{
public:
	std::atomic<std::size_t> ref_count;

	// By default the reference count is initialized to 1
	explicit FReference(std::size_t count = 0)
		: ref_count(count) {}

	virtual ~FReference(){}

	void add_ref(int count = 1)
	{
		ref_count.fetch_add(count, std::memory_order_relaxed);
		std::cout << "current count" << ref_count << std::endl;
	}
	void remove_ref(int count = 1)
	{
		//ref_count.fetch_sub(count, std::memory_order_release);
		std::cout << "remove op current count" << ref_count << std::endl;
		if (ref_count.fetch_sub(count, std::memory_order_release) == count) {
			std::atomic_thread_fence(std::memory_order_acquire);
			delete this;
		}
	}
	void add_ref_unlocked()
	{
		ref_count.store(ref_count.load(std::memory_order_relaxed) + 1, std::memory_order_relaxed);
	}
	bool is_unique_ref(std::memory_order order)
	{
		return ref_count.load(order) == 1;
	}
};
template<typename T>
class FPtr {
	T* p;

public:
	// Note that this doesn't increment the reference count, instead it takes
	// ownership of a pointer which you already own a reference to.
	FPtr(T* t)
		: p(t) {
		std::cout << "io defalut Ptr" << std::endl;
		if (p)
			p->add_ref();
	}
	FPtr()
		: p(nullptr) {
		std::cout << "defalut Ptr" << std::endl;
	}
	FPtr(std::nullptr_t)
		: p(nullptr) {
		std::cout << "null default Ptr" << std::endl;
	}
	FPtr(const FPtr& other)
		: p(other.p)
	{
		std::cout << "copy ctorPtr" << std::endl;
		if (p)
			p->add_ref();
	}


	FPtr(FPtr&& other)
		: p(other.p)
	{
		// ¼Ì³ÐÔ­ÏÈµÄrefcount
		std::cout << "move ctor Ptr" << std::endl;
		other.p = nullptr;
	}
	FPtr& operator=(std::nullptr_t)
	{
		std::cout << "null Ptr" << std::endl;
		if (p)
			p->remove_ref();
		p = nullptr;
		return *this;
	}
	FPtr& operator=(const FPtr& other)
	{
		//std::cout << "Ptr copy assgin" << std::endl;
		if (p) {
			p->remove_ref();
			p = nullptr;
		}
		p = other.p;
		if (p)
			p->add_ref();
		return *this;
	}
	inline FPtr& operator=(T* ptr)
	{
		std::cout << "Ptr orign ptr assgin" << std::endl;
		if (p == ptr) return *this;
		if (p)
		{
			p->remove_ref();
			p = nullptr;
		}
		p = ptr;
		if (p)
			p->add_ref();
		return *this;

	}
	FPtr& operator=(FPtr&& other)
	{
		p = other.p;
		//std::cout << "Ptr move assgin" << std::endl;
		other.p = nullptr;
	}
	~FPtr()
	{
		//std::cout << "Ptr dtor"  << std::endl;
		if (p)
			p->remove_ref();
	}

	T* get() const
	{
		return p;
	}

	T& operator*() const
	{
		return *p;
	}
	T* operator->() const
	{
		return p;
	}

	T* release()
	{
		T* out = p;
		p = nullptr;
		return out;
	}

	explicit operator bool() const
	{
		return p != nullptr;
	}
	friend bool operator==(const FPtr& a, const FPtr& b)
	{
		return a.p == b.p;
	}
	friend bool operator!=(const FPtr& a, const FPtr& b)
	{
		return a.p != b.p;
	}
	friend bool operator==(const FPtr& a, std::nullptr_t)
	{
		return a.p == nullptr;
	}
	friend bool operator!=(const FPtr& a, std::nullptr_t)
	{
		return a.p != nullptr;
	}
	//friend bool operator==(std::nullptr_t, const ref_count_ptr& a)
	//{
	//	return a.p == nullptr;
	//}
	//friend bool operator!=(std::nullptr_t, const ref_count_ptr& a)
	//{
	//	return a.p != nullptr;
	//}
};

template<typename T>
using Ptr = FPtr<T>;
class FObject : public FReference
{
public:
	inline FObject():FReference() {}
};




