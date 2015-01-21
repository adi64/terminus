#pragma once

#include <QSoundEffect>
#include <QAudio>
#include <QFile>
#include <QAudioOutput>

class QMediaPlayer;
class QMediaPlaylist;

namespace terminus
{

class SoundManager : public QObject
{

    Q_OBJECT

protected:
    SoundManager();
    void initialize();

public:
    virtual ~SoundManager();

    static SoundManager * getInstance();

    void playSound(QString name);
    void playSoundDistant(QString name, qreal distance);
    void playBackgroundMusic();
    void toggleBackgroundMusic();

    QSoundEffect * sound(QString name);
    std::map<QString, QSoundEffect *> sounds();
    QMediaPlayer * mediaPlayer();
    QMediaPlaylist * mediaPlaylist();

protected:
    static SoundManager * m_instance;
    std::map<QString, QSoundEffect *> m_sounds;     //use unique_ptr later
    QMediaPlayer * m_mediaPlayer;
    QMediaPlaylist * m_mediaPlaylist;

    QFile m_sourceFile;
    QAudioOutput * m_audio;
    QFile m_sourceFile2;
    QAudioOutput * m_audio2;
};

}  //terminus
