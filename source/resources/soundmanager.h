#pragma once

#include <QSoundEffect>

class QMediaPlayer;

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
    void playBackgroundMusic();
    void toggleBackgroundMusic();

public:
    QSoundEffect * sound(QString name);
    std::map<QString, QSoundEffect *> sounds();
    QMediaPlayer * mediaPlayer();

protected:
    std::map<QString, QSoundEffect *> m_sounds;     //use unique_ptr later
    QMediaPlayer * m_mediaPlayer;
};

}  //terminus
