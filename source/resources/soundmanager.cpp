#include <QMediaPlayer>
#include <QMediaPlaylist>

#include <QDebug>

#include "soundmanager.h"

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
    soundShot->setSource(QUrl::fromLocalFile(":/sounds/shot.wav"));
    m_sounds["shot"] = soundShot;

    QSoundEffect * soundEngine = new QSoundEffect();
    soundEngine->setSource(QUrl::fromLocalFile(":/sounds/engine/machine.wav"));
    m_sounds["machine"] = soundEngine;

    m_mediaPlayer = new QMediaPlayer();
    m_mediaPlaylist = new QMediaPlaylist();
    mediaPlayer()->setPlaylist(mediaPlaylist());
    mediaPlayer()->setVolume(99);

    mediaPlaylist()->addMedia(QUrl::fromLocalFile(":/music/Level0107.mp3"));
    mediaPlaylist()->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
}

SoundManager::~SoundManager()
{
    //TODO delete m_sounds contents
    std::map<QString, QSoundEffect *>::iterator it;    //use const auto instead

    for(it = sounds().begin(); it != sounds().end(); it++)
    {
        delete it->second;                             //is second correct?
    }

    mediaPlayer()->setMedia(nullptr);
    delete m_mediaPlayer;
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

void SoundManager::playBackgroundMusic()
{
    mediaPlayer()->play();
}

void SoundManager::toggleBackgroundMusic()
{
    if(mediaPlayer()->state() == 1)
    {
        mediaPlayer()->pause();
    }
    else
    {
        mediaPlayer()->play();
    }
}

QSoundEffect * SoundManager::sound(QString name)
{
    return m_sounds[name];
}

std::map<QString, QSoundEffect *> SoundManager::sounds()
{
    return m_sounds;
}

QMediaPlayer * SoundManager::mediaPlayer()
{
    return m_mediaPlayer;
}

QMediaPlaylist * SoundManager::mediaPlaylist(){
    return m_mediaPlaylist;
}

}   //terminus
