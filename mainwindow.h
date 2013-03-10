#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "line.h"
#include <QDebug>
#include <QMouseEvent>
#include "polygon.h"

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

	protected:
		void changeEvent(QEvent *e);
		void paintEvent ( QPaintEvent * event );
		void mouseMoveEvent ( QMouseEvent * event )
		{
			if(distance(QPointF(event->pos().x(), event->pos().y()), poly.startPoint()) < 5.0){
				poly.setEndPoint(poly.startPoint());
			} else {
				poly.setEndPoint(QPointF(event->pos().x(), event->pos().y()));
			}

			this->repaint();
		}
		void mousePressEvent ( QMouseEvent * event )
		{
			if(distance(QPointF(event->pos().x(), event->pos().y()), poly.startPoint()) < 5.0){
				poly.setEndPoint(poly.startPoint());
			} else {
				poly.setEndPoint(QPointF(event->pos().x(), event->pos().y()));
			}

			this->repaint();
		}
		void mouseReleaseEvent ( QMouseEvent * event )
		{
			if(distance(QPointF(event->pos().x(), event->pos().y()), poly.startPoint()) < 5.0){
				poly.addPoint(poly.startPoint());
			} else {
				poly.addPoint(QPointF(event->pos().x(), event->pos().y()));
			}

			this->repaint();
		}

	private:
		Ui::MainWindow *ui;
		Polygon poly;
};

#endif // MAINWINDOW_H
