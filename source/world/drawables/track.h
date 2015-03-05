#pragma once

#include <memory>

#include <QVector3D>

#include "abstractgraphicsobject.h"
#include <util/polyline.h>

namespace terminus
{

class Track : public AbstractGraphicsObject
{
public:
    Track(World & world, std::unique_ptr<Polyline> controlPoints, bool isOtherTrackLeft);

    QVector3D positionAt(float distance);
    QVector3D tangentAt(float distance);
    float length();

    bool isOtherTrackLeft();

protected:
    virtual bool localRenderEnabled() const override;

protected:
    std::unique_ptr<Polyline> m_course;

    bool m_isOtherTrackLeft;
};

}
