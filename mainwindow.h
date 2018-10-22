#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include "C:/Users/jonat/Documents/Qt_Projects/PitDisplay/qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
	bool viewFrame1;
	bool viewFrame2;
	bool viewFrame3;
		
public slots:
		void toggleViewFrame1(bool state);
		void toggleViewFrame2(bool state);
		void toggleViewFrame3(bool state);
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
