#ifndef LINE_H
#define LINE_H

#include <QObject>
#include <QPolygon>
#include <QtCore/qmath.h>
#include <QPainter>

class Line : public QObject
{
		Q_OBJECT
		Q_PROPERTY(unsigned int iterations READ iterations WRITE setIterations NOTIFY iterationsChanged)
		Q_PROPERTY(unsigned int seed READ seed WRITE setSeed NOTIFY seedChanged)
		Q_PROPERTY(QPointF start READ start WRITE setStart NOTIFY startChanged)
		Q_PROPERTY(QPointF end READ end WRITE setEnd NOTIFY endChanged)
		Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

	public:
		explicit Line(QObject *parent = 0, QColor newColor = QColor(0, 0, 0), unsigned int uiSeed = time(0));
		explicit Line(QObject *parent, Line & oldLine);
		explicit Line(QObject *parent, QPointF newStart, QPointF newEnd, QColor newColor = QColor(0, 0, 0), unsigned int uiSeed = time(0));
		
		unsigned int pointCount(){
			return 1 + (qPow(2, m_iterations));
		}

		unsigned int iterations() { return m_iterations; }
		unsigned int seed() { return m_seed; }
		QPointF start() { return m_start; }
		QPointF end() { return m_end; }
		QColor color() { return m_color; }

		QPolygonF polyLine();

		void paint(QPainter & painter);

	signals:
		void iterationsChanged();
		void seedChanged();
		void startChanged();
		void endChanged();
		void colorChanged();

	public slots:
		void setIterations(unsigned int uiNew) { m_iterations = uiNew; emit iterationsChanged(); }
		void setSeed(unsigned int uiNew) { m_seed = uiNew; emit seedChanged(); }
		void setStart(QPointF pNew) { m_start = pNew; emit startChanged(); }
		void setEnd(QPointF pNew) { m_end = pNew; emit endChanged(); }
		void setColor(QColor pNew) { m_color = pNew; emit colorChanged(); }

	private:
		unsigned int m_iterations;
		unsigned int m_seed;

		QPointF m_start;
		QPointF m_end;

		QColor m_color;
};

#endif // LINE_H
