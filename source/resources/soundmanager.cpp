#include "soundmanager.h"

#include <QMediaPlayer>
#include <QMediaPlaylist>

namespace terminus
{

SoundManager * SoundManager::m_instance = nullptr;

SoundManager * SoundManager::getInstance()
{
    if(m_instance == nullptr)
    {
        m_instance = new SoundManager();
    }

    return m_instance;
}

SoundManager::SoundManager()
{
    initialize();
}

void SoundManager::initialize()
{
    QSoundEffect * soundShot = new QSoundEffect();
    soundShot->setSource(QUrl::fromLocalFile(":/data/sounds/shot.wav"));
    m_sounds["shot"] = soundShot;

    QSoundEffect * soundEngine = new QSoundEffect();
    soundEngine->setSource(QUrl::fromLocalFile(":/data/sounds/hover.wav"));
    m_sounds["machine"] = soundEngine;

    QSoundEffect * soundBackgroundMusic = new QSoundEffect();
    soundBackgroundMusic->setSource(QUrl::fromLocalFile(":/data/sounds/level.wav"));
    soundBackgroundMusic->setLoopCount(QSoundEffect::Infinite);
    soundBackgroundMusic->setMuted(false);
    m_sounds["music"] = soundBackgroundMusic;
}

SoundManager::~SoundManager()
{
    std::map<QString, QSoundEffect *>::iterator it;

    for(it = sounds().begin(); it != sounds().end(); it++)
    {
        delete it->second;
    }
}

void SoundManager::playSound(QString name)
{
    sound(name)->play();
}

void SoundManager::toggleBackgroundMusic()
{
    sound("music")->setMuted(!sound("music")->isMuted());
}

QSoundEffect * SoundManager::sound(QString name)
{
    return m_sounds[name];
}

std::map<QString, QSoundEffect *> SoundManager::sounds()
{
    return m_sounds;
}

}   //terminus
