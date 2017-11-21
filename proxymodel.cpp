#include "proxymodel.h"
#include <QDebug>


ProxyModel::ProxyModel(QObject *parent): QSortFilterProxyModel(parent),
    text_search("")
{
}

void ProxyModel::search(QString s){

    if(text_search!=s)
    text_search=s;
    invalidateFilter();

}

bool ProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{

    QModelIndex indG = sourceModel()->index(source_row, 1, source_parent);
    QModelIndex indD = sourceModel()->index(source_row, 2, source_parent);
    return sourceModel()->data(indG).toString().contains(text_search, Qt::CaseInsensitive) ||
            sourceModel()->data(indD).toString().contains(text_search, Qt::CaseInsensitive) ;

}

QVariant ProxyModel::headerData(int section, Qt::Orientation orientation, int role) const {

    return sourceModel()->headerData(section, orientation, role);
}

QVariant ProxyModel::data(const QModelIndex &index, int role) const{


    if(role==Qt::TextAlignmentRole){

        if(alignMap.contains(-1))
            return QVariant(alignMap.value(-1));

        if(alignMap.contains(index.column())){
            return QVariant(alignMap.value(index.column()));
        }

    }

            return QSortFilterProxyModel::data(index, role);


}

void ProxyModel::setAlignment(unsigned int index, Qt::Alignment flag){

    if(alignMap.contains(-1))
        clearAlign();

    alignMap.insert(index, flag);

}


void ProxyModel::clearAlign(){

    alignMap.clear();

}
