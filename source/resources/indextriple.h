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
    int positionIndex() const;
    int textureIndex() const;
    int normalIndex() const;
    bool validTexture() const;
    bool validNormal() const;

    bool operator<(const IndexTriple & compareTriple) const;

    bool operator==(const IndexTriple & compareTriple) const;

protected:
    int m_positionIndex;
    int m_textureIndex;
    int m_normalIndex;

    bool m_validTexture;
    bool m_validNormal;
};

} //terminus
