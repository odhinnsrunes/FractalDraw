#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define VERSION "1.0"

#include <QMainWindow>
#include "line.h"
#include <QDebug>
#include <QMouseEvent>
#include "polygon.h"
#include "colorwell.h"
#include <QAction>

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
			if(polys.count())
				polys.last()->setColor(color);
			repaint();
		}
		void setFillColor(QColor color){
			if(polys.count())
				polys.last()->setFillColor(color);
			repaint();
		}

		void setBGColor(QColor color){
			m_BGColor = color;
			repaint();
		}

		void setLineColor(QColor color){
			m_lineColor = color;
			if(lines.count())
				lines.last()->setColor(color);
			repaint();
		}

		void drawPolys(bool bChecked){
			bDrawPolys = bChecked;
			if(polys.count()){
				if(!bDrawPolys && !polys.last()->complete()){
					delete polys.last();
					polys.remove(polys.count() - 1);
					repaint();
				}
			}
			aDrawLines->setChecked(!bChecked);
		}

		void drawLines(bool bChecked){
			bDrawPolys = !bChecked;
			if(polys.count()){
				if(!bDrawPolys && !polys.last()->complete()){
					delete polys.last();
					polys.remove(polys.count() - 1);
					repaint();
				}
			}
			aDrawPolys->setChecked(!bChecked);
		}

	protected:
		void changeEvent(QEvent *e);
		void paintEvent ( QPaintEvent * event );
		void mouseMoveEvent ( QMouseEvent * event );
		void mousePressEvent ( QMouseEvent * event );
		void mouseReleaseEvent ( QMouseEvent * event );
		void keyPressEvent(QKeyEvent * event);

	private slots:
		void on_actionExport_to_SVG_triggered();

	private:
		Ui::MainWindow *ui;
		//Polygon poly;
		QVector<Polygon*> polys;
		QVector<Line *> lines;

		QString path;

		ColorWell * borderWell;
		ColorWell * fillWell;
		ColorWell * backgroundWell;
		ColorWell * lineWell;
		QColor m_BGColor;
		QColor m_borderColor;
		QColor m_fillColor;
		QColor m_lineColor;

		QAction * aDrawPolys;
		QAction * aDrawLines;
		bool bDrawPolys;
};

#endif // MAINWINDOW_H
