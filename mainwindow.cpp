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

	ui->mainToolBar->setIconSize(QSize(64,64));

	QLabel *colorLabel = new QLabel(tr("Colors"));
	ui->mainToolBar->addWidget(colorLabel);

	borderWell = new ColorWell(ui->mainToolBar, tr("Border"));
	borderWell->resize(64, 64);
	defaultColor = JSONColorString(QColor(255, 240, 168));
	borderWell->setColor(JSONColorString(settings.value("borderColor", defaultColor).toString()));

	ui->mainToolBar->addWidget(borderWell);

	fillWell = new ColorWell(ui->mainToolBar, tr("Fill"));
	fillWell->resize(64, 64);
	defaultColor = JSONColorString(QColor(64, 128, 64));
	fillWell->setColor(JSONColorString(settings.value("fillColor", defaultColor).toString()));

	ui->mainToolBar->addWidget(fillWell);

	backgroundWell = new ColorWell(ui->mainToolBar, tr("Back"));
	backgroundWell->resize(64, 64);
	defaultColor = JSONColorString(QColor(0, 64, 128));
	backgroundWell->setColor(JSONColorString(settings.value("backgroundColor", defaultColor).toString()));

	ui->mainToolBar->addWidget(backgroundWell);

	lineWell = new ColorWell(ui->mainToolBar, tr("Lines"));
	lineWell->resize(64, 64);
	defaultColor = JSONColorString(QColor(0, 64, 128));
	lineWell->setColor(JSONColorString(settings.value("lineColor", defaultColor).toString()));

	ui->mainToolBar->addWidget(lineWell);

	ui->mainToolBar->addSeparator();

	QLabel *toolLabel = new QLabel(tr("Style"));
	ui->mainToolBar->addWidget(toolLabel);

	connect(borderWell, SIGNAL(colorChanged(QColor)), this, SLOT(setColor(QColor)));
	connect(fillWell, SIGNAL(colorChanged(QColor)), this, SLOT(setFillColor(QColor)));
	connect(backgroundWell, SIGNAL(colorChanged(QColor)), this, SLOT(setBGColor(QColor)));
	connect(lineWell, SIGNAL(colorChanged(QColor)), this, SLOT(setLineColor(QColor)));

	m_borderColor = borderWell->color();
	m_BGColor = backgroundWell->color();
	m_fillColor = fillWell->color();
	m_lineColor = lineWell->color();


	aDrawPolys = ui->mainToolBar->addAction(QIcon(":/images/icon.png"), tr("Draw Polys"));
	aDrawPolys->setCheckable(true);
	aDrawPolys->setChecked(true);
	connect(aDrawPolys, SIGNAL(toggled(bool)), this, SLOT(drawPolys(bool)));

	aDrawLines = ui->mainToolBar->addAction(QIcon(":/images/icon2.png"), tr("Draw Lines"));
	aDrawLines->setCheckable(true);
	aDrawLines->setChecked(false);
	connect(aDrawLines, SIGNAL(toggled(bool)), this, SLOT(drawLines(bool)));

	ui->mainToolBar->addSeparator();

	QLabel *optionLabel = new QLabel(tr("Options"));
	ui->mainToolBar->addWidget(optionLabel);

	chFillPolys = new QCheckBox(tr("Fill Polys"), ui->mainToolBar);
	chFillPolys->setChecked(settings.value("fillPolys", true).toBool());
	connect(chFillPolys, SIGNAL(clicked(bool)), this, SLOT(fillPolysChanged(bool)));
	ui->mainToolBar->addWidget(chFillPolys);

	chOutlinePolys = new QCheckBox(tr("Outline Polys"), ui->mainToolBar);
	chOutlinePolys->setChecked(settings.value("outlinePolys", true).toBool());
	connect(chOutlinePolys, SIGNAL(clicked(bool)), this, SLOT(outlinePolysChanged(bool)));
	ui->mainToolBar->addWidget(chOutlinePolys);

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
		polys[i]->paint(painter, chFillPolys->isChecked(), chOutlinePolys->isChecked());
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
	QSettings settings;
	QString lastFile = settings.value("lastsvg", "").toString();

	QString newPath = QFileDialog::getSaveFileName(this, tr("Save SVG"),
		lastFile, tr("SVG files (*.svg)"));

	if (newPath.isEmpty())
		return;

	settings.setValue("lastsvg", newPath);

	QSvgGenerator generator;
	generator.setFileName(newPath);
	//QRectF rectf = polys.last()->boundingRect();
	//QRect rect(qRound(rectf.top()), qRound(rectf.left()), qRound(rectf.width()), qRound(rectf.height()));
	QRect rect = this->rect();
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

void MainWindow::undo()
{
	if(bDrawPolys){
		polys.last()->undo();
		repaint();
	} else {
		if(lines.count()){
			delete lines.last();
			lines.remove(lines.count() - 1);
			repaint();
		}
	}
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
	switch(event->key()){
		case Qt::Key_Space:
		{
			static QTime s;
			static bool bStarted = false;
			if(!bStarted){
				s.start();
				bStarted = true;
			}
			time_t t = time(0) * 10 | s.elapsed();
			if(bDrawPolys){
				polys.last()->setLastSeed(t);
			} else {
				lines.last()->setSeed(t);
			}
			event->accept();
			repaint();
			return;
		}
		default:
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
	if(bSetTo == false && chOutlinePolys->isChecked() == false)
		chOutlinePolys->setChecked(true);
	repaint();
}

void MainWindow::outlinePolysChanged(bool bSetTo)
{
	QSettings settings;
	settings.setValue("outlinePolys", bSetTo);
	if(bSetTo == false && chFillPolys->isChecked() == false)
		chFillPolys->setChecked(true);
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

void MainWindow::save()
{
	QSettings settings;
	QString lastFile = settings.value("lastfile", "").toString();
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Drawing"), lastFile, QString("Drawings (*.fdr)"));
	if(!fileName.isEmpty()){
		QJsonObject obj;
		obj["backgroundcolor"] = JSONColor(m_BGColor);
		obj["bordercolor"] = JSONColor(m_borderColor);
		obj["fillcolor"] = JSONColor(m_fillColor);
		obj["linecolor"] = JSONColor(m_lineColor);
		QJsonArray jpolys;
		for(int i = 0; i < polys.count(); i++){
			jpolys.append(polys[i]->save());
		}
		QJsonArray jlines;
		for(int i = 0; i < lines.count(); i++){
			jlines.append(lines[i]->save());
		}
		obj["polys"] = jpolys;
		obj["lines"] = jlines;
		QJsonDocument doc = QJsonDocument(obj);
		QFile file(fileName);
		if(file.open(QFile::WriteOnly)){
			settings.setValue("lastfile", fileName);
			file.write(doc.toJson());
		}
	}
}

void MainWindow::on_actionSave_triggered()
{
	save();
}

void MainWindow::load()
{
	QSettings settings;
	QString lastFile = settings.value("lastfile", "").toString();
	QString fileName = QFileDialog::getOpenFileName(this, tr("Load Drawing"), lastFile, QString("Drawings (*.fdr)"));
	if(!fileName.isEmpty()){
		QFile file(fileName);
		if(file.open(QFile::ReadOnly)){
			settings.setValue("lastfile", fileName);
			QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
			QJsonObject obj = doc.object();
			clear();

			QJsonArray jpolys = obj["polys"].toArray();
			for(int i = 0; i < jpolys.count(); i++){
				Polygon *newPoly = new Polygon(this, jpolys[i].toObject());
				polys.append(newPoly);

			}
			QJsonArray jlines = obj["lines"].toArray();
			for(int i = 0; i < jlines.count(); i++){
				Line *newLine = new Line(this, jlines[i].toObject());
				lines.append(newLine);
			}

			backgroundWell->setColor(JSONColor(obj["backgroundcolor"].toObject()));

			borderWell->setColor(JSONColor(obj["bordercolor"].toObject()));

			fillWell->setColor(JSONColor(obj["fillcolor"].toObject()));

			lineWell->setColor(JSONColor(obj["linecolor"].toObject()));

		}
	}
	repaint();
}

void MainWindow::clear()
{
	polys.clear();
	lines.clear();
	repaint();
}

void MainWindow::on_actionNew_triggered()
{
	clear();
}

void MainWindow::on_actionOpen_triggered()
{
	load();
}

void MainWindow::on_actionUndo_triggered()
{
	undo();
}
