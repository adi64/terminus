#pragma once

#include <map>
#include <string>
#include <memory>
#include <vector>

#include "geometry.h"
#include "material.h"
#include "program.h"

namespace terminus
{

/*!
 * \brief The ResourceManager class loads geometry, material and shader progam data during initialization.
 *
 * Loaded data is stored in three corresponding member variables of type "std::map<std::string, std::shared_ptr<std::unique_ptr< #data# >>>".
 * Having two pointers secures to have valid data accessable at all times. By the time data is updated during runtime,
 * classes with references to stored data will automatically load the recent data.
 * Implemented as a singelton, accessibility for all classes is safely ensured.
 */
class ResourceManager
{
public:
    /*!
     * \brief Returns the singelton instance of the SoundManager class
     */
    static ResourceManager* getInstance();

protected:
    static ResourceManager* m_instance;

protected:
    static std::string entityName(const std::string path, const std::string name);
    static std::string entityName(const std::string path);

public:
    virtual ~ResourceManager();

    virtual void loadResources();

    virtual std::shared_ptr<std::unique_ptr<Geometry>> getGeometry(std::string name);
    virtual std::shared_ptr<std::unique_ptr<Material>> getMaterial(std::string name);
    virtual std::shared_ptr<std::unique_ptr<Program>> getProgram(std::string name);

protected:
    ResourceManager();

    virtual void loadObj(std::string path);
    virtual void loadObjGenerateAdd(std::vector<QVector3D> & positions,
                                     std::vector<QVector3D> & texCoords,
                                     std::vector<QVector3D> & normals,
                                     std::vector<IndexTriple> & indexTriples,
                                     std::string name);
    virtual void loadMtl(std::string path);
    virtual void loadProgram(std::string path);

    virtual void putGeometry(std::string name, Geometry * geometry);
    virtual void putMaterial(std::string name, Material * material);
    virtual void putProgram(std::string name, Program * program);

protected:
    std::map<std::string, std::shared_ptr<std::unique_ptr<Geometry>>> m_geometryStorage;
    std::map<std::string, std::shared_ptr<std::unique_ptr<Material>>> m_materialStorage;
    std::map<std::string, std::shared_ptr<std::unique_ptr<Program>>> m_programStorage;
};

} // terminus
