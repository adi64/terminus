#include "indextriple.h"

#include <stdexcept>

#include <cstdlib>

namespace terminus
{

IndexTriple::IndexTriple(std::string positionSpec, std::string textureSpec, std::string normalSpec)
: m_validTexture(true)
, m_validNormal(true)
{
    char * next = nullptr;
    m_positionIndex = static_cast<int>(strtol(positionSpec.c_str(), &next, 0));

    m_textureIndex = static_cast<int>(strtol(textureSpec.c_str(), &next, 0));
    m_validTexture = textureSpec.c_str() != next;

    m_normalIndex = static_cast<int>(strtol(normalSpec.c_str(), &next, 0));
    m_validNormal = normalSpec.c_str() != next;
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
