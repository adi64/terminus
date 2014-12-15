#pragma once

#include <map>

#include <QVector4D>

#include "program.h"

namespace terminus
{

class Material
{
public:
    Material();
    Material(const std::map<std::string,QVector4D> & uniforms);
    virtual ~Material();

    virtual void setUniforms(Program & program);
protected:
    std::map<std::string,QVector4D> m_uniforms;
};

}//namespace terminus
