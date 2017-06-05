#include "objectmatrix.h"

#include <algorithm>

namespace terminus
{

ObjectMatrix::ObjectMatrix(const QVector3D & position,
                           const QQuaternion & rotation,
                           const QVector3D & scale)
: m_position(position)
, m_rotation(rotation)
, m_scale(scale)
, m_matrixChanged(true)
, m_parent(nullptr)
{
}

const QVector3D & ObjectMatrix::position() const
{
    return m_position;
}

void ObjectMatrix::setPosition(const QVector3D & position)
{
    m_position = position;
    invalidate();
}

void ObjectMatrix::translate(const QVector3D & vector)
{
    m_position += vector;
    invalidate();
}

const QQuaternion & ObjectMatrix::rotation() const
{
    return m_rotation;
}

void ObjectMatrix::setRotation(const QQuaternion & rotation)
{
    m_rotation = rotation;
    invalidate();
}

void ObjectMatrix::rotate(const QQuaternion & rotation)
{
    m_rotation += rotation;
    invalidate();
}

const QVector3D & ObjectMatrix::scale() const
{
    return m_scale;
}

void ObjectMatrix::setScale(const QVector3D & scale)
{
    m_scale = scale;
    invalidate();
}

void ObjectMatrix::scale(const QVector3D & factors)
{
    m_scale *= factors;
    invalidate();
}

void ObjectMatrix::setParent(ObjectMatrix * parent)
{
    if (m_parent)
    {
        m_parent->removeChild(this);
    }
    m_parent = parent;
    if (m_parent)
    {
        m_parent->addChild(this);
    }
}


QMatrix4x4 & ObjectMatrix::matrix() const
{
    if (m_matrixChanged)
    {
        calculateMatrix();
    }
    return m_matrix;
}

QMatrix4x4 & ObjectMatrix::inverseMatrix() const
{
    if (m_inverseMatrixChanged)
    {
        calculateMatrixInverted();
    }
    return m_inverseMatrix;
}

QVector3D ObjectMatrix::transform(const QVector3D &vector)
{
    QVector4D v4 = matrix() * QVector4D(vector, 1.f);
    return v4.toVector3DAffine();
}

QVector3D ObjectMatrix::inverseTransform(const QVector3D &vector)
{
    QVector4D v4 = inverseMatrix() * QVector4D(vector, 1.f);
    return v4.toVector3DAffine();
}

void ObjectMatrix::calculateMatrix() const
{
    if (m_parent)
    {
        m_matrix = m_parent->matrix();
    }
    else
    {
        m_matrix.setToIdentity();
    }

    //TODO-LW: this order was reversed in the original code (AbstractGraphicsObject)... check!
    m_matrix.scale(m_scale);
    m_matrix.rotate(m_rotation);
    m_matrix.translate(m_position);

    m_matrixChanged = false;
}

void ObjectMatrix::calculateMatrixInverted() const
{
    m_inverseMatrix = m_matrix.inverted();
    m_inverseMatrixChanged = false;
}

void ObjectMatrix::invalidate() const
{
    m_matrixChanged = true;
    m_inverseMatrixChanged = true;
    for (ObjectMatrix * child : m_children)
    {
        if(child)
        {
            child->onParentChanged();
        }
    }
}

void ObjectMatrix::addChild(ObjectMatrix * child)
{
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it == m_children.end())
    {
        m_children.push_back(child);
    }
}

void ObjectMatrix::removeChild(ObjectMatrix * child)
{
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end())
    {
        m_children.erase(it);
    }
}

void ObjectMatrix::onParentChanged()
{
    invalidate();
}

}
