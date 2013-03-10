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
	poly.setColor(QColor(255, 0, 255));
	setMouseTracking(false);
	setAcceptDrops(false);
	setCursor(QCursor(Qt::CrossCursor));

	borderWell = new ColorWell(ui->mainToolBar);
	borderWell->resize(64, 64);
	borderWell->setColor(QColor(32, 64, 32));

	ui->mainToolBar->addWidget(borderWell);

	fillWell = new ColorWell(ui->mainToolBar);
	fillWell->resize(64, 64);
	fillWell->setColor(QColor(64, 128, 64));

	ui->mainToolBar->addWidget(fillWell);

	backgroundWell = new ColorWell(ui->mainToolBar);
	backgroundWell->resize(64, 64);
	backgroundWell->setColor(QColor(0, 64, 128));

	ui->mainToolBar->addWidget(backgroundWell);

	poly.setColor(borderWell->color());
	poly.setFillColor(fillWell->color());
	m_BGColor = backgroundWell->color();

	connect(borderWell, SIGNAL(colorChanged(QColor)), this, SLOT(setColor(QColor)));
	connect(fillWell, SIGNAL(colorChanged(QColor)), this, SLOT(setFillColor(QColor)));
	connect(backgroundWell, SIGNAL(colorChanged(QColor)), this, SLOT(setBGColor(QColor)));
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
	painter.fillRect(this->rect(), m_BGColor);
	poly.paint(painter);
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
	QRectF rectf = poly.boundingRect();
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
				poly.undo();
				repaint();
				event->accept();
				qDebug() << "Undo";
				return;
			}
			break;
	}
	QWidget::keyPressEvent(event);
}
