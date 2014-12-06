#pragma once

#include <string>

namespace terminus
{

class IndexTriple
{
public:
    IndexTriple(std::string positionSpec, std::string textureSpec, std::string normalSpec);
    ~IndexTriple();

public:
    unsigned int positionIndex() const;
    unsigned int textureIndex() const;
    unsigned int normalIndex() const;
    bool validTexture() const;
    bool validNormal() const;

    bool operator<(const IndexTriple & compareTriple) const;

    bool operator==(const IndexTriple & compareTriple) const;

protected:
    unsigned int m_positionIndex;
    unsigned int m_textureIndex;
    unsigned int m_normalIndex;

    bool m_validTexture;
    bool m_validNormal;
};

} //terminus
