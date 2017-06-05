#pragma once

#include <list>

#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>


namespace terminus
{

class ObjectMatrix
{
public:
    ObjectMatrix(const QVector3D & position = QVector3D(),
                 const QQuaternion & rotation = QQuaternion(),
                 const QVector3D & scale = QVector3D(1.0, 1.0, 1.0) );
    virtual ~ObjectMatrix();

    virtual const QVector3D & position() const;
    virtual void setPosition(const QVector3D & position);
    void translate(const QVector3D & vector);
    virtual const QQuaternion & rotation() const;
    virtual void setRotation(const QQuaternion & rotation);
    void rotate(const QQuaternion & rotation);
    virtual const QVector3D & scale() const;
    virtual void setScale(const QVector3D & scale);
    void scale(const QVector3D & factors);

    void setParent(ObjectMatrix * parent);

    virtual QMatrix4x4 & matrix() const;
    virtual QMatrix4x4 & inverseMatrix() const;

    virtual QVector3D transform(const QVector3D & vector);
    virtual QVector3D inverseTransform(const QVector3D & vector);

protected:
    void invalidate() const;
    void calculateMatrix() const;
    void calculateMatrixInverted() const;

    void addChild(ObjectMatrix * child);
    void removeChild(ObjectMatrix * child);

    void onParentChanged();

protected:
    QVector3D m_position;
    QQuaternion m_rotation;
    QVector3D m_scale;

    mutable bool m_matrixChanged;
    mutable QMatrix4x4 m_matrix;
    mutable bool m_inverseMatrixChanged;
    mutable QMatrix4x4 m_inverseMatrix;

    ObjectMatrix * m_parent;
    std::list<ObjectMatrix *> m_children;
};

} // namespace terminus

