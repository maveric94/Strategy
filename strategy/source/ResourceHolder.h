#ifndef RESOURCEHOLDER_H
#define RESOURCEHOLDER_H

#include <map>
#include <string>
#include <memory>

template <typename Resource, typename Identifier>
class ResourceHolder
{
public:
	void load(Identifier id, const std::string& filename);

	Resource& get(Identifier id);
	const Resource& get(Identifier id) const;

private:
	void insertResource(Identifier id, std::unique_ptr<Resource> resource);

	std::map<Identifier, std::unique_ptr<Resource>>	mResourceMap;
};

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
	std::unique_ptr<Resource> resource(new Resource());
	resource->loadFromFile(filename);
	insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id)
{
	auto found = mResourceMap.find(id);
	return *found->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
	auto found = mResourceMap.find(id);
	return *found->second;
}

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insertResource(Identifier id, std::unique_ptr<Resource> resource) 
{
	mResourceMap.insert(std::make_pair(id, std::move(resource)));
}

#endif