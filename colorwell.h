#ifndef COLORWELL_H
#define COLORWELL_H

#include <QObject>
#include <QFrame>
#include <QPainter>
#include <QColorDialog>
#include <QMouseEvent>

class ColorWell : public QFrame
{
		Q_OBJECT
		Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
	public:
		explicit ColorWell(QWidget *parent = 0);

		virtual QSize	sizeHint () const {
			return QSize(64, 64);
		}
		
		QColor color() {
			return m_color;
		}

		void paintEvent(QPaintEvent * ){
			QPainter painter(this);
			painter.fillRect(this->rect(), m_color);
		}

	signals:
		void colorChanged(QColor);

	public slots:
		void setColor(QColor newColor){
			m_color = newColor;
			emit colorChanged(newColor);
		}

	protected:
		void mousePressEvent( QMouseEvent * event ){
			event->accept();
		}

		void mouseMoveEvent( QMouseEvent * event ){
			event->accept();
		}

		void mouseReleaseEvent ( QMouseEvent * event ){
			QColor newColor = QColorDialog::getColor (m_color, this);
			if(newColor.isValid()){
				setColor(newColor);
				repaint();
			}
			event->accept();
		}

	private:
		QColor m_color;
};

#endif // COLORWELL_H
