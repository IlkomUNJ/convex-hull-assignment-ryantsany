#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Tugas Komgraf Convex Hull Algorithm");
    setMinimumSize(1000, 700);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QHBoxLayout *controlLayout = new QHBoxLayout();

    clearButton = new QPushButton("Clear Canvas");
    convexHullButton = new QPushButton("Find Convex Hull");
    slowConvexHullButton = new QPushButton("Find Slow Convex Hull");

    slowConvexHullIterationLabel = new QLabel("Slow Convex Hull Iteration: 0");
    slowConvexHullIterationLabel->setStyleSheet("font-weight: bold;");
    convexHullIterationLabel = new QLabel("Convex Hull Iteration: 0");
    convexHullIterationLabel->setStyleSheet("font-weight: bold;");

    controlLayout->addStretch(1);
    controlLayout->addWidget(clearButton);
    controlLayout->addWidget(slowConvexHullButton);
    controlLayout->addWidget(convexHullButton);
    controlLayout->addStretch();
    controlLayout->addWidget(slowConvexHullIterationLabel);
    controlLayout->addWidget(convexHullIterationLabel);
    controlLayout->addStretch(1);

    canvas = new DrawingCanvas(this);
    mainLayout->addWidget(canvas);
    mainLayout->addLayout(controlLayout);

    connect(clearButton, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    connect(convexHullButton, &QPushButton::clicked, this, &MainWindow::onConvexHullClicked);
    connect(slowConvexHullButton, &QPushButton::clicked, this, &MainWindow::onSlowConvexHullClicked);
    connect(canvas, &DrawingCanvas::pointAdded, this, &MainWindow::onPointCountChanged);
}

MainWindow::~MainWindow()
{
}


void MainWindow::onClearClicked()
{
    canvas->clearCanvas();
    convexHullIterationLabel->setText(QString("Convex Hull Iteration: %1").arg(0));
    slowConvexHullIterationLabel->setText(QString("Slow Convex Hull Iteration: %1").arg(0));
}

void MainWindow::onConvexHullClicked()
{
    int convexHullIteration = 0;

    canvas->findConvexHull(convexHullIteration);

    convexHullIterationLabel->setText(QString("Convex Hull Iteration: %1").arg(convexHullIteration));
}

void MainWindow::onSlowConvexHullClicked()
{
    int slowConvexHullIteration = 0;

    canvas->findSlowConvexHull(slowConvexHullIteration);

    slowConvexHullIterationLabel->setText(QString("Slow Convex Hull Iteration: %1").arg(slowConvexHullIteration));
}

void MainWindow::onPointCountChanged(int count)
{
    convexHullButton->setEnabled(count >= 3);
}
