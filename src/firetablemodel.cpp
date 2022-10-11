#include "firetablemodel.h"

FireTableModel::FireTableModel(int size, QObject *parent)
    : QAbstractTableModel{parent} {
  mMatrix = QVector<QVector<int>>(size, QVector<int>(size, 0));
}

bool FireTableModel::itCorrectIndex(const QModelIndex &index) const {
  auto row = index.row();
  auto column = index.column();

  bool rowCorrect = (row >= 0) && (row < mMatrix.size());
  return rowCorrect && (column >= 0) && (column < mMatrix[0].size());
}

QVariant FireTableModel::data(const QModelIndex &index, int role) const {

  if (!itCorrectIndex(index))
    return QVariant();

  return mMatrix[index.row()][index.column()];
}

bool FireTableModel::setData(const QModelIndex &index, const QVariant &value,
                             int role) {
  if (!itCorrectIndex(index))
    return false;

  mMatrix[index.row()][index.column()] = value.toInt() % 3;
  emit dataChanged(index, index, {role});

  return true;
}

int FireTableModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;

  return mMatrix.size();
}

int FireTableModel::columnCount(const QModelIndex &parent) const {
  if (parent.isValid() || mMatrix.isEmpty())
    return 0;

  return mMatrix[0].size();
}
