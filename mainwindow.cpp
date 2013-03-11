#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "line.h"
#include <QPainter>
#include <QtSvg>
#include <QSvgGenerator>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	bDrawPolys = true;
	setMouseTracking(false);
	setAcceptDrops(false);
	setCursor(QCursor(Qt::CrossCursor));

	borderWell = new ColorWell(ui->mainToolBar, tr("Border"));
	borderWell->resize(64, 64);
	borderWell->setColor(QColor(255, 240, 168));

	ui->mainToolBar->addWidget(borderWell);

	fillWell = new ColorWell(ui->mainToolBar, tr("Fill"));
	fillWell->resize(64, 64);
	fillWell->setColor(QColor(64, 128, 64));

	ui->mainToolBar->addWidget(fillWell);

	backgroundWell = new ColorWell(ui->mainToolBar, tr("Back"));
	backgroundWell->resize(64, 64);
	backgroundWell->setColor(QColor(0, 64, 128));

	ui->mainToolBar->addWidget(backgroundWell);

	lineWell = new ColorWell(ui->mainToolBar, tr("Lines"));
	lineWell->resize(64, 64);
	lineWell->setColor(QColor(0, 64, 128));

	ui->mainToolBar->addWidget(lineWell);

	connect(borderWell, SIGNAL(colorChanged(QColor)), this, SLOT(setColor(QColor)));
	connect(fillWell, SIGNAL(colorChanged(QColor)), this, SLOT(setFillColor(QColor)));
	connect(backgroundWell, SIGNAL(colorChanged(QColor)), this, SLOT(setBGColor(QColor)));
	connect(lineWell, SIGNAL(colorChanged(QColor)), this, SLOT(setLineColor(QColor)));

	m_borderColor = borderWell->color();
	m_BGColor = backgroundWell->color();
	m_fillColor = fillWell->color();
	m_lineColor = lineWell->color();

	aDrawPolys = ui->mainToolBar->addAction(tr("Draw Polys"));
	aDrawPolys->setCheckable(true);
	aDrawPolys->setChecked(true);
	connect(aDrawPolys, SIGNAL(toggled(bool)), this, SLOT(drawPolys(bool)));

	aDrawLines = ui->mainToolBar->addAction(tr("Draw Lines"));
	aDrawLines->setCheckable(true);
	aDrawLines->setChecked(false);
	connect(aDrawLines, SIGNAL(toggled(bool)), this, SLOT(drawLines(bool)));


	this->setWindowTitle(QString("FractalDraw v%1").arg(VERSION));

	showMaximized();
}

MainWindow::~MainWindow()
{
	delete ui;
	int c = polys.count();
	for(int i = 0; i < c; i++){
		if(polys[i]){
			delete polys[i];
		}
	}
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
	painter.fillRect(this->rect(), m_BGColor);
	for(int i = 0; i < polys.count(); i++){
		polys[i]->paint(painter);
	}
	for(int i = 0; i < lines.count(); i++){
		lines[i]->paint(painter);
	}
	bDrawing = false;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	paint(painter);
	painter.end();
}

void MainWindow::saveSvg()
{
	QString newPath = QFileDialog::getSaveFileName(this, tr("Save SVG"),
		path, tr("SVG files (*.svg)"));

	if (newPath.isEmpty())
		return;

	path = newPath;

	QSvgGenerator generator;
	generator.setFileName(path);
	QRectF rectf = polys.last()->boundingRect();
	QRect rect(qRound(rectf.top()), qRound(rectf.left()), qRound(rectf.width()), qRound(rectf.height()));
	generator.setSize(QSize(rect.width(), rect.height()));
	generator.setViewBox(rect);
	generator.setTitle(tr("SVG Generator Example Drawing"));
	generator.setDescription(tr("An SVG drawing created by the SVG Generator "
								"Example provided with Qt."));
	QPainter painter;
	painter.begin(&generator);
	paint(painter);
	painter.end();
}

void MainWindow::on_actionExport_to_SVG_triggered()
{
	saveSvg();
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
	switch(event->key()){
		case Qt::Key_Z:
			if((event->modifiers() & Qt::ControlModifier)){
				if(bDrawPolys){
					polys.last()->undo();
					repaint();
					event->accept();
					return;
				} else {
					if(lines.count()){
						delete lines.last();
						lines.remove(lines.count() - 1);
						repaint();
						event->accept();
					}
				}
			}
			break;
	}
	QWidget::keyPressEvent(event);
}

void MainWindow::mousePressEvent ( QMouseEvent * event )
{
	if(bDrawPolys){
		if(polys.count() == 0){
			Polygon *newPoly = new Polygon(this, m_borderColor, m_fillColor);
			polys.append(newPoly);
			polys.last()->addPoint(QPointF(event->pos().x(), event->pos().y()));
		} else {
			if(polys.last()->complete()){
				Polygon *newPoly = new Polygon(this, m_borderColor, m_fillColor);
				polys.append(newPoly);
				polys.last()->addPoint(QPointF(event->pos().x(), event->pos().y()));
			}
		}
		if(distance(QPointF(event->pos().x(), event->pos().y()), polys.last()->startPoint()) < 5.0){
			polys.last()->setEndPoint(polys.last()->startPoint());
		} else {
			polys.last()->setEndPoint(QPointF(event->pos().x(), event->pos().y()));
		}
	} else {
		Line * newLine = new Line(this, QPointF(event->pos().x(), event->pos().y()), QPointF(event->pos().x(), event->pos().y()), m_lineColor);
		lines.append(newLine);
	}
	this->repaint();
}
