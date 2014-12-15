#include "material.h"


namespace terminus
{

Material::Material()
{
}

Material::Material(const std::map<std::string,QVector4D> & uniforms)
: m_uniforms(uniforms)
{
}

Material::~Material()
{
}

void Material::setUniforms(Program & program)
{
    for(auto it = m_uniforms.begin(); it != m_uniforms.end(); it++)
    {
        program.setUniform(it->first, it->second);
    }
}

}//namespace terminus
