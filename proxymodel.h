#ifndef PROXYMODEL_H
#define PROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QMap>

class ProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QMap<int, Qt::Alignment>alignMap;

public:
      enum mode{column,smart,fixed,tobefixed,favorite,languageGenre};
      enum role{sortRole=Qt::UserRole};

    explicit ProxyModel(QObject* parent = nullptr);
    explicit ProxyModel(mode, QObject *parent=nullptr);
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

public slots:
  void search(QString);
//  void search(QVariantList);
  void fixSearchByColumn(int column, const QString &fixText);
  void searchByColumn(int column, const QString &text);
  void setAlignment(unsigned int, Qt::Alignment);
  void toBeFixed();
  void filterFavorite();
  void filterByLanguageGenre(const QString &);
  void reset();

private slots:
  void clearAlign();

private:
    QString text_search;
    unsigned short int _colom=0;
    mode md;

protected :
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

};

#endif // PROXYMODEL_H
