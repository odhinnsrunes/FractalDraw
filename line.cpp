#include "line.h"
#include <QDebug>
#include <QMatrix>
#include <QDebug>

Line::Line(QObject *parent, QColor newColor, unsigned int uiSeed) :
	QObject(parent)
{
	m_seed = uiSeed;
	m_start = QPointF(0, 0);
	m_end = QPointF(0, 10);
	m_color = newColor;
}

Line::Line(const Line & oldLine) :
	QObject(oldLine.parent())
{
	m_seed = oldLine.seed();
	m_start = oldLine.start();
	m_end = oldLine.end();
	m_color = oldLine.color();
}

Line::Line(QObject *parent, QPointF newStart, QPointF newEnd, QColor newColor, unsigned int uiSeed):
	QObject(parent)
{
	m_seed = uiSeed;
	m_start = newStart;
	m_end = newEnd;
	m_color = newColor;
}

QPointF midPoint(QPointF point1, QPointF point2)
{
	QPointF ret((point1.x() + point2.x()) / 2.0, (point1.y() + point2.y()) / 2.0);
	return ret;
}

qreal distance(QPointF point1, QPointF point2)
{
	qreal x = fabs(point2.x() - point1.x());
	qreal y = fabs(point2.y() - point1.y());
	return qSqrt((x * x) + (y * y));
}

qreal rotation(QPointF point1, QPointF point2)
{
	qreal dx = point2.x() - point1.x();
	qreal dy = point2.y() - point1.y();
	return qAtan2(dy, dx) * 180 / 3.141592;
}

QPolygonF Line::polyLine() const
{
	QPolygonF ret;
	ret << QPointF(0, 0) << QPointF(distance(m_start, m_end), 0);
	//ret.setPoints(2, 0, 0, distance(m_start, m_end), 0);
	qsrand(1234);
	qsrand(m_seed);
	unsigned int m_iterations = (unsigned int)(qSqrt(distance(m_start, m_end)) / 2.0);
	if(m_iterations > 12)
		m_iterations = 12;
	for(unsigned int i = 0; i < m_iterations; i++){
		for(unsigned int j = ret.count() - 1; j > 0; j--){
			QPointF point1 = ret.at(j - 1);
			QPointF point2 = ret.at(j);
			QPointF point3 = midPoint(point1, point2);
			qreal dist = distance(point1, point2) / 2;
			qreal offset = (qrand() * (dist / (qreal)RAND_MAX)) - (dist / 2.0);
			point3.setY(point3.y() + offset);
			ret.insert(j, point3);
		}
	}

	QMatrix matrix1;
	QMatrix matrix2;
	qreal rot = rotation(m_start, m_end);
	//qDebug() << rot << "\n" << m_start;
	matrix1.rotate(rot);
	matrix2.translate(m_start.x(), m_start.y());
	ret = matrix1.map(ret);
	ret = matrix2.map(ret);
	return ret;
}

void Line::paint(QPainter &painter) const
{
	QPolygonF pLine = polyLine();
	painter.setPen(QPen(m_color, 1, Qt::SolidLine));
	painter.drawPolyline(pLine);
}
