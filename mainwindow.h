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
		void outlinePolysChanged(bool bSetTo);

		void on_actionDefault_Colors_triggered();

		void on_actionSave_triggered();

		void on_actionNew_triggered();

		void on_actionOpen_triggered();

		void on_actionUndo_triggered();

	private:
		Ui::MainWindow *ui;
		//FractalPolygon poly;
		QVector<FractalPolygon*> polys;
		QVector<FractalLine *> lines;

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
		QCheckBox * chOutlinePolys;

		bool bDrawPolys;
};

#endif // MAINWINDOW_H
