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

    void run(qreal vol, QString file);
    void run2(qreal vol, QString file);

public slots:
    void handleStateChanged(QAudio::State newState);
    void handleStateChanged2(QAudio::State newState);



protected:
    std::map<QString, QSoundEffect *> m_sounds;     //use unique_ptr later
    QMediaPlayer * m_mediaPlayer;
    QMediaPlaylist * m_mediaPlaylist;

    QFile m_sourceFile;
    QAudioOutput * m_audio;
    QFile m_sourceFile2;
    QAudioOutput * m_audio2;
};

}  //terminus
