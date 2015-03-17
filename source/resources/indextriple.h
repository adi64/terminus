#pragma once

#include <string>

namespace terminus
{

/*!
 * \brief The IndexTriple class is used as a temporary geometry data storage, while parsing .obj files in the resourcemanager class.
 *
 * The important relational operators " < " and " == " are implemented in order to decide, whether this specific vertex data has already been stored.
 */
class IndexTriple
{
public:
    IndexTriple(std::string positionSpec, std::string textureSpec, std::string normalSpec);
    ~IndexTriple();

    int positionIndex() const;
    int textureIndex() const;
    int normalIndex() const;
    bool validTexture() const;
    bool validNormal() const;

    /*!
     * \brief Returns true, if one of the attributes position, texture, or normal of the comparedTriple is larger, than the triple it is compared to.
     * \param compareTriple
     */
    bool operator<(const IndexTriple & compareTriple) const;

    /*!
     * \brief Returns true, if the compared attributes position, texture and normal have the same value between the compared triples.
     * \param compareTriple
     */
    bool operator==(const IndexTriple & compareTriple) const;

protected:
    int m_positionIndex;
    int m_textureIndex;
    int m_normalIndex;

    bool m_validTexture;
    bool m_validNormal;
};

} //terminus
