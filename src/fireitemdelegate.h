#pragma once

#include <QPainter>
#include <QStyledItemDelegate>

class FireItemDelegate : public QStyledItemDelegate {
  Q_OBJECT
public:
  FireItemDelegate(QObject *parent = nullptr);

  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const override;
};
