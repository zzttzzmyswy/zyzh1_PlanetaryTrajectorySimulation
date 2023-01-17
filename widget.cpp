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
    /*使用lambda表达式简化函数调用*/
	connect(Timer, &QTimer::timeout, [ = ]() {
		double x, y, r, r0, a;
        QString strShow1, strShow2, strShow3;
		painter.begin(&pixmap);
		pen.setColor(Qt::red);
		painter.setPen(pen);
		/* 在不损失仿真精度的前提下一定程度地提高仿真速度 */
#ifndef TARGET_ARCH_ARM
        for (int i = 0; i < 1000; i++)
#else
        for (int i = 0; i < 100; i++)
#endif
			planeRun->Work(x, y);
		/* 对输出的坐标点极坐标化，
		 * 并且将与太阳的距离对数化，
		 * 以确保水星的轨迹容易辨识*/
		r0 = sqrt((x) * (x) + (y) * (y));
		r = qLn(r0 + 1) / qLn(51);
		a = qAtan2(y, x);
		/* 将极坐标转为直角坐标系坐标 */
		x = r * qCos(a);
		y = r * qSin(a);
        px = 250 + 240 * x;
        py = 250 + 240 * y;
		/* 显示当前仿真点的数据 */
        strShow1 = QString("距离%1").arg(r0);
        strShow2 = QString("角度%2°").arg(
                a / M_PI * 180);
        strShow3 = QString("坐标(%3,%4)").arg(px - 250).arg(250 - py);
        ui->label_2->setText(strShow1);
        ui->label_5->setText(strShow2);
        ui->label_6->setText(strShow3);
        /* qDebug() << strShow1 << strShow2 << strShow3; */
        /* 绘制仿真结果 */
#ifndef TARGET_ARCH_ARM /* 选择绘制连线还是点,arm平台由于仿真精度低所以绘制连线 */
		painter.drawPoint(px, py);
#else
        if (pxOld == 0) {
            pxOld = px;
            pyOld = py;
        }
        painter.drawLine(px, py, pxOld, pyOld);
        pxOld = px;
        pyOld = py;
#endif
		painter.end();
		/* 开始绘制顶层 */
		pixmapTop.fill(colorTop);
		painterTop.begin(&pixmapTop);
		penTop.setColor(Qt::blue);
		penTop.setWidth(1);
		painterTop.setPen(penTop);
		painterTop.setBrush(Qt::blue);
		painterTop.drawEllipse(px - 5, py - 5, 10, 10);
		painterTop.end();
		ui->label->setPixmap(pixmap);
		ui->label_4->setPixmap(pixmapTop);
	});
	Timer->start(1);
}

Widget::~Widget() {
	delete ui;
}

void Widget::keyPressEvent(QKeyEvent *event) {
	int key = event->key();
	/* 当空格按下时切换行星 */
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
	colorTop.setAlpha(0);
	pixmap.fill(Qt::white);
	pixmapTop.fill(colorTop);
	painter.begin(&pixmap);
	//设置抗锯齿，反走样。如果为false会有锯齿
	painter.setRenderHint(QPainter::Antialiasing, true);
	pen.setWidth(2);
	pen.setColor(Qt::black);
	painter.setPen(pen);
    line.setLine(250, 0, 250, 500);
	painter.drawLine(line);
    line.setLine(0, 250, 500, 250);
	painter.drawLine(line);
	painter.drawText((const QRectF)(rect), planeRun->name);
	pen.setColor(Qt::yellow);
	painter.setPen(pen);
	painter.setBrush(Qt::yellow);
    painter.drawEllipse(250 - 10, 250 - 10, 20, 20);
	painter.end();
	ui->label->setPixmap(pixmap);
	ui->label_4->setPixmap(pixmapTop);
    pxOld = 0;
    pyOld = 0;
}

void Widget::PlanetInit() {
	planeRun = planes.at(planetFlag);
	planeRun->WorkInt();
}


void Widget::on_pushButton_clicked() {
    /* 切换行星 */
    Timer->stop();
    while (Timer->isActive());
    planetFlag = planetFlag < plantMax ? planetFlag + 1 : 0;
    PlanetInit();
    PixmapInit();
    Timer->start(1);
}

