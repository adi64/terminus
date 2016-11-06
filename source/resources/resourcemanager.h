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
 * \brief The ResourceManager class loads geometry, materials and shader progams during initialization and provides access to those resources.
 *
 * Loaded data is stored in three corresponding member variables of type "std::map<std::string, std::shared_ptr<std::unique_ptr< ... >>>".
 * Having two pointers secures to have valid data accessable at all times. By the time data is updated during runtime,
 * classes with references to stored data will automatically load the recent data.
 * Implemented as a singleton, accessibility for all classes is ensured.
 */
class ResourceManager
{
public:
    /*!
     * \brief Returns the singelton instance of the ResourceManager class
     */
    static ResourceManager* getInstance();

protected:
    static ResourceManager* m_instance;

protected:
    /*!
     * \brief Generates a unique resource identifier from the resource's file path and name.
     * \param path
     * \param name
     * \return resource identifier
     */
    static std::string entityName(const std::string & path, const std::string & name);

    /*!
     * \brief Generates a unique resource identifier from the resource's file path.
     * \param path
     * \return resource identifier
     */
    static std::string entityName(const std::string & path);

public:
    virtual ~ResourceManager();

    /*!
     * \brief All external resources the game might use are loaded here.
     */
    virtual void loadResources();

    virtual std::shared_ptr<std::unique_ptr<Geometry>> getGeometry(const std::string & name);
    virtual std::shared_ptr<std::unique_ptr<Material>> getMaterial(const std::string & name);
    virtual std::shared_ptr<std::unique_ptr<Program>> getProgram(const std::string & name);

protected:
    ResourceManager();

    /*!
     * \brief The .obj file specified by "path" will be parsed and stored in the geometry container #m_geometryStorage.
     * \param path
     *
     * The file is parsed line by line, storing data for vertex position (line begins with "v"),
     * texture coordinates (line begins with "t"), normal vectors (line begins with "n") and geometry faces (line begins with "f").
     * A line starting with "o" specifies the object name.
     * The corresponding BBox is generated while parsing the data.
     */
    virtual void loadObj(const std::string & path);

    /*!
     * \brief This method is part of the loading mechanism for .obj files.
     * \param positions
     * \param texCoords
     * \param normals
     * \param indexTriples
     * \param name
     *
     * IndexTriples as specified in the .obj file are converted to a single index and a new Geometry instance is created
     * and added to the geometry container "m_geometryStorage".
     */
    virtual void loadObjGenerateAdd(std::vector<QVector3D> & positions,
                                     std::vector<QVector3D> & texCoords,
                                     std::vector<QVector3D> & normals,
                                     std::vector<IndexTriple> & indexTriples,
                                     std::string name);

    /*!
     * \brief Loads material data from .mtl files.
     * \param path
     *
     * Any number of materials encoded in one .mtl file are loaded and stored in the material container #m_materialStorage.
     */
    virtual void loadMtl(const std::string & path);

    /*!
     * \brief Vertex and fragment shader files (with same base name) are loaded an combined into on program instance from the Program class.
     * \param name
     * \param path
     */
    virtual void loadProgram(const std::string & name, const std::string & path);

    /*!
     * \brief Vertex and fragment shader files are loaded an combined into on program instance from the Program class.
     * \param name
     * \param vertexShaderPath
     * \param fragmentShaderPath
     */
    virtual void loadProgram(const std::string & name, const std::string & vertexShaderPath, const std::string & fragmentShaderPath);

    virtual void putGeometry(const std::string & name, Geometry * geometry);
    virtual void putMaterial(const std::string & name, Material * material);
    virtual void putProgram(const std::string & name, Program * program);

protected:
    std::map<std::string, std::shared_ptr<std::unique_ptr<Geometry>>> m_geometryStorage;
    std::map<std::string, std::shared_ptr<std::unique_ptr<Material>>> m_materialStorage;
    std::map<std::string, std::shared_ptr<std::unique_ptr<Program>>> m_programStorage;
};

} // terminus
