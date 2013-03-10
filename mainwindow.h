#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "line.h"
#include <QDebug>
#include <QMouseEvent>
#include "polygon.h"
#include "colorwell.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
		Q_OBJECT
		
	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();
		void paint(QPainter &painter);

	public slots:
		void saveSvg();
		void setColor(QColor color){
			polys.last()->setColor(color);
			repaint();
		}
		void setFillColor(QColor color){
			polys.last()->setFillColor(color);
			repaint();
		}

		void setBGColor(QColor color){
			m_BGColor = color;
			repaint();
		}

	protected:
		void changeEvent(QEvent *e);
		void paintEvent ( QPaintEvent * event );
		void mouseMoveEvent ( QMouseEvent * event )
		{
			if(distance(QPointF(event->pos().x(), event->pos().y()), polys.last()->startPoint()) < 5.0){
				polys.last()->setEndPoint(polys.last()->startPoint());
			} else {
				polys.last()->setEndPoint(QPointF(event->pos().x(), event->pos().y()));
			}

			this->repaint();
		}
		void mousePressEvent ( QMouseEvent * event );

		void mouseReleaseEvent ( QMouseEvent * event )
		{
			if(distance(QPointF(event->pos().x(), event->pos().y()), polys.last()->startPoint()) < 5.0){
				polys.last()->addPoint(polys.last()->startPoint());
			} else {
				polys.last()->addPoint(QPointF(event->pos().x(), event->pos().y()));
			}

			this->repaint();
		}

		void keyPressEvent(QKeyEvent * event);

	private slots:
		void on_actionExport_to_SVG_triggered();

	private:
		Ui::MainWindow *ui;
		//Polygon poly;
		QVector<Polygon*> polys;
		QString path;

		ColorWell * borderWell;
		ColorWell * fillWell;
		ColorWell * backgroundWell;
		QColor m_BGColor;
		QColor m_borderColor;
		QColor m_fillColor;
		QColor m_lineColor;
};

#endif // MAINWINDOW_H
