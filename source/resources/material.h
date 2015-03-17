#pragma once

#include <map>

#include <QVector4D>

#include "program.h"

namespace terminus
{

/*!
 * \brief The Material class wraps loading uniform data used as material coefficients.
 */
class Material
{
public:
    Material();
    Material(const std::map<std::string, QVector4D> & uniforms);
    virtual ~Material();

    /*!
     * \brief Provides all stored materials as a uniform for the given program.
     * \param program
     *
     * All stored uniforms are send to the program, which itself is checking, whether the provided uniform is needed.
     */
    virtual void setUniforms(Program & program);

protected:
    std::map<std::string,QVector4D> m_uniforms;
};

}//namespace terminus
