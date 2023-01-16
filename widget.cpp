#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::Widget) {
	ui->setupUi(this);
	plane1 = new PlanetInf(QString("test"), 0.5, 0, 0, 1.63);
	plane1->WorkInt();
	pixmap.fill(Qt::white);
	ui->label->setPixmap(pixmap);
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
	painter.end();
	ui->label->setPixmap(pixmap);
	Timer = new QTimer(this);
	connect(Timer, &QTimer::timeout, [ = ]() {
		double x, y;
		painter.begin(&pixmap);
		pen.setColor(Qt::red);
		painter.setPen(pen);
		plane1->Work(x, y);
		px = 400 + 200 * x;
		py = 400 + 200 * y;
		qDebug() << px << " " << py;
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
	painter.begin(&pixmap);
	if (key == Qt::Key_Space) {
		pixmap.fill(Qt::white);
		pen.setWidth(2);
		pen.setColor(Qt::black);
		painter.setPen(pen);
		line.setLine(400, 0, 400, 800);
		painter.drawLine(line);
		line.setLine(0, 400, 800, 400);
		painter.drawLine(line);
	}
	painter.end();
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

