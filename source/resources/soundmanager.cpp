#include <QSound>
#include <QMediaPlayer>

#include "soundmanager.h"

#include <QDebug> //TODO remove

//TODO use unique or shared pointer instead of standard pointer for QSound

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
    QSound * soundOne = new QSound("sounds/test.wav");
    m_sounds["test"] = soundOne;

    m_mediaPlayer = new QMediaPlayer();
    mediaPlayer()->setMedia(QUrl::fromLocalFile("music/Kalimba.mp3"));
    mediaPlayer()->setVolume(50);
}

SoundManager::~SoundManager()
{
    //TODO delete m_sounds contents
    std::map<QString, QSound *>::iterator it = sounds().begin();    //use const auto instead

    qDebug() << "am i called?";

    for(it; it != sounds().end(); it++)
    {
        delete it->second;                                               //is second correct?
    }

    delete m_mediaPlayer;
}

void SoundManager::playSound(QString name)
{
    qDebug() << "right before playing";
    sound(name)->play();
    //QSound::play(name);
}

void SoundManager::playBackgroundMusic()
{
    mediaPlayer()->play();
}

QSound * SoundManager::sound(QString name)
{
    return m_sounds[name];
}

std::map<QString, QSound *> SoundManager::sounds()
{
    return m_sounds;
}

QMediaPlayer * SoundManager::mediaPlayer()
{
    return m_mediaPlayer;
}

}   //terminus
