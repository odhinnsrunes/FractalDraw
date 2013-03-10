#ifndef LINE_H
#define LINE_H

#include <QObject>
#include <QPolygon>
#include <QtCore/qmath.h>

class Line : public QObject
{
		Q_OBJECT
		Q_PROPERTY(unsigned int iterations READ iterations WRITE setIterations NOTIFY iterationsChanged)
		Q_PROPERTY(unsigned int seed READ seed WRITE setSeed NOTIFY seedChanged)
		Q_PROPERTY(QPointF start READ start WRITE setStart NOTIFY startChanged)
		Q_PROPERTY(QPointF end READ end WRITE setEnd NOTIFY endChanged)

	public:
		explicit Line(QObject *parent = 0);
		
		unsigned int pointCount(){
			return 1 + (qPow(2, m_iterations));
		}

		unsigned int iterations() { return m_iterations; }
		unsigned int seed() { return m_seed; }
		QPointF start() { return m_start; }
		QPointF end() { return m_end; }

		QPolygonF polyLine();

	signals:
		void iterationsChanged();
		void seedChanged();
		void startChanged();
		void endChanged();

	public slots:
		void setIterations(unsigned int uiNew) { m_iterations = uiNew; emit iterationsChanged(); }
		void setSeed(unsigned int uiNew) { m_seed = uiNew; emit seedChanged(); }
		void setStart(QPointF pNew) { m_start = pNew; emit startChanged(); }
		void setEnd(QPointF pNew) { m_end = pNew; emit endChanged(); }

	private:
		unsigned int m_iterations;
		unsigned int m_seed;

		QPointF m_start;
		QPointF m_end;
};

#endif // LINE_H
