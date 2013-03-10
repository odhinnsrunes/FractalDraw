#ifndef POLYGON_H
#define POLYGON_H

#include <QObject>
#include "line.h"

class Polygon : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

	public:
		explicit Polygon(QObject *parent = 0);
		void addPoint(QPointF newPoint);
		void setEndPoint(QPointF newPoint);
		void undo(){
			if(lines.count()){
				lines.remove(lines.count() - 1);
			}
		}

		QRectF boundingRect();

		void paint(QPainter & painter);
		QColor color() const { return m_color; }

		bool complete(){
			if(lines.count()){
				if(startPoint() == lines.last().end()){
					return true;
				}
			}
			return false;
		}

		QPointF startPoint(){
			if(lines.count()){
				return lines.at(0).start();
			}
			return QPointF(0.0, 0.0);
		}

	signals:
		void colorChanged();

	public slots:
		void setColor(QColor color);

	private:
		QVector<Line> lines;

		QColor m_color;
		
};

#endif // POLYGON_H