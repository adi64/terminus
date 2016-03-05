#pragma once

#include <QSoundEffect>
#include <QAudio>
#include <QFile>
#include <QAudioOutput>

class QMediaPlayer;
class QMediaPlaylist;

namespace terminus
{

/*!
 * \brief The SoundManager class provides basic music and sound playing functionality.
 * All needed sound effects are stored in the member variable m_sounds.
 * Implemented as a singelton, accessibility for all classes is ensured.
 */
class SoundManager : public QObject
{

    Q_OBJECT

protected:
    SoundManager();
    void initialize();

public:
    virtual ~SoundManager();

    /*!
     * \brief Returns the singelton instance of the SoundManager class
     */
    static SoundManager * getInstance();

    /*!
     * \brief Plays a sound effect of a certain name
     *
     * Provided that the desired sound effect is stored on initializiation, the sound effect will be played in full length.
     * Calling the function for the same sound effect a second time, while the sound is still playing from the first invocation,
     * will result in restarting the current sound effect.
     * \param name Name of the sound effect meant to be played
     */
    void playSound(QString name);

    void toggleBackgroundMusic();

    QSoundEffect * sound(QString name);
    std::map<QString, QSoundEffect *> sounds();

protected:
    static SoundManager * m_instance;
    std::map<QString, QSoundEffect *> m_sounds;
};

}  //terminus
