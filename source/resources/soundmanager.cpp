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
    QSoundEffect * soundShotAction = new QSoundEffect();
    soundShotAction->setSource(QUrl::fromLocalFile(":/data/sounds/shot.wav"));
    m_sounds["shot"] = soundShotAction;

    QSoundEffect * soundExplosion = new QSoundEffect();
    soundExplosion->setSource(QUrl::fromLocalFile(":/data/sounds/explosion.wav"));
    m_sounds["explosion"] = soundExplosion;

    QSoundEffect * soundRepairAction = new QSoundEffect();
    soundRepairAction->setSource(QUrl::fromLocalFile(":/data/sounds/repair.wav"));
    m_sounds["repairAction"] = soundRepairAction;

    QSoundEffect * soundEngineAction = new QSoundEffect();
    soundEngineAction->setSource(QUrl::fromLocalFile(":/data/sounds/engine.wav"));
    m_sounds["engineAction"] = soundEngineAction;

    QSoundEffect * soundBackgroundMusic = new QSoundEffect();
    soundBackgroundMusic->setSource(QUrl::fromLocalFile(":/data/sounds/level.wav"));
    soundBackgroundMusic->setLoopCount(QSoundEffect::Infinite);
    soundBackgroundMusic->setMuted(false);
    m_sounds["music"] = soundBackgroundMusic;
}

SoundManager::~SoundManager()
{
    std::map<QString, QSoundEffect *>::iterator it;

    for(it = sounds().begin(); it != sounds().end(); ++it)
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
