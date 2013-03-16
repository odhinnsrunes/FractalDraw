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
