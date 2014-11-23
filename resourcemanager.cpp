#include "resourcemanager.h"

#include "geometry.h"

//initialize singleton
ResourceManager::m_instance();

ResourceManager::ResourceManager & get()
{
    return & m_instance;
}

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::loadGeometry(std::string name, std::string path)
{

}

std::unique_ptr<Geometry> ResourceManager::getGeometry(std::string name)
{

}
