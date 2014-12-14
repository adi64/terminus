#pragma once

#include <map>
#include <string>
#include <memory>
#include <vector>

#include "geometry.h"
#include "material.h"

namespace terminus
{

class ResourceManager
{
public:
    static ResourceManager* getInstance();
protected:
    static ResourceManager* m_instance;

protected:
    static std::string constructEntityName(const std::string path, const std::string name);

public:
    virtual ~ResourceManager();

    virtual void loadResources();

    virtual std::shared_ptr<std::unique_ptr<Geometry>> getGeometry(std::string name);
    virtual std::shared_ptr<std::unique_ptr<Material>> getMaterial(std::string name);

protected:
    ResourceManager();
    virtual void putGeometry(std::string name, Geometry * geometry);
    virtual void loadObj(std::string path);
    virtual void loadObjGenerateAdd(std::vector<QVector3D> & positions,
                                     std::vector<QVector3D> & texCoords,
                                     std::vector<QVector3D> & normals,
                                     std::vector<IndexTriple> & indexTriples,
                                     std::string name);

    virtual void putMaterial(std::string name, Material * material);
    virtual void loadMtl(std::string path);

protected:
    std::map<std::string, std::shared_ptr<std::unique_ptr<Geometry>>> m_geometryStorage;
    std::map<std::string, std::shared_ptr<std::unique_ptr<Material>>> m_materialStorage;
};

} // terminus
