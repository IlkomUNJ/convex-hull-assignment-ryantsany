#include<QMessageBox>
#include "drawingcanvas.h"

DrawingCanvas::DrawingCanvas(QWidget *parent)
    : QWidget(parent), penStyle(Qt::SolidLine), penWidth(2), showConvexHull(false)
{
    setMinimumSize(800, 600);
    setStyleSheet("background-color: white; border: 1px solid black;");
    setMouseTracking(true);
}

void DrawingCanvas::setPenStyle(Qt::PenStyle style)
{
    penStyle = style;
    update();
}

void DrawingCanvas::setPenWidth(int width)
{
    penWidth = width;
    update();
}

void DrawingCanvas::clearCanvas()
{
    points.clear();
    convexHull.clear();
    showConvexHull = false;
    showSlowConvexHull = false;
    update();
    emit pointAdded(0);
}

void DrawingCanvas::findConvexHull(int& convexIteration)
{
    convexHull = convexHullAlgorithm(points, convexIteration);
    showSlowConvexHull = false;
    showConvexHull = true;
    update();
}

void DrawingCanvas::findSlowConvexHull(int& slowConvexIteration)
{
    slowConvexHull = slowConvexHullAlgorithm(points, slowConvexIteration);
    showConvexHull = false;
    showSlowConvexHull = true;
    update();
}

void DrawingCanvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pointPen(Qt::blue, 6, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(pointPen);

    for (const QPoint &point : points) {
        painter.drawPoint(point);
    }

    painter.setPen(QPen(Qt::white, 1));
    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont(font);

    for (int i = 0; i < points.size(); ++i) {
        QRect textRect(points[i].x() + 10, points[i].y() - 10, 30, 20);
        painter.drawText(textRect, Qt::AlignLeft, QString::number(i + 1));
    }

    if (showSlowConvexHull && slowConvexHull.size() >= 3) {
        QPen hullPen(Qt::red, penWidth, penStyle, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(hullPen);

        for (int i = 0; i < slowConvexHull.size(); ++i) {
            QPoint start = slowConvexHull[i];
            QPoint end = slowConvexHull[(i + 1) % slowConvexHull.size()];
            painter.drawLine(start, end);
        }

        QPen hullVertexPen(Qt::red, 8, Qt::SolidLine, Qt::RoundCap);
        painter.setPen(hullVertexPen);
        for (const QPoint &point : slowConvexHull) {
            painter.drawPoint(point);
        }
    }

    if (showConvexHull && convexHull.size() >= 3) {
        QPen hullPen(Qt::green, penWidth, penStyle, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(hullPen);

        for (int i = 0; i < convexHull.size(); ++i) {
            QPoint start = convexHull[i];
            QPoint end = convexHull[(i + 1) % convexHull.size()];
            painter.drawLine(start, end);
        }

        QPen hullVertexPen(Qt::green, 8, Qt::SolidLine, Qt::RoundCap);
        painter.setPen(hullVertexPen);
        for (const QPoint &point : convexHull) {
            painter.drawPoint(point);
        }
    }
}

void DrawingCanvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        points.append(event->pos());
        showConvexHull = false;
        convexHull.clear();
        update();
        emit pointAdded(points.size());
    }
}

QVector<QPoint> DrawingCanvas::slowConvexHullAlgorithm(QVector<QPoint> points, int& iteration)
{
    iteration = 0;
    int n = points.size();
    if (n < 3) return QVector<QPoint>();

    QVector<QPair<QPoint, QPoint>> edges;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) continue;

            bool valid = true;

            for (int k = 0; k < n; k++) {
                iteration++;
                if (k == i || k == j){
                    continue;
                }

                if (orientation(points[i], points[j], points[k]) > 0) {
                    valid = false;
                    break;
                }
            }

            if (valid) {
                edges.append(qMakePair(points[i], points[j]));
            }
        }
    }

    if (edges.isEmpty()) return QVector<QPoint>();

    QVector<QPoint> hull;
    QPoint start = edges[0].first;
    QPoint current = start;

    do {
        hull.append(current);

        QPoint next;
        for (const auto& edge : edges) {
            if (edge.first == current) {
                next = edge.second;
                break;
            }
        }
        current = next;

    } while (current != start && hull.size() < n);

    return hull;
}

QVector<QPoint> DrawingCanvas::convexHullAlgorithm(QVector<QPoint> points, int& iteration)
{
    iteration = 0;
    int n = points.size();
    if (n < 3) return QVector<QPoint>();


    std::sort(points.begin(), points.end(), [](const QPoint &a, const QPoint &b) {
        return (a.x() < b.x()) || (a.x() == b.x() && a.y() < b.y());
    });

    QVector<QPoint> l_upper, l_lower, hull;

    for (int i = 0; i < n; i++) {
        while (l_upper.size() >= 2 && orientation(l_upper[l_upper.size()-2], l_upper.last(), points[i]) <= 0) {
            l_upper.pop_back();
            iteration++;
        }
        l_upper.append(points[i]);
        iteration++;
    }

    qDebug() << "upper:" << l_upper;

    for (int i = n - 1; i >= 0; i--) {
        while (l_lower.size() >= 2 && orientation(l_lower[l_lower.size()-2], l_lower.last(), points[i]) <= 0) {
            l_lower.pop_back();
            iteration++;
        }
        l_lower.append(points[i]);
        iteration++;
    }

    qDebug() << "lower:" << l_lower;

    l_upper.pop_back();
    l_lower.pop_back();

    hull = l_upper + l_lower;


    qDebug() << "hasil:" << hull;
    return hull;
}

int DrawingCanvas::orientation(const QPoint& p, const QPoint& q, const QPoint& r)
{
    return (q.x() - p.x()) * (r.y() - p.y()) - (q.y() - p.y()) * (r.x() - p.x());
}
