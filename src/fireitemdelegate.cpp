#include "fireitemdelegate.h"

FireItemDelegate::FireItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {}

void FireItemDelegate::paint(QPainter *painter,
                             const QStyleOptionViewItem &option,
                             const QModelIndex &index) const {
  // Load icon and change
  QIcon fireIcon{loadIcon("://icons/fire-solid.svg")};

  auto fireStep = index.data().toInt();
  switch (fireStep) {
  case 0:
    break;
  case 1: {
    fireIcon.paint(painter, option.rect);
    break;
  }
  case 2: {
    auto leftRect = option.rect;
    leftRect.setWidth(leftRect.width() / 2);

    auto rightRect = option.rect;
    auto halfWidth = rightRect.width() / 2;
    rightRect.setX(rightRect.x() + halfWidth);
    rightRect.setWidth(halfWidth);

    fireIcon.paint(painter, leftRect);
    fireIcon.paint(painter, rightRect);
    break;
  }
  }
}

QIcon FireItemDelegate::loadIcon(const QString &fileName) const {
  QPixmap pixmap("://icons/fire-solid.svg");
  auto mask =
      pixmap.createMaskFromColor(QColor("black"), Qt::MaskMode::MaskOutColor);
  pixmap.fill(QApplication::palette().text().color());
  pixmap.setMask(mask);
  return pixmap;
}
