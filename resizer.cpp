#include "resizer.h"

resizer::resizer(QObject *parent) : QObject(parent)
{
    _size = QSize(160,90);
    _mode = Qt::KeepAspectRatio;
}

void resizer::setSize(const QSize &size)
{
    _size = size;
}

void resizer::setAspectRatioMode(Qt::AspectRatioMode mode)
{
    _mode = mode;
}

//void resizer::setInput(const QImage &input)
//{
//    d->input(input);
//    _mode = resizer::single;
//}

//void resizer::setInput(const QString &filename)
//{
//    _filename = filename;
//    _mode = resizer::single;
//}

void resizer::setInput(const QStringList &filenames){
    _filename = filenames;
//    _mode = resizer::multi;
}

//void resizer::setInputs(const QList<QImage> images){
//    imagelist = images;
//    _mode = resizer::multi;
//}

void resizer::start()
{
    QList<QImage> imglist;
    foreach (QString x, _filename) {
        QImage output(x);
        output = output.scaled( _size, _mode, Qt::SmoothTransformation );
        imglist<<output;
    }

        emit finished( imglist );

}
