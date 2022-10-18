#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mUi(new Ui::MainWindow) {
  mUi->setupUi(this);
  // Init mMathModel.
  mMathModel = std::make_unique<FireTableModel>(Constans::Plot::countCell);

  setupPlot();
  setupTable();
  // Make group invisible. It appeare only, when 'increasePlotCountBox' is
  // checked.
  mUi->countCellWidget->setVisible(false);
  mUi->increasePlotCountBox->setVisible(false);
  // Connections
  connect(mUi->tableView, &QTableView::clicked, this,
          &MainWindow::changeStepCell);

  connect(mUi->stepButton, &QAbstractButton::clicked, this,
          &MainWindow::makeStep);

  connect(mUi->interpolatePlotBox, &QCheckBox::toggled, [&](bool checked) {
    mColorMap->setInterpolate(checked);
    mUi->plot->replot();
  });

  connect(mUi->resetButton, &QAbstractButton::clicked, this,
          &MainWindow::syncPlotWithTable);

  connect(mUi->increasePlotCountBox, &QCheckBox::toggled, [&](bool checked) {
    if (checked)
      mColorMap->data()->setSize(Constans::Plot::increasedCountCell,
                                 Constans::Plot::increasedCountCell);
    else {
      mColorMap->data()->setSize(Constans::Plot::countCell,
                                 Constans::Plot::countCell);
      mUi->countCellSpinBox->setValue(1);
    }

    mUi->countCellWidget->setVisible(checked);
    syncPlotWithTable();
    mUi->plot->replot();
  });

  // Timer connections.
  connect(mUi->timeIntervalSpinBox, qOverload<int>(&QSpinBox::valueChanged),
          &mTimer, qOverload<int>(&QTimer::setInterval));

  connect(mUi->runButton, &QAbstractButton::toggled,
          [&](bool checked) { (checked) ? mTimer.start() : mTimer.stop(); });

  connect(&mTimer, &QTimer::timeout, [&]() {
    makeStep();
    mTimer.start(mUi->timeIntervalSpinBox->value());
  });
}

void MainWindow::setupPlot() {
  auto plot = mUi->plot;
  // Enable opengl. Experemental function.
  if (plot->openGl())
    plot->setOpenGl(true);

  // xAxis2 because
  mColorMap = new QCPColorMap(plot->xAxis, plot->yAxis);
  plot->yAxis->setRangeReversed(true);
  // Interpolate improve quality.
  mColorMap->setInterpolate(false);
  mColorMap->setTightBoundary(true);
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
  mColorScale = new QCPColorScale(plot);
  plot->plotLayout()->addElement(0, 1, mColorScale);
  mColorScale->setType(QCPAxis::atRight);
  mColorMap->setColorScale(mColorScale);
  mColorScale->setGradient(QCPColorGradient::gpHot);
  mColorScale->setDataRange(
      QCPRange(Constans::Plot::minRangeValue, Constans::Plot::maxRangeValue));

  // Setup group.
  mPlotScaleGroup = new QCPMarginGroup(plot);
  plot->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop,
                                   mPlotScaleGroup);

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
  int size = (mUi->increasePlotCountBox->isChecked())
                 ? Constans::Plot::increasedCountCell
                 : Constans::Plot::countCell;
  int countCell = mUi->countCellSpinBox->value();

  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      auto index = mMathModel->index(i, j);
      mColorMap->data()->setCell(j, i, mMathModel->data(index).toInt());
    }
  }
  mUi->plot->replot();
}

void MainWindow::syncPlotWithCell(const QModelIndex &index) {
  auto [row, column] = std::make_pair(index.row(), index.column());
  mColorMap->data()->setCell(column, row, index.data().toInt());
  mUi->plot->replot();
}

void MainWindow::makeStep() {
  int size = (mUi->increasePlotCountBox->isChecked())
                 ? Constans::Plot::increasedCountCell
                 : Constans::Plot::countCell;

  std::vector<std::vector<short>> tmpMatrix(size, std::vector<short>(size, 0));

  // Copy current step to tmp.
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      tmpMatrix[i][j] = mColorMap->data()->cell(i, j);
    }
  }

  // Make step.
  int currentValue{0};
  auto data = mColorMap->data();

  auto needChange = [&currentValue](const double &value) -> bool {
    if (currentValue == 2.0 && value == 0.0)
      return true;

    return currentValue < value;
  };

  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      currentValue = std::floor(data->cell(i, j));
      bool left = (j != 0) ? needChange(tmpMatrix[i][j - 1]) : false;
      bool up = (i != 0) ? needChange(tmpMatrix[i - 1][j]) : false;

      bool right = (j != size - 1) ? needChange(tmpMatrix[i][j + 1]) : false;
      bool down = (i != size - 1) ? needChange(tmpMatrix[i + 1][j]) : false;

      if (left | right | down | up)
        data->setCell(i, j, (currentValue + 1) % 3);
    }
  }

  // Update plot.
  mUi->plot->replot();
}
