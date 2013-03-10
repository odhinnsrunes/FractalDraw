#include "polygon.h"

Polygon::Polygon(QObject *parent, QColor color, QColor bgColor) :
	QObject(parent)
{
	m_color = color;
	m_fillColor = bgColor;
}


Polygon::Polygon(const Polygon & oldPolygon) :
	QObject(oldPolygon.parent())
{
	m_color = oldPolygon.color();
	m_fillColor = oldPolygon.fillColor();
	for(int i = 0; i < oldPolygon.lines.count(); i++){
		lines.append(oldPolygon.lines[i]);
	}
}

Polygon::~Polygon()
{
	int c = lines.count();
	for(int i = 0; i < c; i++){
		lines.remove(lines.count() - 1);
	}
}

Polygon & Polygon::operator= (const Polygon & rhs){
	if (this == &rhs)
		  return *this;
	setParent(rhs.parent());
	m_color = rhs.color();
	m_fillColor = rhs.fillColor();
	for(int i = 0; i < rhs.lines.count(); i++){
		lines.append(rhs.lines[i]);
	}
	return *this;
}

void Polygon::addPoint(QPointF newPoint)
{
	Line newLine(this, m_color);
	newLine.setStart(newPoint);
	newLine.setEnd(QPointF(newPoint.x() + 0.0, newPoint.y() + 0.0));
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
		painter.setBrush(QBrush(m_fillColor, Qt::SolidPattern));
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
	emit colorChanged(color);
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
