#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

#include "resource_getter.h"
#include "singleton.hpp"
#include <SFML/Graphics.hpp>

template < class T >
class ResourceManager:public Singleton < ResourceManager<T> >
{
    ResourceManager();
    ~ResourceManager();
    friend Singleton < ResourceManager > ;

    std::vector <T> resource;
    std::map < std::string, size_t > ids;

public:
    T & push( std::string id);

    size_t get_index(std::string id);
    T & get_resource(size_t index);
    T & get_resource(std::string id);

    void free_resource();
};

template <class T>
ResourceManager<T>::ResourceManager(){}

template <class T>
ResourceManager<T>::~ResourceManager(){}

template<class T>
T &ResourceManager<T>::push(std::string id)
{
    if(ids.find(id) != ids.end()) throw std::runtime_error("id alredy existing <" + id + ">");

    ids[id] = resource.size();
    resource.resize(resource.size()+1);

    return resource.back();
}

template<class T>
size_t ResourceManager<T>::get_index(std::string id)
{
    if(ids.find(id) == ids.end()) throw std::runtime_error("id <" + id + "> doesnt exist");
    return ids.at(id);
}

template<class T>
T &ResourceManager<T>::get_resource(size_t index)
{
    if(index >= resource.size())
        throw std::runtime_error("index <" +
                                 std::to_string(index) +
                                 "> not in range of resource <" +
                                 std::to_string(resource.size()) + ">");

    return resource.at(index);
}

template<class T>
T &ResourceManager<T>::get_resource(std::string id)
{
    if(ids.find(id) == ids.end())
        throw std::runtime_error("id <" + id + "> doesnt exist");

    return resource.at(ids.at(id));
}

template<class T>
void ResourceManager<T>::free_resource()
{
    resource.clear();
}

#endif
