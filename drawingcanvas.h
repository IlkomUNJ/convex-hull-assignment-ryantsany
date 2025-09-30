#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

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

class DrawingCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit DrawingCanvas(QWidget *parent = nullptr);
    void setPenStyle(Qt::PenStyle style);
    void setPenWidth(int width);
    void clearCanvas();
    void findConvexHull(int& convexIteration);
    void findSlowConvexHull(int& slowConvexIteration);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QVector<QPoint> points;
    QVector<QPoint> convexHull;
    QVector<QPoint> slowConvexHull;
    Qt::PenStyle penStyle;
    int penWidth;
    bool showConvexHull;
    bool showSlowConvexHull;
    QVector<QPoint> convexHullAlgorithm(QVector<QPoint> points, int& iteration);
    QVector<QPoint> slowConvexHullAlgorithm(QVector<QPoint> points, int& iteration);
    int orientation(const QPoint& p, const QPoint& q, const QPoint& r);

signals:
    void pointAdded(int count);
};


#endif // DRAWINGCANVAS_H
