#include "indextriple.h"

#include <stdexcept>

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
    }

    try
    {
        m_normalIndex = std::stoi(normalSpec);
    }
    catch (const std::invalid_argument& e)
    {
        m_validNormal = false;
    }
}

IndexTriple::~IndexTriple()
{
}

bool IndexTriple::operator<(const IndexTriple & other) const
{
    return m_positionIndex < other.m_positionIndex ||
            m_textureIndex < other.m_textureIndex ||
            m_normalIndex < other.m_normalIndex;
}

bool IndexTriple::operator==(const IndexTriple & other) const
{
    return (positionIndex() == other.positionIndex() &&
            validTexture() == other.validTexture() && (!validTexture() || textureIndex() == other.textureIndex()) &&
            validNormal() == other.validNormal() && (!validNormal() || normalIndex() == other.normalIndex()));
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
