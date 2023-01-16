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
#include <QVector>
#include <QtMath>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

#define MY_SUN_M (1) /* 太阳质量 */
#define MY_G (0.000295989) /* 万有引力常量 */

class PlanetInf {
  public:
	PlanetInf(QString nameIn, double x0In, double y0In,
		double vx0In, double vy0In, double dtIn)
		: name(nameIn), x0(x0In), y0(y0In), vx0(vx0In), vy0(vy0In), dt(dtIn) {
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
		x1 = x0 + dt * vx0;
		y1 = y0 + dt * vy0;
	}
	void Work(double &xout, double &yout) {
		double xOld = x1, yOld = y1;
		r = sqrt((x) * (x) + (y) * (y));
		x1 = -MY_G * MY_SUN_M * (x) / pow(r, 3) * pow(dt,
				2) + 2 * (x1) - (x);
		y1 = -MY_G * MY_SUN_M * (y) / pow(r, 3) * pow(dt,
				2) + 2 * (y1) - (y);
		x = xOld;
		y = yOld;
		xout = x1;
		yout = y1;
	}
	QString name;
  private:
	double x0;
	double y0;
	double vx0;
	double vy0;
	double dt;
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
	void PixmapInit(void);
	void PlanetInit(void);

  private:
	Ui::Widget *ui;
	QPainter painter;
	QPen pen;
	QLineF line;
	/* 当前行星编号 */
	int planetFlag = 0;
	const int plantMax = 8;
	/* 不同的行星 */
	PlanetInf plane1 = PlanetInf(QString("水星"), 0.466699373,
			0, 0, 0.0224454181635083, 0.0001);
	PlanetInf plane2 = PlanetInf(QString("金星"), 0.728230404304,
			0, 0, 0.0200920831078843, 0.001);
	PlanetInf plane3 = PlanetInf(QString("地球"), 1.01669516395119,
			0, 0, 0.0169192315430405, 0.001);
	PlanetInf plane4 = PlanetInf(QString("火星"), 1.66600940556207,
			0, 0, 0.0126911527706869, 0.001);
	PlanetInf plane5 = PlanetInf(QString("木星"), 5.45889516,
			0, 0, 0.00718117658914553, 0.005);
	PlanetInf plane6 = PlanetInf(QString("土星"), 10.1240169,
			0, 0, 0.00525205679561466, 0.01);
	PlanetInf plane7 = PlanetInf(QString("天王星"), 20.1097686104,
			0, 0, 0.0037464401867908, 0.01);
	PlanetInf plane8 = PlanetInf(QString("海王星"), 30.33094746,
			0, 0, 0.00311027848501303, 0.02);
	PlanetInf plane9 = PlanetInf(QString("冥王星"), 49.3051216,
			0, 0, 0.00212357040837083, 0.02);
	QVector<PlanetInf *> planes{&plane1,
				&plane2, &plane3, &plane4,
				&plane5, &plane6, &plane7,
				&plane8, &plane9,};
	/* 当前绘制的曲线 */
	PlanetInf *planeRun;
	QTimer *Timer;
	/* 绘图状态机 */
	int runState = 0;
	int px, py;
	QPixmap pixmap = QPixmap(800, 800);
};


#endif // WIDGET_H
