#include <QSoundEffect>
#include <QMediaPlayer>

#include "soundmanager.h"

#include <QDebug> //TODO remove

//TODO use unique or shared pointer instead of standard pointer for QSoundEffect

//if Android doesn't like QMultimedia -> try libVLC

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
    QSoundEffect * soundOne = new QSoundEffect();
    soundOne->setSource(QUrl::fromLocalFile("sounds/alarm.wav"));
    m_sounds["alarm"] = soundOne;

    QSoundEffect * soundTwo = new QSoundEffect();
    soundTwo->setSource(QUrl::fromLocalFile("sounds/angriff.wav"));
    m_sounds["angriff"] = soundTwo;

    QSoundEffect * soundThree = new QSoundEffect();
    soundThree->setSource(QUrl::fromLocalFile("sounds/shot.wav"));
    m_sounds["shot"] = soundThree;

    QSoundEffect * soundFour = new QSoundEffect();
    soundFour->setSource(QUrl::fromLocalFile("sounds/engin/machine.wav"));
    soundFour->setVolume(0.1);
    m_sounds["machine"] = soundFour;

    m_mediaPlayer = new QMediaPlayer();
    mediaPlayer()->setMedia(QUrl::fromLocalFile("music/Level0107.mp3"));
    mediaPlayer()->setVolume(99);
}

SoundManager::~SoundManager()
{
    //TODO delete m_sounds contents
    std::map<QString, QSoundEffect *>::iterator it;    //use const auto instead

    qDebug() << "am i called?";

    for(it = sounds().begin(); it != sounds().end(); it++)
    {
        delete it->second;                             //is second correct?
    }

    delete m_mediaPlayer;
}

void SoundManager::playSound(QString name)
{
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

}   //terminus
