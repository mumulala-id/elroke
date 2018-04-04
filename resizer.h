#ifndef RESIZER_H
#define RESIZER_H

#include <QObject>
#include <QSize>
#include <QImage>

class resizer : public QObject
{
    Q_OBJECT
public:
    explicit resizer(QObject *parent = nullptr);
private:
    QStringList _filename;

signals:

public slots:

    void setInputs(const QStringList &filenames);
    void start();
signals:
    void error();
    void finished( QList<QImage> output );
};

#endif // RESIZER_H
