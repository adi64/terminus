#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QSize>


class QOpenGLShaderProgram;

namespace Terminus
{

class Scene : public QObject
{
    Q_OBJECT
public:
    Scene();

    void setViewportSize(const QSize &size) { m_viewportSize = size; }
public slots:
    void render();
protected:
private:
    QSize m_viewportSize;
    QOpenGLShaderProgram *m_program;

};



}


#endif // SCENE_H
