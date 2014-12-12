#include <QSoundEffect>
#include <QMediaPlayer>

#include "soundmanager.h"

#include <QDebug> //TODO remove

//TODO use unique or shared pointer instead of standard pointer for QSoundEffect

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
    soundOne->setSource(QUrl::fromLocalFile("sounds/test.wav"));
    m_sounds["test"] = soundOne;

    QSoundEffect * soundTwo = new QSoundEffect();
    soundTwo->setSource(QUrl::fromLocalFile("sounds/beebomb.wav"));
    m_sounds["beebomb"] = soundTwo;

    QSoundEffect * soundThree = new QSoundEffect();
    soundThree->setSource(QUrl::fromLocalFile("sounds/beesting.wav"));
    soundThree->setLoopCount(3);
    soundThree->setVolume(1.0);
    m_sounds["beesting"] = soundThree;

    m_mediaPlayer = new QMediaPlayer();
    mediaPlayer()->setMedia(QUrl::fromLocalFile("music/Kalimba.mp3"));
    mediaPlayer()->setVolume(80);
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
