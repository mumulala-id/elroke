#ifndef PROXYMODEL_H
#define PROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QMap>

class ProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QMap<int, Qt::Alignment>alignMap;
public:
    ProxyModel(QObject* parent = 0);
    bool filterAcceptsRow(int source_row,
                          const QModelIndex &source_parent) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;

public slots:
    void search(QString);
  void setAlignment(unsigned int, Qt::Alignment);

private slots:
  void clearAlign();
  //void setAlignment(unsigned int, Qt::ItemFlags);

private:
    QString text_search;
  //  QString m_kapal;
};

#endif // PROXYMODEL_H
