#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define VERSION "0.9.1"

#include <QMainWindow>
#include "line.h"
#include <QDebug>
#include <QMouseEvent>
#include "polygon.h"
#include "colorwell.h"
#include <QAction>
#include <QCheckBox>
#include <QSettings>

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
			QSettings settings;
			settings.setValue("borderColor", JSONColorString(color));
			if(polys.count())
				polys.last()->setColor(color);
			repaint();
		}
		void setFillColor(QColor color){
			QSettings settings;
			settings.setValue("fillColor", JSONColorString(color));
			if(polys.count())
				polys.last()->setFillColor(color);
			repaint();
		}

		void setBGColor(QColor color){
			QSettings settings;
			settings.setValue("backgroundColor", JSONColorString(color));
			m_BGColor = color;
			repaint();
		}

		void setLineColor(QColor color){
			QSettings settings;
			settings.setValue("lineColor", JSONColorString(color));
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

		void save();
		void load();
		void clear();
		void undo();

	protected:
		void changeEvent(QEvent *e);
		void paintEvent ( QPaintEvent * event );
		void mouseMoveEvent ( QMouseEvent * event );
		void mousePressEvent ( QMouseEvent * event );
		void mouseReleaseEvent ( QMouseEvent * event );
		void keyPressEvent(QKeyEvent * event);

	private slots:
		void on_actionExport_to_SVG_triggered();
		void fillPolysChanged(bool bSetTo);
		void showBackgroundChanged(bool bSetTo);

		void on_actionDefault_Colors_triggered();

		void on_actionSave_triggered();

		void on_actionNew_triggered();

		void on_actionOpen_triggered();

		void on_actionUndo_triggered();

	private:
		Ui::MainWindow *ui;
		//Polygon poly;
		QVector<Polygon*> polys;
		QVector<Line *> lines;

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

		QCheckBox * chShowBackground;
		QCheckBox * chFillPolys;

		bool bDrawPolys;
};

#endif // MAINWINDOW_H
