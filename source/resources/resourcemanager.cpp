#include "resourcemanager.h"

#include "geometry.h"

#include <limits.h>

#include <regex>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>

#include <QFile>
#include <QString>
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

std::string ResourceManager::entityName(std::string path, std::string name)
{
    std::string ename = entityName(path);

    ename.append("_");
    ename.append(name);

    return ename;
}
std::string ResourceManager::entityName(std::string path)
{
    std::string ename(path);

    size_t index = ename.find_last_of("\\/");
    if (std::string::npos != index)
    {
        ename.erase(0, index + 1);
    }

    index = ename.rfind('.');
    if (std::string::npos != index)
    {
        ename.erase(index);
    }

    return ename;
}


ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::loadResources()
{
    loadObj(std::string(":/data/base.obj"));
    loadMtl(std::string(":/data/base.mtl"));
    loadProgram(std::string(":/data/basicShader"));
    loadProgram(std::string(":/data/envmap"));
}

std::shared_ptr<std::unique_ptr<Geometry>> ResourceManager::getGeometry(std::string name)
{
    return m_geometryStorage[name];
}
std::shared_ptr<std::unique_ptr<Material>> ResourceManager::getMaterial(std::string name)
{
    return m_materialStorage[name];
}
std::shared_ptr<std::unique_ptr<Program>> ResourceManager::getProgram(std::string name)
{
    return m_programStorage[name];
}


void ResourceManager::loadObj(std::string path)
{
    std::vector<QVector3D> positions;
    std::vector<QVector3D> texCoords;
    std::vector<QVector3D> normals;
    std::vector<IndexTriple> indexTriples;

    std::string objectName;

    QFile objFile(path.c_str());
    if (!objFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "FATAL: *.obj file can not be opened";
        return;
    }
    QTextStream objStream(&objFile);

    while(!objStream.atEnd())
    {
        std::string lineHeader;
        std::stringstream lineStream(objStream.readLine().toStdString());
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
            std::regex indexPattern("([-]?[0-9]+)(?:[/]([-]?[0-9]*)(?:[/]([-]?[0-9]*))?)?");
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
                loadObjGenerateAdd(positions, texCoords, normals, indexTriples, entityName(path, objectName));
                indexTriples.clear();
            }
            lineStream >> objectName;
        }
    }
    if(objectName.length() > 0)
    {
        loadObjGenerateAdd(positions, texCoords, normals, indexTriples, entityName(path, objectName));
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
                 iP = indexTriples[i].positionIndex(),
                 sT = texCoords.size(),
                 iT = indexTriples[i].textureIndex(),
                 sN = normals.size(),
                 iN = indexTriples[i].normalIndex();
            //convert relative indices
            iP = (iP < 0)? sP + iP : iP - 1;
            iT = (iT < 0)? sT + iT : iT - 1;
            iN = (iN < 0)? sN + iN : iN - 1;
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
    std::map<std::string, QVector4D> uniforms;
    std::string objectName;

    QFile mtlFile(path.c_str());
    if (!mtlFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "FATAL: *.obj file can not be opened";
        return;
    }
    QTextStream mtlStream(&mtlFile);

    while(!mtlStream.atEnd())
    {
        std::string lineHeader;
        std::stringstream lineStream(mtlStream.readLine().toStdString());
        lineStream >> lineHeader;

        if(lineHeader == "uniform"){
            std::string name;
            lineStream >> name;
            float v[] = {0.f, 0.f, 0.f, 0.f};

            for(int i = 0; i < 4; i++)
            {
                lineStream >> v[i];
                if(!lineStream.good())
                {
                    break;
                }
            }
            uniforms[name] = QVector4D(v[0], v[1], v[2], v[3]);
        }
        else if(lineHeader == "newmtl")
        {
            if(objectName.length() > 0)
            {
                putMaterial(entityName(path, objectName), new Material(uniforms));
                uniforms.clear();
            }
            lineStream >> objectName;
        }
    }
    if(objectName.length() > 0)
    {
        putMaterial(entityName(path, objectName), new Material(uniforms));
    }
}

void ResourceManager::loadProgram(std::string path)
{
    std::string vSrc(path);
    std::string fSrc(path);
    vSrc.append(".vert");
    fSrc.append(".frag");
    putProgram(entityName(path), new Program(vSrc, fSrc));
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
void ResourceManager::putProgram(std::string name, Program * program)
{
    if(m_programStorage.count(name) == 0)
    {
        m_programStorage[name] = std::shared_ptr<std::unique_ptr<Program>>(new std::unique_ptr<Program>(program));
    }
    else
    {
        std::unique_ptr<Program> newPtr(program);
        (*m_programStorage[name]) = std::move(newPtr);
    }
}

} // terminus
