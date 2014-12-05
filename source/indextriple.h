#pragma once

#

namespace terminus
{

class IndexTriple
{
public:
    IndexTriple(std::string positionSpec, std::string textureSpec, std::string normalSpec);
    ~IndexTriple();

public:
    unsigned int positionIndex();
    unsigned int textureIndex();
    unsigned int normalIndex();
    bool validTexture();
    bool validNormal();

    bool operator<(const IndexTriple compareTriple);

    bool operator==(const IndexTriple compareTriple);

protected:
    unsigned int m_positionIndex;
    unsigned int m_textureIndex;
    unsigned int m_normalIndex;

    bool m_validTexture;
    bool m_validNormal;
};

} //terminus
