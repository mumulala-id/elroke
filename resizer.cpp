#include "resizer.h"

resizer::resizer(QObject *parent) : QObject(parent)
{

}

void resizer::setInputs(const QStringList &filenames)
{
    _filename = filenames;
}

void resizer::start()
{
    QList<QImage> imglist;
    foreach (const QString &x, _filename) {
        QImage output(x);
        output = output.scaled(QSize(160,90), Qt::KeepAspectRatio, Qt::FastTransformation );
        imglist<<output;
    }
   emit finished( imglist );
}
