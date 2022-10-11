#pragma once

#include "constans.hpp"
#include <QAbstractTableModel>
#include <QChar>
#include <QDebug>
#include <QVector>

class FireTableModel : public QAbstractTableModel {
  Q_OBJECT

public:
  explicit FireTableModel(int size, QObject *parent = nullptr);

  /**
   * @brief data
   * @param index
   * @param role
   * @return
   */
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  /**
   * @brief setData
   * @param index
   * @param value
   * @param role
   * @return
   */
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) override;

  /**
   * @brief rowCount
   * @param parent
   * @return
   */
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  /**
   * @brief columnCount
   * @param parent
   * @return
   */
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
  bool itCorrectIndex(const QModelIndex &index) const;

private:
  QVector<QVector<int>> mMatrix;
};
