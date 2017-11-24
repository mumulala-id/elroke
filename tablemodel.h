#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>


class tablemodel : public QAbstractTableModel
{
public:
    tablemodel(QObject * parent = 0);
   // tablemodel(const QList<QString> &m_videoList, QObject *parent = 0);
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
void update(QList<QString> list);


private :
  QList<QString> m_videoList;
  QString m_title;
  QString m_singer;
 // const QStringList &filename;
};

#endif // TABLEMODEL_H
