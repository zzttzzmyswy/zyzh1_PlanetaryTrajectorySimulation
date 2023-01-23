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
	/* 初始话行星参数 名字 初始坐标 初始速度 仿真时间步长 */
	PlanetInf(QString nameIn, double x0In, double y0In,
		double vx0In, double vy0In, double dtIn)
		: name(nameIn), x0(x0In), y0(y0In), vx0(vx0In), vy0(vy0In), dt0(dtIn) {
		WorkInt();
	}
	/* 为迭代计算初始化参数 */
	void WorkInt(double dtIn = 0) {
		dt = dtIn == 0 ? dt0 : dtIn;
		x = x0;
		y = y0;
		x1 = x0 + dt * vx0;
		y1 = y0 + dt * vy0;
	}
	/* 进行迭代计算 */
	void Work(double &xout, double &yout, double dtIn = 0) {
		xOld = x1, yOld = y1;
		dt = dtIn == 0 ? dt0 : dtIn;
		r = sqrt((x) * (x) + (y) * (y));
		/* 由行星运动公式差分获得 */
		x1 = -MY_G * MY_SUN_M * (x) / pow(r, 3) * pow(dt,
				2) + 2 * (x1) - (x);
		y1 = -MY_G * MY_SUN_M * (y) / pow(r, 3) * pow(dt,
				2) + 2 * (y1) - (y);
		x = xOld;
		y = yOld;
		xout = x1;
		yout = y1;
#if 1 /* 用于刷新数据，抵消掉低仿真精度带来的误差 */
		if (x1 > 0 && abs(y1) < 0.001 && flashFlag == false) {
			WorkInt(dt);
			flashFlag = true;
		}
		else if (x1 < 0 && abs(y1) < 0.001 && flashFlag == true)
			flashFlag = false;
#endif
	}
	QString name;
	double x, x1, xOld;
	double y, y1, yOld;
  private:
	double x0;/* 起始坐标 */
	double y0;
	double vx0;/* 其实初速度 */
	double vy0;
	double dt0, dt; /* 时间步长 */
	double r;
	/* 用于刷新数据，抵消掉低仿真精度带来的误差 */
	bool flashFlag = false;
};

class Widget : public QWidget {
	Q_OBJECT

  public:
	Widget(QWidget *parent = nullptr);
	~Widget();
	void keyPressEvent(QKeyEvent *event);
	void PixmapInit(void);
	void PlanetInit(void);

  private slots:
	void on_pushButton_clicked();

  private:
	Ui::Widget *ui;
	/* 当前行星编号 */
	int planetFlag = 0;
	const int plantMax = 8;
#ifndef TARGET_ARCH_ARM /* 选择不同的仿真时间步长以在不同的平台上流畅运行 */
	/* 不同的行星 */
	PlanetInf plane1 = PlanetInf(QString("水星"), 0.466699373,
			0, 0, 0.0224454181635083, 0.0001);
	PlanetInf plane2 = PlanetInf(QString("金星"), 0.728230404304,
			0, 0, 0.0200920831078843, 0.0005);
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
#else /* 当在低性能arm平台运行时其精度会降低，需要刷新代码约束误差 */
	PlanetInf plane1 = PlanetInf(QString("水星"), 0.466699373,
			0, 0, 0.0224454181635083, 0.025);
	PlanetInf plane2 = PlanetInf(QString("金星"), 0.728230404304,
			0, 0, 0.0200920831078843, 0.04);
	PlanetInf plane3 = PlanetInf(QString("地球"), 1.01669516395119,
			0, 0, 0.0169192315430405, 0.05);
	PlanetInf plane4 = PlanetInf(QString("火星"), 1.66600940556207,
			0, 0, 0.0126911527706869, 0.1);
	PlanetInf plane5 = PlanetInf(QString("木星"), 5.45889516,
			0, 0, 0.00718117658914553, 0.2);
	PlanetInf plane6 = PlanetInf(QString("土星"), 10.1240169,
			0, 0, 0.00525205679561466, 0.5);
	PlanetInf plane7 = PlanetInf(QString("天王星"), 20.1097686104,
			0, 0, 0.0037464401867908, 1);
	PlanetInf plane8 = PlanetInf(QString("海王星"), 30.33094746,
			0, 0, 0.00311027848501303, 2);
	PlanetInf plane9 = PlanetInf(QString("冥王星"), 49.3051216,
			0, 0, 0.00212357040837083, 3);
#endif
	QVector<PlanetInf *> planes {&plane1,
				&plane2, &plane3, &plane4,
				&plane5, &plane6, &plane7,
				&plane8, &plane9,
	};
	/* 当前绘制的曲线 */
	PlanetInf *planeRun;
	QTimer *Timer;
	/* 绘制点与连线需要的变量 */
	int px, py, pxOld = 0, pyOld = 0;
	int pxOlds[9];
	int pyOlds[9];
	/* 绘制底层需要的对象 */
	QPainter painter;
	QPen pen;
	QLineF line;
	/* 绘制顶层所需要的对象 */
	QPainter painterTop;
	QPen penTop;
	QPixmap pixmap = QPixmap(500, 500);
	QPixmap pixmapTop = QPixmap(500, 500);
	QColor colorTop;
};


#endif // WIDGET_H
