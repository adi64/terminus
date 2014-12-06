#pragma once

#include <map>
#include <string>
#include <memory>
#include <vector>

#include "geometry.h"

namespace terminus
{

class ResourceManager
{  
public:
    static ResourceManager* getInstance();
    virtual void storeGeometry(std::string name, std::string path);
    virtual Geometry loadGeometry(std::string path);
    virtual void loadMaterial(std::string path);


    virtual void setGeometry(std::string name, Geometry * geometry);
    virtual std::shared_ptr<std::unique_ptr<Geometry>> getGeometry(std::string name);

protected:
    std::map<std::string, std::shared_ptr<std::unique_ptr<Geometry>>> m_geometryStorage;
    ResourceManager();
    virtual ~ResourceManager();

    static ResourceManager* m_instance;
};

} // terminus
