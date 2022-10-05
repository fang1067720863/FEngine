#pragma once
#include<map>
#include<unordered_map>


template<class T, class S, class SubClass>
class ResourceFactory
{
public:
	typedef T Resource;
	typedef S SLOT;
	typedef std::unordered_map<SLOT, Resource> ResouceMap;

	static SubClass& Instance()
	{
		static SubClass instance;
		return instance;
	}

	virtual Resource GetResource(const SLOT& slot) {

		if (resourceMap.find(slot) != resourceMap.end())
		{
			return resourceMap.at(slot);
		}
		return Resource{};
	}

	virtual bool AddResouce(const SLOT& slot, Resource resource)
	{
		if (resourceMap.find(slot) != resourceMap.end())
		{
			return false;
		}
		resourceMap[slot] = resource;
		return true;
	}

protected:
	ResouceMap resourceMap;
	ResourceFactory() = default;
	virtual ~ResourceFactory() = default;
	

};

template<class T, class SubClass>
class UintResourceFactory : public ResourceFactory< T, uint32_t, SubClass>
{
public:
	typedef T Resource;
	typedef int32_t SLOT;

	SLOT CreateResouce(Resource resource)
	{
		guid++;
		this->resourceMap[guid] = resource;
		return guid;
	}
protected:
	UintResourceFactory() :ResourceFactory<T, uint32_t, SubClass>() {};
	SLOT guid;
};