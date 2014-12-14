#include "indextriple.h"

#include <typeinfo>
#include <QDebug>

namespace terminus
{

IndexTriple::IndexTriple(std::string positionSpec, std::string textureSpec, std::string normalSpec)
: m_validTexture(true)
, m_validNormal(true)
{
    m_positionIndex = std::stoi(positionSpec);

    try
    {
        m_textureIndex = std::stoi(textureSpec);
    }
    catch (const std::invalid_argument& e)
    {
        m_validTexture = false;
        qDebug() << typeid(e).name();
    }

    try
    {
        m_normalIndex = std::stoi(normalSpec);
    }
    catch (const std::invalid_argument& e)
    {
        m_validNormal = false;
        qDebug() << typeid(e).name();
    }
}

IndexTriple::~IndexTriple()
{

}

bool IndexTriple::operator<(const IndexTriple & other) const
{
    return true;
}

bool IndexTriple::operator==(const IndexTriple & other) const
{
    return (positionIndex() == other.positionIndex() &&
            validTexture() == other.validTexture() && (!validTexture() || textureIndex() == other.textureIndex()) &&
            validNormal() == other.validNormal() && (!validNormal() || normalIndex() == other.normalIndex()));
/*
    bool posV = false;
    bool texV = false;
    bool norV = false;

    if(positionIndex == compareTriple.positionIndex)
    {
        posV = true;
    }

    if(validTexture)
    {
        if(textureIndex == compareTriple.textureIndex)
        {
            texV = true;
        }
    }
    else
    {
        texV = true;
    }

    if(validNormal)
    {
        if(normalIndex == compareTriple.normalIndex)
        {
            norV = true;
        }
    }
    else
    {
        norV = true;
    }

    return (posV && texV && norV);
*/
}


int IndexTriple::positionIndex() const
{
    return m_positionIndex;
}

int IndexTriple::textureIndex() const
{
    return m_textureIndex;
}

int IndexTriple::normalIndex() const
{
    return m_normalIndex;
}

bool IndexTriple::validTexture() const
{
    return m_validTexture;
}

bool IndexTriple::validNormal() const
{
    return m_validNormal;
}

}//namespace terminus
