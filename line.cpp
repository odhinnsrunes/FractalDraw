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

#include "line.h"
#include <QDebug>
#include <QMatrix>

QJsonObject JSONColor(QColor color){
	QJsonObject obj;
	obj["red"] = color.red();
	obj["green"] = color.green();
	obj["blue"] = color.blue();
	return obj;
}

QString JSONColorString(QColor color){
	QJsonDocument doc = QJsonDocument(JSONColor(color));
	return QString(doc.toJson());
}

QColor JSONColorString(QString str){
	QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
	QColor ret;
	ret.setRed((int)doc.object()["red"].toDouble());
	ret.setGreen((int)doc.object()["green"].toDouble());
	ret.setBlue((int)doc.object()["blue"].toDouble());
	return ret;
}

QColor JSONColor(QJsonObject obj){

	QColor ret;
	ret.setRed((int)obj["red"].toDouble());
	ret.setGreen((int)obj["green"].toDouble());
	ret.setBlue((int)obj["blue"].toDouble());
	qDebug() << ret;
	return ret;
}

FractalLine::FractalLine(QObject *parent, QColor newColor, unsigned int uiSeed) :
	QObject(parent)
{
	m_seed = uiSeed;
	m_start = QPointF(0, 0);
	m_end = QPointF(0, 10);
	m_color = newColor;
}

FractalLine::FractalLine(const FractalLine & oldLine) :
	QObject(oldLine.parent())
{
	m_seed = oldLine.seed();
	m_start = oldLine.start();
	m_end = oldLine.end();
	m_color = oldLine.color();
}

FractalLine::FractalLine(QObject *parent, QPointF newStart, QPointF newEnd, QColor newColor, unsigned int uiSeed):
	QObject(parent)
{
	m_seed = uiSeed;
	m_start = newStart;
	m_end = newEnd;
	m_color = newColor;
}

FractalLine::FractalLine(QObject *parent, QJsonObject obj) :
	QObject(parent)
{
	m_seed = (unsigned int)obj["seed"].toString().toLong();
	m_start = QPointF(obj["start"].toObject()["x"].toDouble(), obj["start"].toObject()["y"].toDouble());
	m_end = QPointF(obj["end"].toObject()["x"].toDouble(), obj["end"].toObject()["y"].toDouble());
	m_color = JSONColor(obj["color"].toObject());
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

QPolygonF FractalLine::polyLine() const
{
	QPolygonF ret;
	ret << QPointF(0, 0) << QPointF(distance(m_start, m_end), 0);
	//ret.setPoints(2, 0, 0, distance(m_start, m_end), 0);
	//qsrand(1234);
	qsrand(m_seed);
	for(int i = 0; i < qrand() % 127; i++){  // don't know why I have to do this, but if I don't the first offset is always positive
		qrand();
	}
	unsigned int m_iterations = (unsigned int)(qSqrt(distance(m_start, m_end)) /1.5);

	if(m_iterations > 12)
		m_iterations = 12;
	for(unsigned int i = 0; i < m_iterations; i++){
		for(unsigned int j = ret.count() - 1; j > 0; j--){
			QPointF point1 = ret.at(j - 1);
			QPointF point2 = ret.at(j);
			QPointF point3 = midPoint(point1, point2);
			qreal dist = distance(point1, point2) / 4;
			qreal offset = -1.0 + (((qreal)qrand() / (qreal)RAND_MAX) * 2.0);
			offset *= dist;
			point3.setY(point3.y() + offset);
			ret.insert(j, point3);
		}
	}

	QMatrix matrix1;
	QMatrix matrix2;
	qreal rot = rotation(m_start, m_end);

	matrix1.rotate(rot);
	matrix2.translate(m_start.x(), m_start.y());
	ret = matrix1.map(ret);
	ret = matrix2.map(ret);
	return ret;
}

QRectF FractalLine::boundingRect()
{
	return polyLine().boundingRect();
}

void FractalLine::paint(QPainter &painter) const
{
	QPolygonF pLine = polyLine();
	painter.setPen(QPen(m_color, 1, Qt::SolidLine));
	painter.drawPolyline(pLine);
}

QPointF FractalLine::closestTo(QPointF testPoint)
{
	QPolygonF FractalLine = polyLine();
	qreal minDist = distance(testPoint, FractalLine[0]);
	QPointF ret = FractalLine[0];
	for(int i = 1; i < FractalLine.count(); i++){
		qreal d = distance(testPoint, FractalLine[i]);
		if(d < minDist){
			minDist = d;
			ret = FractalLine[i];
		}
		if(d == 0.0){
			break;
		}
	}
	return ret;
}
