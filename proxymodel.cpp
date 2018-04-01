#include "proxymodel.h"
#include <QDebug>
#include <QVariant>

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
//void ProxyModel::search(QVariantList var_list)
//{
//    if(var_list.size()!=2)
//        return;

//    colom =  var_list.at(0).toInt();
//    text_search = var_list.at(1).toString();

//    invalidateFilter();
//}

bool ProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
//smart mode will search title and singer
//    if(md==ProxyModel::smart)
//    {
    QModelIndex indG = sourceModel()->index(source_row, 1, source_parent);
    QModelIndex indD = sourceModel()->index(source_row, 2, source_parent);
    return sourceModel()->data(indG).toString().contains(text_search, Qt::CaseInsensitive) ||
            sourceModel()->data(indD).toString().contains(text_search, Qt::CaseInsensitive) ;
//    }

//    if(md==ProxyModel::column)
//    {
//        QModelIndex indG = sourceModel()->index(source_row, colom+1, source_parent);
//         return sourceModel()->data(indG).toString().contains(text_search, Qt::CaseInsensitive);
//    }
    return false;
}

QVariant ProxyModel::headerData(int section, Qt::Orientation orientation, int role) const {

    if(orientation == Qt::Horizontal){
        if(role == Qt::DisplayRole)

            switch (section) {
            case 0: return tr("ID");    break;
            case 1: return tr("TITLE"); break;
            case 2: return tr("SINGER"); break;
            case 3: return tr("LANGUAGE"); break;
            case 4: return tr("CATEGORY"); break;
            case 5 : return tr("CHANNEL"); break;
            case 6 : return tr("PLAYTIMES"); break;
            case 7 : return tr("PATH"); break;
            case 8 : return tr("DATE"); break;
            default:
                return QString(tr("COLUMN %1").arg(section+1));
                break;
            }
    }
//    return sourceModel()->headerData(section, orientation, role);
    return QVariant();
}

QVariant ProxyModel::data(const QModelIndex &index, int role) const
{
    if(role==Qt::TextAlignmentRole)
    {
        if(alignMap.contains(-1))
            return QVariant(alignMap.value(-1));

        if(alignMap.contains(index.column()))
        {
            return QVariant(alignMap.value(index.column()));
        }

    }
    if(role==sortRole)
        return QVariant().toInt();

    return QSortFilterProxyModel::data(index, role);
}

void ProxyModel::setAlignment(unsigned int index, Qt::Alignment flag)
{
    if(alignMap.contains(-1))
        clearAlign();

    alignMap.insert(index, flag);
}

void ProxyModel::clearAlign()
{
    alignMap.clear();
}

bool ProxyModel::lessThan(const QModelIndex &left,
                                      const QModelIndex &right) const
{
    QVariant leftData = sourceModel()->data(left);
    QVariant rightData = sourceModel()->data(right);

    if (leftData.type() == QVariant::Int)
    {
           return leftData.toInt() < rightData.toInt();
    }
    else{
        QString leftString = leftData.toString();
//               if(left.column() == 1 && emailPattern.indexIn(leftString) != -1)
//                   leftString = emailPattern.cap(1);

               QString rightString = rightData.toString();
//               if(right.column() == 1 && emailPattern.indexIn(rightString) != -1)
//                   rightString = emailPattern.cap(1);

               return leftString < rightString;
    }
}
void ProxyModel::reset(){
    colom =  0;
    text_search ="";
    invalidateFilter();
}
