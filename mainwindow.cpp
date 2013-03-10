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
	setCursor(QCursor(Qt::CrossCursor));
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

void MainWindow::paint(QPainter &painter)
{
	static bool bDrawing = false;
	if(bDrawing)
		return;
	bDrawing = true;

	line->paint(painter);

	bDrawing = false;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	paint(painter);
}
