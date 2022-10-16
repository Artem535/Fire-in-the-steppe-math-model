#pragma once

#include "constans.hpp"
#include "fireitemdelegate.h"
#include "firetablemodel.h"
#include "qcustomplot.h"
#include <QMainWindow>
#include <QSpinBox>
#include <QTimer>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  void setupPlot();
  void setupTable();

public slots:
  void syncPlotWithTable();
  void syncPlotWithCell(const QModelIndex &index);
  void changeStepCell(const QModelIndex &index);
  void makeStep();

private:
  Ui::MainWindow *mUi;
  QTimer mTimer;

  std::unique_ptr<QCPColorMap> mColorMap;
  std::unique_ptr<QCPColorScale> mColorScale;
  std::unique_ptr<QCPMarginGroup> mPlotScaleGroup;

  std::unique_ptr<FireTableModel> mMathModel;
};
