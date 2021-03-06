#ifndef SONG_H
#define SONG_H

#include <QObject>

class Song : public QObject
{
    Q_OBJECT
    
public:
    explicit Song(QObject *parent = nullptr);

private :
    unsigned int  playtimes;
    QString id;
    QString titile,singer, path,language, category, audio_channel;
    bool favorite;
    
    
public slots:
    
    void setId(QString);
    void setTitle(QString _title);
    void setSinger(QString);
    void setLanguage(QString);
    void setCategory(QString);
    void setPath(QString);
    void setPlaytimes(int);
    void setAudioChannel(QString);
    void setFavorite(bool);
   QString getId();
    QString getTitle();
    QString getSinger();
    QString getLanguage();
    QString getCategory();
    QString getPath();
    QString getAudioChannel();
    bool isFavorite()const{ return favorite;};
   unsigned int getPlayTimes();
    
    

};

#endif // SONG_H
