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
	QList<QVector<double>> chargeCoordinates; //contains (references to) the full history of charge level coordinates
	QList<QVector<double>> powerCoordinates; //contains (references to) the full history of power coordinates
	QList<QVector<double>> energyCoordinates; //contains (references to) the full history of energy coordinates (in watt hours)
	QList<QVector<double>> currentCoordinates; //contains (references to) the full history of current coordinates
	QList<QVector<double>> voltageCoordinates; //contains (references to) the full history of instant coordinates
	QList<QVector<double>> ampHourCoordinates; //contains (references to) the full history of pack amp hour coordinates
	QList<QVector<double>> temperatureCoordinates;
	
	//some variables that I'm guessing are quantitative values?
	//QList<QVector<double>> DCL_Low_SOCCoordinates;
	//QList<QVector<double>> DCL_High_Cell_ResistanceCoordinates;
	//QList<QVector<double>> DCL_Low_Cell_VoltageCoordinates;
	//QList<QVector<double>> DCL_Low_Pack_VoltageCoordinates;
	
	//Messages(?)
	bool DCL_Low_SOC;
	bool DCL_High_Cell_Resistance;
	bool DCL_Temperature;
	bool DCL_Low_Cell_Voltage;
	bool DCL_Low_Pack_Voltage;
	bool DCL_Voltage_Failsafe;
	bool CCL_High_SOC;
	bool CCL_High_Cell_Resistance;
	bool CCL_Temperature;	
	bool CCL_High_Cell_Voltage;
	bool CCL_High_Pack_Voltage;
	bool CCL_Charger_Latch;
	bool discharge_relay_disabled;
	bool charge_relay_disabled;
	bool charger_safety_disabled;
	bool diagnostic_trouble_code_active;
	bool always_on_power_status;
	bool is_ready_power_status;
	bool is_charging_power_status;
	bool High_Temperature;
	bool Low_Temperature;
	
	int index; //index into graphs
	
public slots:
		void toggleViewFrame1(bool state);
		void toggleViewFrame2(bool state);
		void toggleViewFrame3(bool state);
		void getUSBData(QString usbFileName);
		void getData();
		//void setData(QPoint thePoint);
		void plotData();
		void updateMessages(int message, int value);
		void lol(bool unused);
		
signals:
	//void sig_rightClicked(QPoint);
	void sig_DCL_Low_SOC(int);
	void sig_DCL_High_Cell_Resistance(int);
	void sig_DCL_Temperature(int); //not sure if needed
	void sig_DCL_Low_Cell_Voltage(int);
	void sig_DCL_Low_Pack_Voltage(int);
	void sig_DCL_Voltage_Failsafe(int);
	void sig_CCL_High_SOC(int);
	void sig_CCL_High_Cell_Resistance(int);
	void sig_CCL_Temperature(int);
	void sig_CCL_High_Cell_Voltage(int);
	void sig_CCL_High_Pack_Voltage(int);
	void sig_CCL_Charger_Latch(int);
	void sig_discharge_relay_disabled(int);
	void sig_charge_relay_disabled(int);
	void sig_charger_safety_disabled(int);
	void sig_diagnostic_trouble_code_active(int);
	void sig_always_on_power_status(int);
	void sig_is_ready_power_status(int);
	void sig_is_charging_power_status(int);
	void sig_Pack_Amp_Hours(int);
	void sig_High_Temperature(int);
	void sig_Low_Temperature(int);
	void sig_Pack_Current(int);
	void sig_Pack_Instant_Voltage(int);
	void sig_State_Of_Charge(int);
	void sig_Relay_Status(int);
	void sig_Watt_Hours(int);
	
		
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
