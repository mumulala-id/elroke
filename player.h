#ifndef PLAYER_H
#define PLAYER_H

#include "vlc/vlc.h"
#include <QWidget>
#include <QTimer>
#include <QDesktopWidget>
#include <QApplication>
#include <QEvent>
#define POSITION_RESOLUTION 10000

class Player : public QWidget
{
    Q_OBJECT

    QString filename;

    QTimer *poller;
    bool _isplaying;
    bool _isPausing;
    libvlc_instance_t *_vlcinstance;
    libvlc_media_player_t *_mp;
    libvlc_media_t *_m;
    bool replay=false;
    libvlc_event_manager_t *m_eventMgr;
    QDesktopWidget *desktop = QApplication::desktop();
    int desktop_width = desktop->width();
    int desktop_height = desktop->height();
    QString _singer, _title;


public:
    Player(QWidget *parent = 0);
    ~Player();

public slots:
    void setFile(QString file);
    void play();
    void pause();
    void stop();
    void setMute(bool mute);
    bool isMute();
    QString getFile();
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
      void setMeta(QString title, QString singer);//title & singer

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
