#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
/* 加入画笔支持 */
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QRgb>
#include <QLine>
#include <QLineF>
#include <QImage>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QString>
#include <QTimer>
#include <QDebug>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

#define MY_DT (0.001) /* 时间步长 */
#define MY_SUN_M (1) /* 太阳质量 */
#define MY_G (1) /* 万有引力常熟 */

class PlanetInf {
  public:
	PlanetInf(QString nameIn, double x0In, double y0In,
		double vx0In, double vy0In)
		: name(nameIn), x0(x0In), y0(y0In), vx0(vx0In), vy0(vy0In) {
		WorkClear();
	}
	void WorkClear(void) {
		x = x0;
		x1 = 0;
		y = y0;
		y1 = 0;
	}
	void WorkInt() {
		x = x0;
		y = y0;
		x1 = x0 + MY_DT * vx0;
		y1 = y0 + MY_DT * vy0;
	}
	void Work(double &xout, double &yout) {
		double xOld = x1, yOld = y1;
		r = sqrt((x) * (x) + (y) * (y));
		x1 = -MY_G * MY_SUN_M * (x) / pow(r, 3) * pow(MY_DT,
				2) + 2 * (x1) - (x);
		y1 = -MY_G * MY_SUN_M * (y) / pow(r, 3) * pow(MY_DT,
				2) + 2 * (y1) - (y);
		x = xOld;
		y = yOld;
		xout = x1;
		yout = y1;
	}
  private:
	QString name;
	double x0;
	double y0;
	double vx0;
	double vy0;
	double x, x1;
	double y, y1;
	double r;
};

class Widget : public QWidget {
	Q_OBJECT

  public:
	Widget(QWidget *parent = nullptr);
	~Widget();
	void paintEvent(QPaintEvent *event);
	void keyPressEvent(QKeyEvent *event);

  private:
	Ui::Widget *ui;
	QPainter painter;
	QPen pen;
	QLineF line;
	/* 当前行星编号 */
	int planetFlag = 0;
	/* 不同的行星 */
	PlanetInf *plane1;
	/* 当前绘制的曲线 */
	PlanetInf *planeRun;
	QTimer *Timer;
	/* 绘图状态机 */
	int runState = 0;
	int px, py;
	QPixmap pixmap = QPixmap(800, 800);
};


#endif // WIDGET_H
