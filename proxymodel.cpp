#include "proxymodel.h"
#include <QDebug>


ProxyModel::ProxyModel(QObject *parent): QSortFilterProxyModel(parent),
    text_search("")
{
}

ProxyModel::ProxyModel(mode _md, QObject *parent):
QSortFilterProxyModel(parent),text_search(""),md(_md)
{



}

void ProxyModel::search(QString s){

    if(text_search!=s)
    text_search=s;
    invalidateFilter();

}
void ProxyModel::search(QVariantList var_list){

    if(var_list.size()!=2)
        return;

    colom =  var_list.at(0).toInt();
    text_search = var_list.at(1).toString();

    invalidateFilter();


}

bool ProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{
//smart mode will search title and singer
    if(md==ProxyModel::smart){
    QModelIndex indG = sourceModel()->index(source_row, 1, source_parent);
    QModelIndex indD = sourceModel()->index(source_row, 2, source_parent);
    return sourceModel()->data(indG).toString().contains(text_search, Qt::CaseInsensitive) ||
            sourceModel()->data(indD).toString().contains(text_search, Qt::CaseInsensitive) ;
    }

    if(md==ProxyModel::column){
        QModelIndex indG = sourceModel()->index(source_row, colom+1, source_parent);
         return sourceModel()->data(indG).toString().contains(text_search, Qt::CaseInsensitive);



    }
    return false;

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
    if(role==sortRole)
        return QVariant().toInt();

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

void ProxyModel::reset(){
    colom =  0;
    text_search ="";
    invalidateFilter();


}
