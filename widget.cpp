#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::Widget) {
	ui->setupUi(this);
	PlanetInit();
	planeRun->WorkInt();
	PixmapInit();
	Timer = new QTimer(this);
	connect(Timer, &QTimer::timeout, [ = ]() {
		double x, y, r, a;
		QString strShow;
		painter.begin(&pixmap);
		pen.setColor(Qt::red);
		painter.setPen(pen);
		for (int i = 0; i < 1000; i++)
			planeRun->Work(x, y);
		r = sqrt((x) * (x) + (y) * (y));
		r = qLn(r + 1) / qLn(51);
		a = qAtan2(y, x);
		x = r * qCos(a);
		y = r * qSin(a);
		px = 400 + 400 * x;
		py = 400 + 400 * y;
		strShow = QString("半径%1 角度%2° 坐标(%3,%4)").arg(r).arg(
				a / M_PI * 180).arg(
				px).arg(py);
		ui->label_2->setText(strShow);
		painter.drawPoint(px, py);
		painter.end();
		ui->label->setPixmap(pixmap);
	});
	Timer->start(1);
}

Widget::~Widget() {
	delete ui;
}

void Widget::keyPressEvent(QKeyEvent *event) {
	int key = event->key();
	if (key == Qt::Key_Space) {
		Timer->stop();
		while (Timer->isActive());
		planetFlag = planetFlag < plantMax ? planetFlag + 1 : 0;
		PlanetInit();
		PixmapInit();
		Timer->start(1);
	}
}

void Widget::PixmapInit() {
	QRect rect(0, 0, 200, 80);
	pixmap.fill(Qt::white);
	painter.begin(&pixmap);
	//设置抗锯齿，反走样。如果为false会有锯齿
	painter.setRenderHint(QPainter::Antialiasing, true);
	pen.setWidth(2);
	pen.setColor(Qt::black);
	painter.setPen(pen);
	line.setLine(400, 0, 400, 800);
	painter.drawLine(line);
	line.setLine(0, 400, 800, 400);
	painter.drawLine(line);
	painter.drawText((const QRectF)(rect), planeRun->name);
	painter.end();
	ui->label->setPixmap(pixmap);
}

void Widget::PlanetInit() {
	planeRun = planes.at(planetFlag);
	planeRun->WorkInt();
}

void Widget::paintEvent(QPaintEvent *event) {
	//  painter.begin(this);
	//  switch (runState) {
	//  case 0:
	//      //设置抗锯齿，反走样。如果为false会有锯齿
	//      painter.setRenderHint(QPainter::Antialiasing, true);
	//      pen.setWidth(2);
	//      pen.setColor(Qt::black);
	//      painter.setPen(pen);
	//      line.setLine(400, 0, 400, 800);
	//      painter.drawLine(line);
	//      line.setLine(0, 400, 800, 400);
	//      painter.drawLine(line);
	//      runState = 1;
	//      break;
	//  case 1:
	//      qDebug() << px << " " << py;
	//      pen.setWidth(2);
	//      pen.setColor(Qt::black);
	//      painter.setPen(pen);
	//      painter.drawPoint(px, py);
	//      break;
	//  default:
	//      break;
	//  }
	//  painter.end();
}

