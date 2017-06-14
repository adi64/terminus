#pragma once

#include <glincl.h>

#include <QDebug>
#include <QString>

inline void printGlError(const char * file, int line)
{
    GLenum err = gl.glGetError();
    if(err != GL_NO_ERROR)
    {
        qDebug() << file << " at " << line << ": 0x" << QString::number(err, 16) << " (" << err << ")";
    }
}
