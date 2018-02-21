#include "song.h"

Song::Song(QObject *parent) : QObject(parent)
{

}


void Song::setTitle(QString _title){
    
    titile = _title;
    
}

void Song::setId(int _id){
    
    id = _id;
    
}

void Song::setSinger(QString _singer){
    
    singer = _singer;
    
}

void Song::setLanguage(QString _language){
    
    language = _language;
}

void Song::setCategory(QString _category){
    
    category = _category;
    
}

void Song::setPath(QString _path){
    
    path =_path;
    
}

void Song::setAudioChannel(audioChannel _audioChannel){

    audio_channel = _audioChannel;
}

int Song::getId(){
    return id;
}

QString Song::getTitle(){
    return titile;
}

QString Song::getSinger(){
    return singer;
}

QString Song::getLanguage(){
    return language;
}

QString Song::getgetCategory(){
    return category;
}

QString Song::getPath(){
    
    return path;
}

Song::audioChannel Song::getAudioChannel(){
    return audio_channel;
}

void Song::setPlaytimes(int _playtimes){

    playtimes = _playtimes;
}
