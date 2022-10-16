#pragma once

#include <QApplication>
#include <QBitmap>
#include <QColor>
#include <QPainter>
#include <QStyledItemDelegate>

class FireItemDelegate : public QStyledItemDelegate {
  Q_OBJECT
public:
  FireItemDelegate(QObject *parent = nullptr);

  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const override;

private:
  QIcon loadIcon(const QString &fileName) const;
};
