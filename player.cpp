#include "player.h"
#include<QDebug>

Player::Player(QObject *parent) : QObject(parent)
{
    const char * const vlc_args[] = {
    //   "--verbose=2",
    };

    vlc = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args );
    media_player = libvlc_media_player_new(vlc);
    m_eventMgr = libvlc_media_player_event_manager(media_player);
    registerEvents();

    poller = new QTimer(this);
    connect(poller,SIGNAL(timeout()), this,SLOT(signalAlmostEnd()));
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
        media = libvlc_media_new_path(vlc, getFile().toLatin1());
        libvlc_media_player_set_media(media_player, media);
        libvlc_media_player_play(media_player);
        _isplaying=1;
        _isPausing=0;
    }
    else
    {
        libvlc_media_player_play(media_player);
        _isplaying=1;
        _isPausing=0;
    }
}

void Player::pause()
{
    libvlc_media_player_pause(media_player);
    _isplaying=false;
    _isPausing=true;
}

void Player::stop()
{
    _isplaying=false;
    libvlc_media_player_stop(media_player);
    poller->stop();
}

void Player::setVolume(int newVolume)
{
    libvlc_audio_set_volume(media_player, newVolume);
}

void Player::changePosition(int newPosition)
{
    libvlc_media_t *curMedia = libvlc_media_player_get_media(media_player);

    if(curMedia == NULL)
        return;

    float pos = (float)(newPosition)/(float)POSITION_RESOLUTION;
    libvlc_media_player_set_position(media_player,pos);
}

void Player::signalAlmostEnd()
{
    if(!_isplaying)
        return;

    auto duration= libvlc_media_player_get_length(media_player);

     if(duration<10000)
     return;

     //before 10 sec video end signal will be emitted
    auto limit= (int)(duration/1000)-10;

    auto time =(int) libvlc_media_player_get_time(media_player)/1000;
          if(time==limit )
          {
             emit almostEnded();
          }
}

void Player::setAudioChannelStereo()
{
    if(!_isplaying)
        return;

      libvlc_audio_set_channel(media_player, 1);
}

void Player::setAudioChannelLeft()
{
    if(!_isplaying)        return;
    libvlc_audio_set_channel(media_player, 3);
}

void Player::setAudioChannelRight()
{
    if(!_isplaying)        return;
    libvlc_audio_set_channel(media_player, 4);
}

void Player::setMute(bool mute)
{
    if(!_isplaying)        return;
    libvlc_audio_set_mute(media_player, mute);
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
    return   libvlc_audio_get_volume(media_player);
}

int Player::position()
{
       float pos = libvlc_media_player_get_position(media_player);
        return (int)(pos * (float)(100));
}

int Player::getAudioChannel()
{
    return libvlc_audio_get_channel(media_player);
}

bool Player::isMute()
{
    return libvlc_audio_get_mute(media_player);
}

Player::~Player()
{
    libvlc_event_detach(m_eventMgr,libvlc_MediaPlayerEndReached,callback,this);
    libvlc_event_detach(m_eventMgr,libvlc_MediaPlayerPaused,callback,this);
    libvlc_event_detach(m_eventMgr,libvlc_MediaPlayerStopped,callback,this);
    libvlc_event_detach(m_eventMgr,libvlc_MediaPlayerPositionChanged,callback,this);
    libvlc_event_detach(m_eventMgr,libvlc_MediaPlayerEncounteredError,callback, this);
    libvlc_event_detach(m_eventMgr,libvlc_MediaPlayerPlaying,callback, this);

    libvlc_media_player_stop(media_player);
    libvlc_media_player_release(media_player);
    libvlc_release(vlc);

}

void Player::setWinId(WId _wid)
{
    libvlc_media_player_set_xwindow(media_player, _wid );
}
