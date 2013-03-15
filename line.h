#ifndef LINE_H
#define LINE_H

#include <QObject>
#include <QPolygon>
#include <QtCore/qmath.h>
#include <QPainter>

#define SNAP_DISTANCE 10.0

QPointF midPoint(QPointF point1, QPointF point2);
qreal distance(QPointF point1, QPointF point2);
qreal rotation(QPointF point1, QPointF point2);

class Line : public QObject
{
		Q_OBJECT
		Q_PROPERTY(unsigned int seed READ seed WRITE setSeed NOTIFY seedChanged)
		Q_PROPERTY(QPointF start READ start WRITE setStart NOTIFY startChanged)
		Q_PROPERTY(QPointF end READ end WRITE setEnd NOTIFY endChanged)
		Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

	public:
		explicit Line(QObject *parent = 0, QColor newColor = QColor(0, 0, 0), unsigned int uiSeed = time(0));
		explicit Line(const Line & oldLine);
		explicit Line(QObject *parent, QPointF newStart, QPointF newEnd, QColor newColor = QColor(0, 0, 0), unsigned int uiSeed = time(0));
		
		Line & operator=(const Line &rhs){
			if (this == &rhs)
				  return *this;
			setParent(rhs.parent());
			m_seed = rhs.seed();
			m_start = rhs.start();
			m_end = rhs.end();
			m_color = rhs.color();
			return *this;
		}

		unsigned int seed() const { return m_seed; }
		QPointF start() const { return m_start; }
		QPointF end() const { return m_end; }
		QColor color() const { return m_color; }
		QPointF closestTo(QPointF testPoint);

		bool near(QPointF testPoint, qreal threshold){
			QRectF rect = boundingRect();
			rect.setLeft(rect.left() - threshold);
			rect.setRight(rect.right() + threshold);
			rect.setTop(rect.top() - threshold);
			rect.setBottom(rect.bottom() + threshold);
			return rect.contains(testPoint);
		}

		QPolygonF polyLine() const;

		QRectF boundingRect();

		void paint(QPainter & painter) const ;

	signals:
		void seedChanged(unsigned int);
		void startChanged(QPointF);
		void endChanged(QPointF);
		void colorChanged(QColor);

	public slots:
		void setSeed(unsigned int uiNew) { m_seed = uiNew; emit seedChanged(uiNew); }
		void setStart(QPointF pNew) { m_start = pNew; emit startChanged(pNew); }
		void setEnd(QPointF pNew) { m_end = pNew; emit endChanged(pNew); }
		void setColor(QColor pNew) { m_color = pNew; emit colorChanged(pNew); }

	private:
		unsigned int m_seed;

		QPointF m_start;
		QPointF m_end;

		QColor m_color;
};

#endif // LINE_H
