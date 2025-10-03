#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QPainter>
#include <QMouseEvent>
#include <QPoint>
#include <QVector>
#include <QPen>
#include <QBrush>
#include "drawingcanvas.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onClearClicked();
    void onConvexHullClicked();
    void onSlowConvexHullClicked();
    void onPointCountChanged(int count);

private:
    DrawingCanvas *canvas;
    QPushButton *clearButton;
    QPushButton *convexHullButton;
    QPushButton *slowConvexHullButton;
    QLabel *slowConvexHullIterationLabel;
    QLabel *convexHullIterationLabel;
    QLabel *instructionLabel;
};

#endif // MAINWINDOW_H
