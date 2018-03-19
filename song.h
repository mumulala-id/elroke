#ifndef SONG_H
#define SONG_H

#include <QObject>

class Song : public QObject
{
    Q_OBJECT
    
public:
    enum audioChannel{left,right,stereo};

    explicit Song(QObject *parent = nullptr);

private :
    int id;
    QString titile,singer;
    int playtimes;
    QString path;
    QString language;
    QString category;
    QString audio_channel;
    
    
public slots:
    
    void setId(int);
    void setTitle(QString _title);
    void setSinger(QString);
    void setLanguage(QString);
    void setCategory(QString);
    void setPath(QString);
    void setPlaytimes(int);
    void setAudioChannel(QString);
    int getId();
    QString getTitle();
    QString getSinger();
    QString getLanguage();
    QString getgetCategory();
    QString getPath();
    QString getAudioChannel();
    int getPlayTimes();
    
    

};

#endif // SONG_H
