#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
	void startPlotting(int f);
	
	bool viewFrame1;
	bool viewFrame2;
	bool viewFrame3;
	QList<QVector<double>> speedCoordinates; //contains (references to) the full history of speed coordinates
	QList<QVector<double>> batteryCoordinates; //contains (references to) the full history of battery level coordinates
	QList<QVector<double>> powerCoordinates; //contains (references to) the full history of power coordinates
		
public slots:
		void toggleViewFrame1(bool state);
		void toggleViewFrame2(bool state);
		void toggleViewFrame3(bool state);
		void getData();
		//void setData(QPoint thePoint);
		void plotData();
		void lol(bool unused);
  
private slots:
  void titleDoubleClick(QMouseEvent *event);
  void axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part);
  void legendDoubleClick(QCPLegend* legend, QCPAbstractLegendItem* item);
  void selectionChanged();
  void mousePress();
  void mouseWheel();
  void addRandomGraph();
  void removeSelectedGraph();
  void removeAllGraphs1();
  void removeAllGraphs2();
  void removeAllGraphs3();
  void contextMenuRequest1(QPoint pos);
  void contextMenuRequest2(QPoint pos);
  void contextMenuRequest3(QPoint pos);  
  void moveLegend1();
  void moveLegend2();
  void moveLegend3();
  void graphClicked(QCPAbstractPlottable *plottable, int dataIndex);
  
private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H