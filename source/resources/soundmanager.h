#pragma once

class QSound;
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

public:
    QSound * sound(QString name);
    std::map<QString, QSound *> sounds();    
    QMediaPlayer * mediaPlayer();

protected:
    std::map<QString, QSound *> m_sounds;
    QMediaPlayer * m_mediaPlayer;
};

}  //terminus
