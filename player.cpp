#include "player.h"
#include<QDebug>

Player::Player(QObject *parent) : QObject(parent)
{
    const char * const vlc_args[] = {
     //   "--verbose=2",
    };

      _isplaying =0;
    poller = new QTimer(this);

    _vlcinstance = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args );

    _mp = libvlc_media_player_new(_vlcinstance);

    connect(poller,SIGNAL(timeout()), this,SLOT(signalAlmostEnd()));

    m_eventMgr = libvlc_media_player_event_manager(_mp);
    registerEvents();

   poller->start(1000);
}

void Player::setFile(QString file)
{
    filename = file;
}

QString Player::getFile()
{
    return filename;
}

void Player::play()
{
    if(getFile()==NULL)
    {
        qDebug()<<"file may not be empty";
        return;
    }

    if(!_isPausing)
    {
        _m = libvlc_media_new_path(_vlcinstance, getFile().toLatin1());
        libvlc_media_player_set_media(_mp, _m);
        libvlc_media_player_play(_mp);
        _isplaying=1;
        _isPausing=0;
    }
    else
    {
        libvlc_media_player_play(_mp);
        _isplaying=1;
        _isPausing=0;
    }
}

void Player::pause()
{
    libvlc_media_player_pause(_mp);
    _isplaying=false;
    _isPausing=true;
}

void Player::stop()
{
    _isplaying=false;
    libvlc_media_player_stop(_mp);
    poller->stop();
}

void Player::setVolume(int newVolume)
{
    libvlc_audio_set_volume(_mp, newVolume);
}

void Player::changePosition(int newPosition)
{
    libvlc_media_t *curMedia = libvlc_media_player_get_media(_mp);

    if(curMedia == NULL)
        return;

    float pos = (float)(newPosition)/(float)POSITION_RESOLUTION;
    libvlc_media_player_set_position(_mp,pos);
}

void Player::signalAlmostEnd()
{
    if(!_isplaying)
        return;

    auto duration= libvlc_media_player_get_length(_mp);

     if(duration<10000)
     return;

     //before 10 sec video end signal will be emitted
    auto limit= (int)(duration/1000)-10;

    auto time =(int) libvlc_media_player_get_time(_mp)/1000;
          if(time==limit )
          {
             emit almostEnded();
          }
}

void Player::setAudioChannelStereo()
{
    if(!_isplaying)
        return;

      libvlc_audio_set_channel(_mp, 1);
}

void Player::setAudioChannelLeft()
{
    if(!_isplaying)        return;
    libvlc_audio_set_channel(_mp, 3);
}

void Player::setAudioChannelRight()
{
    if(!_isplaying)        return;
    libvlc_audio_set_channel(_mp, 4);
}

void Player::setMute(bool mute)
{
    if(!_isplaying)        return;
    libvlc_audio_set_mute(_mp, mute);
}

void Player::registerEvents()
{
    libvlc_event_attach(m_eventMgr,libvlc_MediaPlayerEndReached,callback,this);
    libvlc_event_attach(m_eventMgr,libvlc_MediaPlayerPaused,callback,this);
    libvlc_event_attach(m_eventMgr,libvlc_MediaPlayerStopped,callback,this);
    libvlc_event_attach(m_eventMgr,libvlc_MediaPlayerPositionChanged,callback,this);
    libvlc_event_attach(m_eventMgr,libvlc_MediaPlayerEncounteredError,callback, this);
    libvlc_event_attach(m_eventMgr,libvlc_MediaPlayerPlaying,callback, this);
}

void Player::callback(const libvlc_event_t *event, void *ptr)
{
    Player * self = reinterpret_cast<Player*>(ptr);

    switch (event->type)
    {
    case libvlc_MediaPlayerEndReached:
        self->emit reachEnded();
        break;
    case libvlc_MediaPlayerPaused:
        self->emit paused();
        break;
    case libvlc_MediaPlayerStopped:
        self->emit stopped();
        break;
    case libvlc_MediaPlayerPositionChanged:
        self->emit positionChanged();
        break;
    case libvlc_MediaPlayerPlaying:
        self->emit playing();
        break;
    case libvlc_MediaPlayerEncounteredError:
        self->emit error();
        break;
    }
}

int Player::volume()
{
    return   libvlc_audio_get_volume(_mp);
}

int Player::position()
{
       float pos = libvlc_media_player_get_position(_mp);
        return (int)(pos * (float)(100));
}

int Player::getAudioChannel()
{
    return libvlc_audio_get_channel(_mp);
}

bool Player::isMute()
{
    return libvlc_audio_get_mute(_mp);
}

Player::~Player()
{
    libvlc_event_detach(m_eventMgr,libvlc_MediaPlayerEndReached,callback,this);
    libvlc_event_detach(m_eventMgr,libvlc_MediaPlayerPaused,callback,this);
    libvlc_event_detach(m_eventMgr,libvlc_MediaPlayerStopped,callback,this);
    libvlc_event_detach(m_eventMgr,libvlc_MediaPlayerPositionChanged,callback,this);
    libvlc_event_detach(m_eventMgr,libvlc_MediaPlayerEncounteredError,callback, this);
    libvlc_event_detach(m_eventMgr,libvlc_MediaPlayerPlaying,callback, this);

    libvlc_media_player_stop(_mp);
    libvlc_media_player_release(_mp);
    libvlc_release(_vlcinstance);

}

void Player::setWinId(WId _wid)
{
    libvlc_media_player_set_xwindow(_mp, _wid );
}
