#ifndef RESIZER_H
#define RESIZER_H

#include <QObject>
#include <QSize>
#include <QImage>

//struct ResizerPrivate
//{
//    QSize size;
//    Qt::AspectRatioMode aspectMode;
//    QImage input;
//    QString inputFilename;
//};


class resizer : public QObject
{
    Q_OBJECT
public:
    enum mode{single, multi};
    explicit resizer(QObject *parent = nullptr);
private:
    QSize _size;
    Qt::AspectRatioMode _mode;
    QStringList _filename;

signals:

public slots:
    void setSize(const QSize &size);
    void setAspectRatioMode(Qt::AspectRatioMode);
//    void setInput(const QImage &input);
//    void setInput(const QString &filename);
    void setInput(const QStringList &filenames);
//    void setInputs(const QList<QImage>images);
    void start();
signals:
    void error();
    void finished( QList<QImage> output );
};

#endif // RESIZER_H
