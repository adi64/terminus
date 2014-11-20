#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include <QObject>
#include <QSize>

class QOpenGLShaderProgram;

namespace Terminus
{

class AbstractGraphicsObject;

class Scene : public QObject
{
    Q_OBJECT
public:
    Scene();

    void setViewportSize(const QSize &size) { m_viewportSize = size; }
    void addNode(AbstractGraphicsObject* node);
public slots:
    void render();
protected:
private:
    QSize m_viewportSize;
    std::vector<AbstractGraphicsObject*> m_nodes;

};



}


#endif // SCENE_H
