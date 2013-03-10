#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "line.h"
#include <QDebug>
#include <QMouseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
		Q_OBJECT
		
	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();
		
	protected:
		void changeEvent(QEvent *e);
		void paintEvent ( QPaintEvent * event );
		void mouseMoveEvent ( QMouseEvent * event )
		{
			line->setEnd(QPointF(event->pos().x(), event->pos().y()));
			this->repaint();
		}
		void mousePressEvent ( QMouseEvent * event )
		{
			line->setStart(QPointF(event->pos().x(), event->pos().y()));
			this->repaint();
		}

	private:
		Ui::MainWindow *ui;
		Line *line;
};

#endif // MAINWINDOW_H
