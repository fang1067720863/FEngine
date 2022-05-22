#pragma once
#pragma once

#include<atomic>

class FReference
{
public:
	std::atomic<int> ref_count;

	// By default the reference count is initialized to 1
	explicit FReference(std::size_t count = 1)
		: ref_count(count) {}

	void add_ref(int count = 1)
	{
		ref_count.fetch_add(count, std::memory_order_relaxed);
	}
	void remove_ref(int count = 1)
	{
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
		if (p)
			p->add_ref();
	}
	FPtr()
		: p(nullptr) {}
	FPtr(std::nullptr_t)
		: p(nullptr) {}
	FPtr(const FPtr& other)
		: p(other.p)
	{
		if (p)
			p->add_ref();
	}


	FPtr(FPtr&& other)
		: p(other.p)
	{
		other.p = nullptr;
	}
	FPtr& operator=(std::nullptr_t)
	{
		if (p)
			p->remove_ref();
		p = nullptr;
		return *this;
	}
	FPtr& operator=(const FPtr& other)
	{
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
		if (p == ptr) return *this;
		if (p)
		{
			p->remove_ref();
			p = nullptr;
		}
		p = ptr;
		return *this;

	}
	FPtr& operator=(FPtr&& other)
	{
		if (p) {
			p->remove_ref();
			p = nullptr;
		}
		p = other.p;
		other.p = nullptr;
		return *this;
	}


	~FPtr()
	{
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
	friend bool operator==(std::nullptr_t, const ref_count_ptr& a)
	{
		return a.p == nullptr;
	}
	friend bool operator!=(std::nullptr_t, const ref_count_ptr& a)
	{
		return a.p != nullptr;
	}
};

template<typename T>
using Ptr = FPtr<T>;
class FObject : public FReference
{
public:
	inline FObject():FReference() {}
};




