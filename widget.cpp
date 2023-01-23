#include "widget.h"
#include "ui_widget.h"

#ifndef TARGET_ARCH_ARM
	#define USER_TD (0.001)
#else
	#define USER_TD (0.04)
#endif

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
		/* 将顶层清空 */
		pixmapTop.fill(colorTop);
		painterTop.begin(&pixmapTop);
		pen.setColor(Qt::red);
		painter.setPen(pen);
		for (int i = 0; i <= plantMax; i++) {
			/* 在不损失仿真精度的前提下一定程度地提高仿真速度 */
#ifndef TARGET_ARCH_ARM
			for (int k = 0; k < 1000; k++)
#else
			for (int k = 0; k < 100; k++)
#endif
				if (planetFlag <= plantMax)
					planeRun->Work(x, y);
				else
					/* 使用USER_TD确保所有行星仿真步长一致 */
					planes.at(i)->Work(x, y, USER_TD);
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
			if (planetFlag <= plantMax) {
				/* 显示当前仿真点的数据 */
				strShow1 = QString("距离%1").arg(r0);
				strShow2 = QString("角度%2°").arg(
						a / M_PI * 180);
				strShow3 = QString("坐标(%3,%4)").arg(px - 250).arg(250 - py);
				ui->label_2->setText(strShow1);
				ui->label_5->setText(strShow2);
				ui->label_6->setText(strShow3);
			}
			else {
				ui->label_2->setText(QString("在全部仿真模式下，"));
				ui->label_5->setText(
					QString("仿真按照真实时间比例"));
				ui->label_6->setText(QString("水金地火木土天海冥"));
			}
			/* 绘制仿真结果 */
#ifndef TARGET_ARCH_ARM /* 选择绘制连线还是点,arm平台由于仿真精度低所以绘制连线 */
			painter.drawPoint(px, py);
#else
			if (pxOlds[i] == 0) {
				pxOlds[i] = px;
				pyOlds[i] = py;
			}
			painter.drawLine(px, py, pxOlds[i], pyOlds[i]);
			pxOlds[i] = px;
			pyOlds[i] = py;
#endif
			/* 开始绘制顶层 */
			penTop.setColor(Qt::blue);
			penTop.setWidth(1);
			painterTop.setPen(penTop);
			painterTop.setBrush(Qt::blue);
			painterTop.drawEllipse(px - 5, py - 5, 10, 10);
			if (planetFlag <= plantMax)
				break;
		}
		painterTop.end();
		painter.end();
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
		planetFlag = planetFlag < plantMax + 1 ? planetFlag + 1 : 0;
		PlanetInit();
		PixmapInit();
		Timer->start(1);
	}
}

/* 初始化坐标系 */
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
	if (planetFlag <= plantMax)
		painter.drawText((const QRectF)(rect), planeRun->name);
	else
		painter.drawText((const QRectF)(rect), QString("全部"));
	pen.setColor(Qt::yellow);
	painter.setPen(pen);
	painter.setBrush(Qt::yellow);
	painter.drawEllipse(250 - 10, 250 - 10, 20, 20);
	painter.end();
	ui->label->setPixmap(pixmap);
	ui->label_4->setPixmap(pixmapTop);
}

/* 初始化行星计算资源 */
void Widget::PlanetInit() {
	if (planetFlag <= plantMax) {
		planeRun = planes.at(planetFlag);
		planeRun->WorkInt();
		pxOlds[0] = 0;
		pyOlds[0] = 0;
	}
	else {
		for (int i = 0; i <= plantMax; i++) {
			/* 使用USER_TD确保所有行星仿真步长一致 */
			planes.at(i)->WorkInt(USER_TD);
			pxOlds[i] = 0;
			pyOlds[i] = 0;
		}
	}
}

void Widget::on_pushButton_clicked() {
	/* 切换行星 */
	Timer->stop();
	while (Timer->isActive());
	planetFlag = (planetFlag < plantMax + 1) ? planetFlag + 1 : 0;
	PlanetInit();
	PixmapInit();
	Timer->start(1);
}

