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
	QPolygonF pLine;
	for(int i = 0; i < lines.count(); i++){
		pLine << lines[i].polyLine();
	}
	painter.setPen(QPen(m_color, 1, Qt::SolidLine));
	if(complete()){
		painter.drawPolygon(pLine);
	} else {
		painter.drawPolyline(pLine);
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

QRectF Polygon::boundingRect()
{
	if(lines.count() == 0)
		return QRect(0.0, 0.0, 0.0, 0.0);
	QRectF ret(1000000.0, 1000000.0, -1000001.0, -1000001.0);
	for(int i = 0; i < lines.count(); i++){
		QRectF thisRect = lines[i].boundingRect();
		if(ret.top() > thisRect.top())
			ret.setTop(thisRect.top());
		if(ret.left() > thisRect.left())
			ret.setLeft(thisRect.left());
		if(ret.bottom() < thisRect.bottom())
			ret.setBottom(thisRect.bottom());
		if(ret.right() < thisRect.right())
			ret.setRight(thisRect.right());
	}
	return ret;
}
