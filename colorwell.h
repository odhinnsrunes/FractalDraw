#ifndef COLORWELL_H
#define COLORWELL_H

#include <QObject>
#include <QFrame>
#include <QPainter>
#include <QColorDialog>
#include <QMouseEvent>
#include <QDebug>
class ColorWell : public QFrame
{
		Q_OBJECT
		Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
		Q_PROPERTY(QString caption READ caption WRITE setCaption NOTIFY captionChanged)
	public:
		explicit ColorWell(QWidget *parent = 0, QString newCaption = QString());

		virtual QSize	sizeHint () const {
			return QSize(64, 64);
		}
		
		QColor color() {
			return m_color;
		}

		QString caption() {
			return m_caption;
		}

		void paintEvent(QPaintEvent * ){
			QPainter painter(this);
			painter.fillRect(this->rect(), m_color);
			painter.setFont(QFont("Helvetica", 12));
			if(m_color.lightness() > 196)
				painter.setPen(QPen(Qt::black, 1));
			else
				painter.setPen(QPen(Qt::white, 1));
			painter.drawText(rect(), Qt::AlignCenter | Qt::AlignVCenter, m_caption);
		}

	signals:
		void colorChanged(QColor);
		void captionChanged(QString);

	public slots:
		void setColor(QColor newColor){
			m_color = newColor;
			qDebug() << newColor;
			emit colorChanged(newColor);
		}

		void setCaption(QString newCaption){
			m_caption = newCaption;
			emit captionChanged(newCaption);
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
		QString m_caption;
};

#endif // COLORWELL_H
