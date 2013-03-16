/******************************************************************************
Copyright (c) 2013, James Baker
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	* Neither the name of the author(s) nor the
	  names of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL JAMES BAKER BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

#include "polygon.h"

FractalPolygon::FractalPolygon(QObject *parent, QColor color, QColor bgColor) :
	QObject(parent)
{
	m_color = color;
	m_fillColor = bgColor;
}


FractalPolygon::FractalPolygon(const FractalPolygon & oldPolygon) :
	QObject(oldPolygon.parent())
{
	m_color = oldPolygon.color();
	m_fillColor = oldPolygon.fillColor();
	for(int i = 0; i < oldPolygon.lines.count(); i++){
		lines.append(oldPolygon.lines[i]);
	}
}

FractalPolygon::FractalPolygon(QObject *parent, QJsonObject obj) :
	QObject(parent)
{
	m_color = JSONColor(obj["color"].toObject());
	m_fillColor = JSONColor(obj["fillcolor"].toObject());
	QJsonArray arr = obj["lines"].toArray();
	for(int i = 0; i < arr.count(); i++){
		FractalLine newLine(this, arr[i].toObject());
		lines.append(newLine);
	}
}

FractalPolygon::~FractalPolygon()
{
	int c = lines.count();
	for(int i = 0; i < c; i++){
		lines.remove(lines.count() - 1);
	}
}

FractalPolygon & FractalPolygon::operator= (const FractalPolygon & rhs){
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

void FractalPolygon::addPoint(QPointF newPoint)
{
	FractalLine newLine(this, m_color);
	newLine.setStart(newPoint);
	newLine.setEnd(QPointF(newPoint.x() + 0.0, newPoint.y() + 0.0));
	if(lines.count()){
		lines.last().setEnd(newPoint);
	}
	lines.append(newLine);
}

void FractalPolygon::setEndPoint(QPointF newPoint)
{
	if(lines.count()){
		lines.last().setEnd(newPoint);
	}
}

void FractalPolygon::paint(QPainter &painter, bool bFill, bool bOutline)
{
	QPolygonF pLine;
	for(int i = 0; i < lines.count(); i++){
		pLine << lines[i].polyLine();
	}
	if(complete() && bFill){
		if(bOutline){
			painter.setPen(QPen(m_color, 1, Qt::SolidLine));
		} else {
			painter.setPen(QPen(m_color, 1, Qt::NoPen));
		}
		painter.setBrush(QBrush(m_fillColor, Qt::SolidPattern));
		painter.drawPolygon(pLine);
	} else {
		painter.setPen(QPen(m_color, 1, Qt::SolidLine));
		painter.setBrush(QBrush(m_fillColor, Qt::NoBrush));
		painter.drawPolyline(pLine);
	}
}

void FractalPolygon::setColor(QColor color)
{
	m_color = color;
	for(int i = 0; i < lines.count(); i++){
		lines[i].setColor(m_color);
	}
	emit colorChanged(color);
}

QRectF FractalPolygon::boundingRect()
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

QPointF FractalPolygon::closestTo(QPointF testPoint)
{
	QPointF ret;
	if(lines.count()){
		ret = lines[0].closestTo(testPoint);
		qreal minDist = distance(testPoint, lines[0].closestTo(testPoint));
		for(int i = 1; i < lines.count(); i++){
			//if(lines[i].isNear(testPoint, SNAP_DISTANCE)){
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

QJsonObject FractalPolygon::save(){
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
