#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "line.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	line = new Line;
	line->setIterations(8);
	line->setStart(QPoint(20, 20));
	line->setEnd(QPoint(100, 100));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
	QMainWindow::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	static bool bDrawing = false;
	if(bDrawing)
		return;
	bDrawing = true;
	QPainter painter(this);
	QMatrix matrix;
//	matrix.scale(5, 5);
	matrix.translate(10, 10);
	QPolygonF pLine = line->polyLine();
	painter.setMatrix(matrix);
	painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
	painter.drawPolyline(pLine);
	bDrawing = false;
}
