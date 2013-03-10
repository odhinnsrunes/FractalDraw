#ifndef POLYGON_H
#define POLYGON_H

#include <QObject>
#include "line.h"

class Polygon : public QObject
{
		Q_OBJECT
	public:
		explicit Polygon(QObject *parent = 0);
		
	signals:
		
	public slots:

	private:
		QVector<Line> lines;
		
};

#endif // POLYGON_H
