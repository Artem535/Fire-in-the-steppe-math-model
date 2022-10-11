#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mUi(new Ui::MainWindow) {
  mUi->setupUi(this);
  // Init mMathModel.
  mMathModel = std::make_unique<FireTableModel>(Constans::Plot::countCell);

  setupPlot();
  setupTable();

  connect(mUi->tableView, &QTableView::clicked, this,
          &MainWindow::changeStepCell);
}

void MainWindow::setupPlot() {
  auto plot = mUi->plot;
  // Enable opengl. Experemental function.
  if (plot->openGl())
    plot->setOpenGl(true);

  mColorMap = std::make_unique<QCPColorMap>(plot->xAxis, plot->yAxis);
  // Interpolate improve quality.
  mColorMap->setInterpolate(true);
  // Setup data initial value - 1.
  mColorMap->data()->setSize(Constans::Plot::countCell,
                             Constans::Plot::countCell);
  {
    auto range = QCPRange{0, Constans::Plot::countCell};
    mColorMap->data()->setRange(range, range);

    for (int i = 0; i < Constans::Plot::countCell; ++i) {
      for (int j = 0; j < Constans::Plot::countCell; ++j) {
        mColorMap->data()->setCell(i, j, Constans::Plot::minRangeValue);
      }
    }
  }

  // Setup scale.
  mColorScale = std::make_unique<QCPColorScale>(plot);
  plot->plotLayout()->addElement(0, 1, mColorScale.get());
  mColorScale->setType(QCPAxis::atRight);
  mColorMap->setColorScale(mColorScale.get());
  mColorScale->setGradient(QCPColorGradient::gpHot);
  mColorScale->setDataRange(
      QCPRange(Constans::Plot::minRangeValue, Constans::Plot::maxRangeValue));

  // Setup group.
  mPlotScaleGroup = std::make_unique<QCPMarginGroup>(plot);
  plot->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop,
                                   mPlotScaleGroup.get());

  mColorMap->rescaleDataRange();
  plot->rescaleAxes(true);
  plot->replot();
}

void MainWindow::setupTable() {
  auto table = mUi->tableView;
  table->setModel(mMathModel.get());
  table->setItemDelegate(new FireItemDelegate);

  table->horizontalHeader()->setMinimumWidth(Constans::Table::minWidthCell);
  table->horizontalHeader()->setDefaultSectionSize(
      Constans::Table::minWidthCell);
  table->horizontalHeader()->resizeSections(QHeaderView::Fixed);

  table->verticalHeader()->setMinimumHeight(Constans::Table::minHeightCell);
  table->verticalHeader()->setDefaultSectionSize(
      Constans::Table::minHeightCell);
  table->verticalHeader()->resizeSections(QHeaderView::Fixed);
}

void MainWindow::changeStepCell(const QModelIndex &index) {
  auto currentState = mMathModel->data(index);
  mMathModel->setData(index, currentState.toInt() + 1, Qt::DisplayRole);
  syncPlotWithCell(index);
}

void MainWindow::syncPlotWithTable() {
  for (int i = 0; i < Constans::Table::countCell; ++i) {
    for (int j = 0; j < Constans::Table::countCell; ++j) {
      auto index = mMathModel->index(i, j);
      mColorMap->data()->setCell(i, j, mMathModel->data(index).toInt());
    }
  }
  mUi->plot->replot();
}

void MainWindow::syncPlotWithCell(const QModelIndex &index) {
  auto [row, column] = std::make_pair(index.row(), index.column());
  auto t = index.data().toInt();
  mColorMap->data()->setCell(column, row, index.data().toInt());
  mUi->plot->replot();
}
