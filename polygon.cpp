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

void Polygon::paint(QPainter &painter, bool bFill)
{
	QPolygonF pLine;
	for(int i = 0; i < lines.count(); i++){
		pLine << lines[i].polyLine();
	}
	painter.setPen(QPen(m_color, 1, Qt::SolidLine));
	if(complete() && bFill){
		painter.setBrush(QBrush(m_fillColor, Qt::SolidPattern));
		painter.drawPolygon(pLine);
	} else {
		painter.setBrush(QBrush(m_fillColor, Qt::NoBrush));
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
		return QRectF(0.0, 0.0, 0.0, 0.0);
	QRectF ret;
	ret.setTop(1000000.0);
	ret.setLeft(1000000.0);
	ret.setBottom(-1000000.0);
	ret.setRight(-1000000.0);
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

QPointF Polygon::closestTo(QPointF testPoint)
{
	QPointF ret;
	if(lines.count()){
		ret = lines[0].closestTo(testPoint);
		qreal minDist = distance(testPoint, lines[0].closestTo(testPoint));
		for(int i = 1; i < lines.count(); i++){
			//if(lines[i].near(testPoint, SNAP_DISTANCE)){
				QPointF p = lines[i].closestTo(testPoint);
				qreal d = distance(testPoint, p);
				if(d < minDist){
					minDist = d;
					ret = p;
				}
				if(d == 0.0){
					break;
				}
			//}
		}
	}
	return ret;
}

QJsonObject Polygon::save(){
	QJsonObject obj;
	QJsonArray jlines;
	for(int i = 0; i < lines.count(); i++){
		jlines.append(lines[i].save());
	}
	obj["lines"] = jlines;
	obj["color"] = JSONColor(m_color);
	obj["fillcolor"] = JSONColor(m_fillColor);
	return obj;
}
