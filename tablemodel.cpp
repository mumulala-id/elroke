#include "tablemodel.h"

#include <QDebug>

tablemodel::tablemodel(QObject *parent) :
    QAbstractTableModel(parent)
{

}


void tablemodel::update(QList<QString> list){

    beginResetModel();
    m_videoList = list;
    endResetModel();

}

int tablemodel::columnCount(const QModelIndex &parent) const{

   Q_UNUSED(parent);
    return 3;

}

int tablemodel::rowCount(const QModelIndex &parent) const {

   Q_UNUSED(parent);
    return m_videoList.size();

}

QVariant tablemodel::data(const QModelIndex &index, int role) const {

    if(!index.isValid())
        return QVariant();
    if(index.row() >= m_videoList.size() || index.row() <0){

        return QVariant();
    }

    if(role == Qt::DisplayRole){

        const QString &vc= m_videoList.at(index.row());


if(vc.contains("#")){
        const    QStringList &filename = vc.split("#");

        switch (index.column()) {
        case 0 :
           return vc;
        case 1:
            return filename.at(0);
        case 2:
            return filename.at(1);
          default :
            return QVariant();

        }
}
else{

//qDebug()<<vc<<"to be fixed";

}

    }

    else if(role == Qt::TextAlignmentRole){
        if(index.column()==0)
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        else
            return Qt::AlignLeft;
    }

    return QVariant();

}


QVariant tablemodel::headerData(int section, Qt::Orientation orientation, int role) const {

    if(role !=Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal){
        switch (section) {
        case 0:
            return tr("Path");
        case 1:
            return tr("Title");
        case 2:
            return tr("Singer");
        default :
                return QVariant();
        }
        }
            return section+1;

}
