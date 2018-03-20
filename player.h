#ifndef PLAYER_H
#define PLAYER_H

#include "vlc/vlc.h"
#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QEvent>
#define POSITION_RESOLUTION 10000

class Player : public QObject
{
    Q_OBJECT

public:
    explicit Player(QObject *parent = 0);
    ~Player();

private:
    libvlc_instance_t *vlc;
    libvlc_media_player_t *media_player;
    libvlc_media_t *media;
    libvlc_event_manager_t *m_eventMgr;
    QString filename;
    QTimer *poller;
    bool replay=false;
    bool _isplaying=false;
    bool _isPausing=false;

public slots:
    void setFile(QString file);
    QString getFile();
    void play();
    void pause();
    void stop();
    void setMute(bool mute);
    bool isMute();    
    void signalAlmostEnd();
    void setVolume(int newVolume);
    void changePosition(int newPosition);
    void setAudioChannelStereo();
    void setAudioChannelLeft();
    void setAudioChannelRight();
    int volume();
    int position();
    int getAudioChannel();
    bool isPlaying(){return _isplaying;}
    bool isPausing(){return _isPausing;}
    void setWinId(WId _wid);
    //event manager
    void registerEvents();
    //event handler callback
    static void callback(const libvlc_event_t* event, void *ptr);

signals:
    void reachEnded();
    void almostEnded();
    void playing();
    void paused();
    void stopped();
    void positionChanged();
    void error();
};

#endif // PLAYER_H
