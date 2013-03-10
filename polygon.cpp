#include "polygon.h"

Polygon::Polygon(QObject *parent) :
	QObject(parent)
{
	m_color = QColor(0, 0, 0);
}

void Polygon::addPoint(QPointF newPoint)
{
	Line newLine(this, m_color);
	newLine.setStart(newPoint);
	newLine.setEnd(QPointF(newPoint.x() + 1.0, newPoint.y() + 1.0));
	if(lines.count()){
		lines.last().setEnd(newPoint);
	}
	lines.append(newLine);
}

void Polygon::setEndPoint(QPointF newPoint)
{
	if(lines.count()){
		lines.last().setEnd(newPoint);
	}
}

void Polygon::paint(QPainter &painter)
{
	for(int i = 0; i < lines.count(); i++){
		lines.at(i).paint(painter);
	}
}

void Polygon::setColor(QColor color)
{
	m_color = color;
	for(int i = 0; i < lines.count(); i++){
		lines[i].setColor(m_color);
	}
	emit colorChanged();
}
