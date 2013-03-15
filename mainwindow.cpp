#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "line.h"
#include <QPainter>
#include <QtSvg>
#include <QSvgGenerator>

QString JSONColor(QColor color){
	QJsonObject obj;
	obj["red"] = color.red();
	obj["green"] = color.green();
	obj["blue"] = color.blue();
	QJsonDocument doc = QJsonDocument(obj);
	return QString(doc.toJson());
}

QColor JSONColor(QString str){
	QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
	QColor ret;
	ret.setRed((int)doc.object()["red"].toDouble());
	ret.setGreen((int)doc.object()["green"].toDouble());
	ret.setBlue((int)doc.object()["blue"].toDouble());
	return ret;
}

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{

	QCoreApplication::setOrganizationName("OdhinnsRunes");
	QCoreApplication::setOrganizationDomain("odhinnsrunes.com");
	QCoreApplication::setApplicationName("Fractal Draw");
	QSettings settings;

	ui->setupUi(this);
	bDrawPolys = true;
	setMouseTracking(false);
	setAcceptDrops(false);
	setCursor(QCursor(Qt::CrossCursor));

	QString defaultColor;

	borderWell = new ColorWell(ui->mainToolBar, tr("Border"));
	borderWell->resize(64, 64);
	defaultColor = JSONColor(QColor(255, 240, 168));
	qDebug() << "border: " << defaultColor;
	borderWell->setColor(JSONColor(settings.value("borderColor", defaultColor).toString()));

	ui->mainToolBar->addWidget(borderWell);

	fillWell = new ColorWell(ui->mainToolBar, tr("Fill"));
	fillWell->resize(64, 64);
	defaultColor = JSONColor(QColor(64, 128, 64));
	fillWell->setColor(JSONColor(settings.value("fillColor", defaultColor).toString()));

	ui->mainToolBar->addWidget(fillWell);

	backgroundWell = new ColorWell(ui->mainToolBar, tr("Back"));
	backgroundWell->resize(64, 64);
	defaultColor = JSONColor(QColor(0, 64, 128));
	backgroundWell->setColor(JSONColor(settings.value("backgroundColor", defaultColor).toString()));

	ui->mainToolBar->addWidget(backgroundWell);

	lineWell = new ColorWell(ui->mainToolBar, tr("Lines"));
	lineWell->resize(64, 64);
	defaultColor = JSONColor(QColor(0, 64, 128));
	lineWell->setColor(JSONColor(settings.value("lineColor", defaultColor).toString()));

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

	chFillPolys = new QCheckBox(tr("Fill Polys"), ui->mainToolBar);
	chFillPolys->setChecked(settings.value("fillPolys", true).toBool());
	connect(chFillPolys, SIGNAL(clicked(bool)), this, SLOT(fillPolysChanged(bool)));
	ui->mainToolBar->addWidget(chFillPolys);

	chShowBackground = new QCheckBox(tr("Show Background"), ui->mainToolBar);
	chShowBackground->setChecked(settings.value("showBackground", true).toBool());
	connect(chShowBackground, SIGNAL(clicked(bool)), this, SLOT(showBackgroundChanged(bool)));
	ui->mainToolBar->addWidget(chShowBackground);

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
	if(chShowBackground->isChecked())
		painter.fillRect(this->rect(), m_BGColor);
	for(int i = 0; i < polys.count(); i++){
		polys[i]->paint(painter, chFillPolys->isChecked());
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
	if(!chShowBackground->isChecked())
		painter.fillRect(this->rect(), Qt::white);
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

void MainWindow::mouseReleaseEvent ( QMouseEvent * event )
{
	QPointF mousePoint = QPointF(event->pos().x(), event->pos().y());
	if(bDrawPolys){
		if(distance(mousePoint, polys.last()->startPoint()) < SNAP_DISTANCE){
			polys.last()->addPoint(polys.last()->startPoint());
		} else {
			polys.last()->addPoint(mousePoint);
		}
	} else {
		if(lines.count()){
			bool bDidIt = false;
			for(int i = 0; i < polys.count(); i++){
//						if(polys[i]->near(mousePoint, SNAP_DISTANCE)){
					QPointF p = polys[i]->closestTo(mousePoint);
					if(distance(p, mousePoint) < SNAP_DISTANCE){
						lines.last()->setEnd(p);
						bDidIt = true;
						break;
					}
//						}
			}
			for(int i = 0; i < lines.count() - 1; i++){
				if(lines[i]->near(mousePoint, SNAP_DISTANCE)){
					QPointF p = lines[i]->closestTo(mousePoint);
					if(distance(p, mousePoint) < SNAP_DISTANCE){
						lines.last()->setEnd(p);
						bDidIt = true;
						break;
					}
				}
			}
			if(!bDidIt)
				lines.last()->setEnd(mousePoint);
		}
	}
	this->repaint();
}

void MainWindow::mouseMoveEvent ( QMouseEvent * event )
{
	QPointF mousePoint = QPointF(event->pos().x(), event->pos().y());
	if(bDrawPolys){
		if(distance(mousePoint, polys.last()->startPoint()) < SNAP_DISTANCE){
			polys.last()->setEndPoint(polys.last()->startPoint());
		} else {
			polys.last()->setEndPoint(mousePoint);
		}
	} else {
		if(lines.count()){
			bool bDidIt = false;
			for(int i = 0; i < polys.count(); i++){
//						if(polys[i]->near(mousePoint, SNAP_DISTANCE)){
					QPointF p = polys[i]->closestTo(mousePoint);
					if(distance(p, mousePoint) < SNAP_DISTANCE){
						lines.last()->setEnd(p);
						bDidIt = true;
						break;
					}
//						}
			}
			for(int i = 0; i < lines.count() - 1; i++){
				if(lines[i]->near(mousePoint, SNAP_DISTANCE)){
					QPointF p = lines[i]->closestTo(mousePoint);
					if(distance(p, mousePoint) < SNAP_DISTANCE){
						lines.last()->setEnd(p);
						bDidIt = true;
						break;
					}
				}
			}
			if(!bDidIt)
				lines.last()->setEnd(mousePoint);
		}
	}
	this->repaint();
}

void MainWindow::mousePressEvent ( QMouseEvent * event )
{
	QPointF mousePoint = QPointF(event->pos().x(), event->pos().y());
	if(bDrawPolys){
		if(polys.count() == 0){
			Polygon *newPoly = new Polygon(this, m_borderColor, m_fillColor);
			polys.append(newPoly);
			polys.last()->addPoint(mousePoint);
		} else {
			if(polys.last()->complete()){
				Polygon *newPoly = new Polygon(this, m_borderColor, m_fillColor);
				polys.append(newPoly);
				polys.last()->addPoint(mousePoint);
			}
		}
		if(distance(mousePoint, polys.last()->startPoint()) < SNAP_DISTANCE){
			polys.last()->setEndPoint(polys.last()->startPoint());
		} else {
			polys.last()->setEndPoint(mousePoint);
		}
	} else {
		bool bDidIt = false;
		for(int i = 0; i < polys.count(); i++){
			//if(polys[i]->near(mousePoint, SNAP_DISTANCE)){
				QPointF p = polys[i]->closestTo(mousePoint);
				if(distance(p, mousePoint) < SNAP_DISTANCE){
					Line * newLine = new Line(this, p, p, m_lineColor);
					lines.append(newLine);
					bDidIt = true;
					break;
				}
//			}
		}
		for(int i = 0; i < lines.count() - 1; i++){
			if(lines[i]->near(mousePoint, SNAP_DISTANCE)){
				QPointF p = lines[i]->closestTo(mousePoint);
				if(distance(p, mousePoint) < SNAP_DISTANCE){
					Line * newLine = new Line(this, p, p, m_lineColor);
					lines.append(newLine);
					bDidIt = true;
					break;
				}
			}
		}
		if(!bDidIt){
			Line * newLine = new Line(this, mousePoint, mousePoint, m_lineColor);
			lines.append(newLine);
		}
	}
	this->repaint();
}


void MainWindow::fillPolysChanged(bool bSetTo)
{
	QSettings settings;
	settings.setValue("fillPolys", bSetTo);
	repaint();
}

void MainWindow::showBackgroundChanged(bool bSetTo)
{
	QSettings settings;
	settings.setValue("showBackground", bSetTo);
	repaint();
}

void MainWindow::on_actionDefault_Colors_triggered()
{
	setColor(QColor(255, 240, 168));
	borderWell->setColor(QColor(255, 240, 168));
	setFillColor(QColor(64, 128, 64));
	fillWell->setColor(QColor(64, 128, 64));
	setBGColor(QColor(0, 64, 128));
	backgroundWell->setColor(QColor(0, 64, 128));
	setLineColor(QColor(0, 64, 128));
	lineWell->setColor(QColor(0, 64, 128));
}
