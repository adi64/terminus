#pragma once

#include <QSound>

namespace terminus
{

class SoundManager
{

public:
    static SoundManager * getInstance();
protected:
    static SoundManager * m_instance;

public:
    virtual ~SoundManager();
protected:
    SoundManager();
    void initialize();

public:
    void playSound(QString name);
    QSound * sound(QString name);
    std::map<QString, QSound *> sounds();

protected:
    std::map<QString, QSound *> m_sounds;



};

}  //terminus
