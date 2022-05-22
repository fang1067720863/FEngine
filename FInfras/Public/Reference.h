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

