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
    QSoundEffect * soundOne = new QSoundEffect();
    soundOne->setSource(QUrl::fromLocalFile("sounds/alarm.wav"));
    m_sounds["alarm"] = soundOne;

    QSoundEffect * soundTwo = new QSoundEffect();
    soundTwo->setVolume(0.05);
    soundTwo->setSource(QUrl::fromLocalFile("sounds/angriff.wav"));
    m_sounds["angriff"] = soundTwo;

    QSoundEffect * soundThree = new QSoundEffect();
    soundThree->setVolume(0.9);
    soundThree->setSource(QUrl::fromLocalFile("sounds/shot.wav"));
    m_sounds["shot"] = soundThree;

    QSoundEffect * soundFour = new QSoundEffect();
    soundFour->setSource(QUrl::fromLocalFile("sounds/engin/machine.wav"));
    soundFour->setVolume(0.1);
    m_sounds["machine"] = soundFour;

    m_mediaPlayer = new QMediaPlayer();
    m_mediaPlaylist = new QMediaPlaylist();
    mediaPlayer()->setPlaylist(mediaPlaylist());
    mediaPlayer()->setVolume(99);

    mediaPlaylist()->addMedia(QUrl::fromLocalFile("music/Level0107.mp3"));
    mediaPlaylist()->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
}

void SoundManager::run(qreal vol, QString file)
{
    m_sourceFile.setFileName("sounds/"+file);

    if(m_sourceFile.open(QIODevice::ReadOnly))
    {
        m_sourceFile.seek(44);
        //QByteArray audio_data = sourceFile.readAll();
        //sourceFile.close();

        //QBuffer * audio_buffer = new QBuffer(&audio_data);
        //audio_buffer.open(QIODevice::ReadOnly);
        //qDebug() << audio_buffer->size();

        QAudioFormat format;

        format.setSampleSize(16); //TODO
        format.setSampleRate(11025);
        format.setChannelCount(1);
        format.setCodec("audio/pcm");
        format.setByteOrder(QAudioFormat::LittleEndian);
        format.setSampleType(QAudioFormat::UnSignedInt);

        m_audio = new QAudioOutput(format);
        m_audio->setVolume(vol);
        qDebug() << vol;
        connect(m_audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));
        m_audio->start(&m_sourceFile);
        qDebug() << "channel 1";
    }
}

void SoundManager::run2(qreal vol, QString file)
{
    m_sourceFile2.setFileName("sounds/"+file);

    if(m_sourceFile2.open(QIODevice::ReadOnly))
    {
        m_sourceFile2.seek(44);
        //QByteArray audio_data = sourceFile.readAll();
        //sourceFile.close();

        //QBuffer * audio_buffer = new QBuffer(&audio_data);
        //audio_buffer.open(QIODevice::ReadOnly);
        //qDebug() << audio_buffer->size();

        QAudioFormat format;

        format.setSampleSize(16); //TODO
        format.setSampleRate(11025);
        format.setChannelCount(1);
        format.setCodec("audio/pcm");
        format.setByteOrder(QAudioFormat::LittleEndian);
        format.setSampleType(QAudioFormat::UnSignedInt);

        m_audio2 = new QAudioOutput(format);
        m_audio2->setVolume(vol);
        qDebug() << vol;
        connect(m_audio2, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged2(QAudio::State)));
        m_audio2->start(&m_sourceFile2);
        qDebug() << "channel 2";
    }
}


void SoundManager::handleStateChanged(QAudio::State newState)
{
    switch(newState){
        case QAudio::IdleState:
            m_audio->stop();
            m_sourceFile.close();
            delete m_audio;
            break;

        case QAudio::StoppedState:
            if(m_audio2->error() != QAudio::NoError){
                qDebug() << "i should not be here";
            }
            break;
    }
}

void SoundManager::handleStateChanged2(QAudio::State newState)
{
    switch(newState){
        case QAudio::IdleState:
            m_audio2->stop();
            m_sourceFile2.close();
            delete m_audio2;
            break;

        case QAudio::StoppedState:
            if(m_audio2->error() != QAudio::NoError){
                qDebug() << "i should not be here";
            }
            break;
    }
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
