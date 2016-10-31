#include "resourcemanager.h"

#include <algorithm>
#include <cassert>
#include <exception>
#include <limits.h>
#include <map>
#include <math.h>
#include <regex>
#include <sstream>
#include <string>

#include <QFile>
#include <QString>
#include <QDebug>

#include "geometry.h"

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

std::string ResourceManager::entityName(const std::string & path, const std::string & name)
{
    std::string ename = entityName(path);
    ename.append("_");
    ename.append(name);
    return ename;
}

std::string ResourceManager::entityName(const std::string & path)
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
    loadObj(std::string(":/data/terrain.obj"));
    loadObj(std::string(":/data/engine.obj"));
    loadObj(std::string(":/data/weapon.obj"));
    loadObj(std::string(":/data/repair.obj"));

    loadMtl(std::string(":/data/base.mtl"));

    loadProgram(std::string("basicShader"), std::string(":/data/basicShader"), std::string(":/data/to_gbuffer"));
    loadProgram(std::string("terrain"), std::string(":/data/terrain"), std::string(":/data/to_gbuffer"));
    loadProgram(std::string("pp_passthrough"), std::string(":/data/postprocessing"), std::string(":/data/postprocessing_passthrough"));
    loadProgram(std::string("pp_compose"), std::string(":/data/postprocessing"), std::string(":/data/postprocessing_compose"));
    loadProgram(std::string("pp_vignette"), std::string(":/data/postprocessing"), std::string(":/data/postprocessing_vignette"));
}

std::shared_ptr<std::unique_ptr<Geometry>> ResourceManager::getGeometry(const std::string & name)
{
    try
    {
        return m_geometryStorage.at(name);
    }
    catch(const std::exception & ex)
    {
        qCritical() << "Cannot get geometry '" << QString::fromStdString(name) << "': " << QString::fromStdString(ex.what());
        throw(ex);
    }
}

std::shared_ptr<std::unique_ptr<Material>> ResourceManager::getMaterial(const std::string & name)
{
    try
    {
        return m_materialStorage.at(name);
    }
    catch(const std::exception & ex)
    {
        qCritical() << "Cannot get material '" << QString::fromStdString(name) << "': " << QString::fromStdString(ex.what());
        throw(ex);
    }
}

std::shared_ptr<std::unique_ptr<Program>> ResourceManager::getProgram(const std::string & name)
{
    try
    {
        return m_programStorage.at(name);
    }
    catch(const std::exception & ex)
    {
        qCritical() << "Cannot get program '" << QString::fromStdString(name) << "': " << QString::fromStdString(ex.what());
        throw(ex);
    }
}

void ResourceManager::loadObj(const std::string & path)
{
    std::vector<QVector3D> positions;
    std::vector<QVector3D> texCoords;
    std::vector<QVector3D> normals;
    std::vector<IndexTriple> indexTriples;
    std::string objectName;
    QFile objFile(path.c_str());
    if (!objFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "*.obj file can not be opened";
        return;
    }
    QTextStream objStream(&objFile);

    std::regex indexPattern("([-]?[0-9]+)(?:[/]([-]?[0-9]*)(?:[/]([-]?[0-9]*))?)?");

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

            std::string indexSpec[3];
            lineStream >> indexSpec[0] >> indexSpec[1] >> indexSpec[2];
            for(int i = 0; i < 3; i++)
            {
                std::smatch match;
                if(!std::regex_match(indexSpec[i], match, indexPattern))
                {
                    qDebug() << "incorrect *.obj format";
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
    QVector3D bBoxMin;
    QVector3D bBoxMax;
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
            bBoxMin.setX(fmin(vP.x(), bBoxMin.x()));
            bBoxMin.setY(fmin(vP.y(), bBoxMin.y()));
            bBoxMin.setZ(fmin(vP.z(), bBoxMin.z()));
            bBoxMax.setX(fmax(vP.x(), bBoxMax.x()));
            bBoxMax.setY(fmax(vP.y(), bBoxMax.y()));
            bBoxMax.setZ(fmax(vP.z(), bBoxMax.z()));
            Vertex v {{vP.x(), vP.y(),vP.z()}, {vT.x(), vT.y(),vT.z()}, {vN.x(), vN.y(),vN.z()}};
            vertexBuffer.push_back(v);
            indexBuffer.push_back(static_cast<unsigned short>(vertexBuffer.size() - 1));
        }
        else
        {
            indexBuffer.push_back(indexLookUp[indexTriples[i]]);
        }
    }
    putGeometry(name, new Geometry(indexBuffer, vertexBuffer, bBoxMin, bBoxMax));
}

void ResourceManager::loadMtl(const std::string & path)
{
    std::map<std::string, QVector4D> uniforms;
    std::string objectName;
    QFile mtlFile(path.c_str());
    if (!mtlFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "*.mtl file can not be opened";
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

void ResourceManager::loadProgram(const std::string & name, const std::string & path)
{
    return loadProgram(name, path, path);
}

void ResourceManager::loadProgram(const std::string & name, const std::string & vertexShaderPath, const std::string & fragmentShaderPath)
{
    std::string vertSourcPath(vertexShaderPath);
    vertSourcPath.append(".vert");
    QFile vertSourceFile(vertSourcPath.c_str());
    if (!vertSourceFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "*.vert file can not be opened";
        return;
    }
    QByteArray vertSource = vertSourceFile.readAll();

    std::string fragSourcPath(fragmentShaderPath);
    fragSourcPath.append(".frag");
    QFile fragSourceFile(fragSourcPath.c_str());
    if (!fragSourceFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "*.frag file can not be opened";
        return;
    }
    QByteArray fragSource = fragSourceFile.readAll();

    putProgram(name, new Program(std::vector<char>(vertSource.begin(), vertSource.end()),
                                             std::vector<char>(fragSource.begin(), fragSource.end())));
}

void ResourceManager::putGeometry(const std::string & name, Geometry * geometry)
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

void ResourceManager::putMaterial(const std::string & name, Material * material)
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

void ResourceManager::putProgram(const std::string & name, Program * program)
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
