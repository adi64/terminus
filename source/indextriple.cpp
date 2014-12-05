#include "indextriple.h"

#include <QDebug>

IndexTriple::IndexTriple(std::string positionSpec, std::string textureSpec, std::string normalSpec)
: m_validTexture(true)
, m_validNormal(true)
{
    m_positionIndex = static_cast<unsigned int>(std::stoi(positionSpec));

    try
    {
        m_textureIndex = static_cast<unsigned int>(std::stoi(textureSpec));
    }
    catch (const std::invalid_argument& e)
    {
        m_validTexture = false;
        qDebug() << typeid(e).name();
    }

    try
    {
        m_normalIndex = static_casdt<unsigned int>(std::stoi(normalSpec));
    }
    catch (const std::invalid_argument& e)
    {
        m_validNormal = false;
        qDebug() << typeid(e).name();
    }
}

bool operator<(const IndexTriple)
{
    return true;
}

bool operator==(const compareTriple)
{
    return (positionIndex() == compareTriple.positionIndex()
            && textureValid() == compareTriple.textureValid() && (!textureValid() || textureIndex() == compareTriple.textureIndex())
            && normalValid() == compareTriple.normalValid() && (!normalValid() || normalIndex() == compareTriple.normalIndex()));
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


unsigned int IndexTriple::positionIndex()
{
    return m_positionIndex;
}

unsigned int IndexTriple::textureIndex()
{
    return m_textureIndex;
}

unsigned int IndexTriple::normalIndex()
{
    return m_normalIndex;
}

bool IndexTriple::validTexture()
{
    return m_validTexture;
}

bool IndexTriple::validNormal()
{
    return m_validNormal;
}
