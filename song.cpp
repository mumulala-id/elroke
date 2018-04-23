#include "song.h"

Song::Song(QObject *parent) : QObject(parent)
{

}

void Song::setTitle(QString _title)
{
    titile = _title;
}

void Song::setId(QString _id)
{
    id = _id;
}

void Song::setSinger(QString _singer)
{
    singer = _singer;
}

void Song::setLanguage(QString _language)
{
    language = _language;
}

void Song::setCategory(QString _category)
{
    category = _category;    
}

void Song::setPath(QString _path)
{
    path =_path;    
}

void Song::setAudioChannel(QString _audioChannel)
{
    audio_channel = _audioChannel;
}

QString Song::getId()
{
    return id;
}

QString Song::getTitle()
{
    return titile;
}

QString Song::getSinger()
{
    return singer;
}

QString Song::getLanguage()
{
    return language;
}

QString Song::getCategory()
{
    return category;
}

QString Song::getPath()
{
    return path;
}

QString Song::getAudioChannel()
{
    return audio_channel;
}

void Song::setPlaytimes(int _playtimes)
{
    playtimes = _playtimes;
}

unsigned int Song::getPlayTimes()
{
    return playtimes;
}

void Song::setFavorite(bool f){
    favorite = f;
}
