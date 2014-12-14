#include "resourcemanager.h"

#include "geometry.h"

#include <limits.h>

#include <regex>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>

#include <QDebug> //TODO remove in the end

namespace terminus
{

ResourceManager * ResourceManager::m_instance = nullptr;

ResourceManager * ResourceManager::getInstance()
{
    if(m_instance == nullptr)
    {
        m_instance = new ResourceManager();
    }

    return m_instance;
}

std::string ResourceManager::constructEntityName(std::string path, std::string name)
{
    std::string entityName(path);

    size_t index = entityName.find_last_of("\\/");
    if (std::string::npos != index)
    {
        entityName.erase(0, index + 1);
    }

    index = entityName.rfind('.');
    if (std::string::npos != index)
    {
        entityName.erase(index);
    }

    entityName.append("_");
    entityName.append(name);

    return entityName;
}

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::loadResources(){
    loadObj(std::string("data/base.obj"));
    //TODO list files to load here
}

void ResourceManager::loadObj(std::string path)
{
    std::vector<QVector3D> positions;
    std::vector<QVector3D> texCoords;
    std::vector<QVector3D> normals;
    std::vector<IndexTriple> indexTriples;

    std::string objectName;

    std::ifstream objFile(path);
    std::string line;

    while(std::getline(objFile, line))
    {
        std::string lineHeader;
        std::stringstream lineStream(line);
        lineStream >> lineHeader;

        if(lineHeader == "v")
        {
            float x,y,z;
            lineStream >> x >> y >> z;
            QVector3D position(x, y, z);
            positions.push_back(position);
        }
        else if(lineHeader == "vt")
        {
            float x,y,z;
            lineStream >> x >> y >> z;
            QVector3D texCoord(x, y, z);
            texCoords.push_back(texCoord);
        }
        else if(lineHeader == "vn")
        {
            float x,y,z;
            lineStream >> x >> y >> z;
            QVector3D normal(x, y, z);
            normals.push_back(normal);
        }
        else if(lineHeader == "f")
        {
            std::regex indexPattern("([0-9]+)(?:[/]([0-9]*)(?:[/]([0-9]*))?)?");
            std::string indexSpec[3];
            lineStream >> indexSpec[0] >> indexSpec[1] >> indexSpec[2];
            for(int i = 0; i < 3; i++)
            {
                std::smatch match;
                if(!std::regex_match(indexSpec[i], match, indexPattern))
                {
                    qDebug() << "FATAL: incorrect *.obj format";
                    return;
                }
                indexTriples.push_back(IndexTriple(match[1], match[2], match[3]));
            }
        }
        else if(lineHeader == "g" || lineHeader == "o")
        {
            if(objectName.length() > 0)
            {
                //generate and save the geometry that was defined up to this line
                loadObjGenerateAdd(positions, texCoords, normals, indexTriples, constructEntityName(path, objectName));
                indexTriples.clear();
            }
            lineStream >> objectName;
        }
    }
    if(objectName.length() > 0)
    {
        loadObjGenerateAdd(positions, texCoords, normals, indexTriples, constructEntityName(path, objectName));
    }
}

void ResourceManager::loadObjGenerateAdd(std::vector<QVector3D> & positions,
                                         std::vector<QVector3D> & texCoords,
                                         std::vector<QVector3D> & normals,
                                         std::vector<IndexTriple> & indexTriples,
                                         std::string name)
{
    std::vector<unsigned short> indexBuffer;
    std::vector<Vertex> vertexBuffer;

    std::map<IndexTriple, unsigned short> indexLookUp;

    for(unsigned int i = 0; i < indexTriples.size() && vertexBuffer.size() < USHRT_MAX; i++)
    {
        if(indexLookUp.count(indexTriples[i]) == 0)
        {
            indexLookUp[indexTriples[i]] = vertexBuffer.size();

            int sP = positions.size(),
                 iP = indexTriples[i].positionIndex() - 1,
                 sT = texCoords.size(),
                 iT = indexTriples[i].textureIndex() - 1,
                 sN = normals.size(),
                 iN = indexTriples[i].normalIndex() - 1;
            //convert relative indices
            iP = (iP < 0)? sP - iP : iP;
            iT = (iT < 0)? sT - iT : iT;
            iN = (iN < 0)? sN - iN : iN;
            //read vectors
            QVector3D vP = (0 <= iP && iP < sP)? positions.at(iP) : QVector3D(),
                      vT = (indexTriples[i].validTexture() && 0 <= iT && iT < sT)? texCoords.at(iT) : QVector3D(),
                      vN = (indexTriples[i].validNormal() && 0 <= iN && iN < sN)? normals.at(iN) : QVector3D();

            Vertex v {{vP.x(), vP.y(),vP.z()}, {vT.x(), vT.y(),vT.z()}, {vN.x(), vN.y(),vN.z()}};
            vertexBuffer.push_back(v);

            indexBuffer.push_back(static_cast<unsigned short>(vertexBuffer.size() - 1));
        }
        else
        {
            indexBuffer.push_back(indexLookUp[indexTriples[i]]);
        }
    }

    putGeometry(name, new Geometry(indexBuffer, vertexBuffer));
}

void ResourceManager::loadMtl(std::string path)
{
    //TODO
}

std::shared_ptr<std::unique_ptr<Geometry>> ResourceManager::getGeometry(std::string name)
{
    return m_geometryStorage[name];
}
std::shared_ptr<std::unique_ptr<Material>> ResourceManager::getMaterial(std::string name)
{
    return m_materialStorage[name];
}
void ResourceManager::putGeometry(std::string name, Geometry * geometry)
{
    if(m_geometryStorage.count(name) == 0)
    {
        m_geometryStorage[name] = std::shared_ptr<std::unique_ptr<Geometry>>(new std::unique_ptr<Geometry>(geometry));
    }
    else
    {
        std::unique_ptr<Geometry> newPtr(geometry);
        (*m_geometryStorage[name]) = std::move(newPtr);
    }
}
void ResourceManager::putMaterial(std::string name, Material * material)
{
    if(m_materialStorage.count(name) == 0)
    {
        m_materialStorage[name] = std::shared_ptr<std::unique_ptr<Material>>(new std::unique_ptr<Material>(material));
    }
    else
    {
        std::unique_ptr<Material> newPtr(material);
        (*m_materialStorage[name]) = std::move(newPtr);
    }
}

} // terminus
