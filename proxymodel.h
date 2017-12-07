#ifndef PROXYMODEL_H
#define PROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QMap>

class ProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QMap<int, Qt::Alignment>alignMap;

public:
      enum mode{column,smart};
    ProxyModel(QObject* parent = 0);
    ProxyModel(mode, QObject *parent=0);
    bool filterAcceptsRow(int source_row,
                          const QModelIndex &source_parent) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;


public slots:
    void search(QString);
    void search(QVariantList);
  void setAlignment(unsigned int, Qt::Alignment);

private slots:
  void clearAlign();
  //void setAlignment(unsigned int, Qt::ItemFlags);

private:
    QString text_search;
 int colom=0;

 mode md;
};

#endif // PROXYMODEL_H
