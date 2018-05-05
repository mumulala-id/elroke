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

       text_search=s.trimmed();
    md = ProxyModel::smart;
    invalidateFilter();

}

void ProxyModel::filterByLanguageGenre(const QString &text){

    md = ProxyModel::languageGenre;
    text_search = text;
    invalidateFilter();
}

void ProxyModel::searchByColumn(int column, const QString &text)
{
    md = ProxyModel::column;
    text_search = text;
    _colom = column;
    invalidateFilter();
}

void ProxyModel::fixSearchByColumn(int column, const QString &fixText)
{
    md = ProxyModel::fixed;
    text_search = fixText;
    _colom = column;
    invalidateFilter();

}

bool ProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
//smart mode will search title and singer
    if(md==ProxyModel::smart)
    {
    QModelIndex indG = sourceModel()->index(source_row, 1, source_parent);
    QModelIndex indD = sourceModel()->index(source_row, 2, source_parent);
    return sourceModel()->data(indG).toString().contains(text_search, Qt::CaseInsensitive) ||
            sourceModel()->data(indD).toString().contains(text_search, Qt::CaseInsensitive) ;
    }

    else if(md==ProxyModel::column)
    {
        QModelIndex indG = sourceModel()->index(source_row, _colom, source_parent);
         return sourceModel()->data(indG).toString().contains(text_search, Qt::CaseInsensitive);
    }
    else if(md==ProxyModel::fixed)
    {
        QModelIndex indG = sourceModel()->index(source_row, _colom, source_parent);
        int x = QString::compare(text_search,sourceModel()->data(indG).toString(),Qt::CaseInsensitive);
        if(x==0) return true;
        return false;

    }
    else if(md==ProxyModel::tobefixed)
    {
        auto the_string = [this,source_row,source_parent](int col){
        QModelIndex indG = sourceModel()->index(source_row, col, source_parent);
        return sourceModel()->data(indG).toString();
        };

        QString title = the_string(1);
        QString singer = the_string(2);
        QString lang = the_string(3);
        QString genre = the_string(4);
        QString audio = the_string(5);
        bool a =
        QString::compare(audio,"LEFT",Qt::CaseInsensitive)==0||
        QString::compare(audio,"RIGHT",Qt::CaseInsensitive)==0||
        QString::compare(audio, "STEREO", Qt::CaseInsensitive)==0;

        QString u ="UNKNWON";
        if(title==u
           || singer==u
           || genre==u
           || lang==u
           || audio==u
           || !a){
            return true;
        } else {
            return false;
        }

    }
    else if(md == ProxyModel::favorite)
    {
        QModelIndex ind = sourceModel()->index(source_row,9,source_parent);
        QString x = sourceModel()->data(ind).toString();

        return QString::compare(x,"YES",Qt::CaseInsensitive)==0;

    }
    else if(md == ProxyModel::languageGenre)
    {

        QModelIndex inedx_lang = sourceModel()->index(source_row,3,source_parent);
        QString lang = sourceModel()->data(inedx_lang).toString();

        QModelIndex inedx_genre = sourceModel()->index(source_row,4,source_parent);
        QString genre = sourceModel()->data(inedx_genre).toString();

      return QString::compare(text_search, lang,Qt::CaseInsensitive)==0 || QString::compare(text_search,genre,Qt::CaseInsensitive)==0;

    }

    else if(md == ProxyModel::Date){
        QModelIndex index= sourceModel()->index(source_row,8,source_parent);
        QString s_date =sourceModel()->data(index).toString();
        QDate date = QDate::fromString(s_date,"yyyy-MM-dd");
        return dateInRange(date);
    }
    return true;
}

QVariant ProxyModel::headerData(int section, Qt::Orientation orientation, int role) const {

    if(orientation == Qt::Horizontal){
        if(role == Qt::DisplayRole)

            switch (section) {
            case 0: return tr("ID");    break;
            case 1: return tr("TITLE"); break;
            case 2: return tr("SINGER"); break;
            case 3: return tr("LANGUAGE"); break;
            case 4: return tr("GENRE"); break;
            case 5 : return tr("AUDIO"); break;
            case 6 : return tr("HITS"); break;
            case 7 : return tr("PATH"); break;
            case 8 : return tr("DATE"); break;
            case 9 : return tr("FAVORITE"); break;
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

void ProxyModel::reset(){
//    _colom =  0;
//    text_search ="";
//    invalidateFilter();
}
void ProxyModel::toBeFixed(){

    md=ProxyModel::tobefixed;
    invalidateFilter();

}

void ProxyModel::filterFavorite()
{
    md = ProxyModel::favorite;
    invalidateFilter();
}

void ProxyModel::setLimitMonth(int month){

    QDate d = QDate::currentDate();
    limit_date = d.addMonths(-month);
}

bool ProxyModel::dateInRange(QDate d) const
{
    return (d > limit_date && d < QDate::currentDate());
}

void ProxyModel::filterByDate()
{
    md=ProxyModel::Date;
    invalidateFilter();
}
