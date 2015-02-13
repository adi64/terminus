#include "soundmanager.h"

#include <QMediaPlayer>
#include <QMediaPlaylist>

#include <QDebug>

//TODO use unique or shared pointer instead of standard pointer for QSoundEffect

//if Android doesn't like QMultimedia -> try libVLC

namespace terminus
{

constexpr int c_maxVol = 100;

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
    //TODO delete m_sounds contents
    std::map<QString, QSoundEffect *>::iterator it;    //use const auto instead

    for(it = sounds().begin(); it != sounds().end(); it++)
    {
        delete it->second;                             //is second correct?
    }
}

void SoundManager::playSound(QString name)
{
    sound(name)->play();
}

void SoundManager::playSoundDistant(QString name, qreal distance)
{
    qreal relativeVolume = 1.0 - distance/c_maxVol;

    if(relativeVolume <= 0)
    {
        relativeVolume = 0.0;
    }

    sound(name)->setVolume(relativeVolume);
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
