#pragma once

#include <QSoundEffect>

class QMediaPlayer;
class QMediaPlaylist;

namespace terminus
{

class SoundManager
{

public:
    static SoundManager * getInstance();
protected:
    static SoundManager * m_instance;

protected:
    SoundManager();
    void initialize();

public:
    virtual ~SoundManager();

    void playSound(QString name);
    void playSoundDistant(QString name, qreal distance);
    void playBackgroundMusic();
    void toggleBackgroundMusic();

public:
    QSoundEffect * sound(QString name);
    std::map<QString, QSoundEffect *> sounds();
    QMediaPlayer * mediaPlayer();
    QMediaPlaylist * mediaPlaylist();

protected:
    std::map<QString, QSoundEffect *> m_sounds;     //use unique_ptr later
    QMediaPlayer * m_mediaPlayer;
    QMediaPlaylist * m_mediaPlaylist;
};

}  //terminus
