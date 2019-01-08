#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include <QDebug>
#include <QTimer>
//#include "tmmessage.h"
#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  usbfp("C:\\Users\\jonat\\Documents\\Qt_Projects\\Log.txt", std::ifstream::in)

{
	ui->setupUi(this);
	
	//Graphs menu
	this->connect(this->ui->viewGraph1_checkbox, SIGNAL(toggled(bool)), this, SLOT(toggleViewFrame1(bool)));	
	this->connect(this->ui->viewGraph2_checkbox, SIGNAL(toggled(bool)), this, SLOT(toggleViewFrame2(bool)));
	this->connect(this->ui->viewGraph3_checkbox, SIGNAL(toggled(bool)), this, SLOT(toggleViewFrame3(bool)));
	
	//Help menu
	this->connect(this->ui->actionHelp_me, SIGNAL(triggered(bool)), this, SLOT(lol(bool)));
	
//Messages
	this->connect(this, SIGNAL(sig_DCL_Low_SOC(int, int)), this, SLOT(updateMessages(int, int)));
	
	
	this->viewFrame1 = true;
	this->viewFrame2 = true;
	
	this->ui->Frame1->show();
	this->ui->Frame2->show();

	index = 0;
	//numFrames = 0;	
	
	//numberVectors = new QList<int>;
	for (int i = 0; i < 8; i++) {
		numberVectors.append(0);		
	}

//Graph1
  //srand(QDateTime::currentDateTime().toTime_t());
  
  ui->Graph1->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                  QCP::iSelectLegend | QCP::iSelectPlottables);
  ui->Graph1->xAxis->setRange(-8, 8);
  ui->Graph1->yAxis->setRange(-5, 5);
  ui->Graph1->axisRect()->setupFullAxesBox();
  
  ui->Graph1->plotLayout()->insertRow(0);
  QCPTextElement *title = new QCPTextElement(ui->Graph1, "Speed", QFont("sans", 17, QFont::Bold));
  ui->Graph1->plotLayout()->addElement(0, 0, title);
  
  ui->Graph1->xAxis->setLabel("Time");
  ui->Graph1->yAxis->setLabel("Speed");
  ui->Graph1->legend->setVisible(true);
  QFont legendFont = font();
  legendFont.setPointSize(10);
  ui->Graph1->legend->setFont(legendFont);
  ui->Graph1->legend->setSelectedFont(legendFont);
  ui->Graph1->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items
  this->ui->Graph1->addGraph();
  //addRandomGraph();
  //addRandomGraph();
  //addRandomGraph();
  //addRandomGraph();
  ui->Graph1->rescaleAxes();
  
  // connect slot that ties some axis selections together (especially opposite axes):
  connect(ui->Graph1, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
  // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
  connect(ui->Graph1, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
  connect(ui->Graph1, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
  
  // make bottom and left axes transfer their ranges to top and right axes:
  connect(ui->Graph1->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->Graph1->xAxis2, SLOT(setRange(QCPRange)));
  connect(ui->Graph1->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->Graph1->yAxis2, SLOT(setRange(QCPRange)));
  
  // connect some interaction slots:
  connect(ui->Graph1, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
  connect(ui->Graph1, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));
  connect(title, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));
  
  // connect slot that shows a message in the status bar when a graph is clicked:
  connect(ui->Graph1, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*,int)));
  
  // setup policy and connect slot for context menu popup:
  ui->Graph1->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui->Graph1, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest1(QPoint)));
  
  
//Graph2
  ui->Graph2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                  QCP::iSelectLegend | QCP::iSelectPlottables);
  ui->Graph2->xAxis->setRange(-10, 10);
  ui->Graph2->yAxis->setRange(-10, 10);
  ui->Graph2->axisRect()->setupFullAxesBox();
  
  ui->Graph2->plotLayout()->insertRow(0);
  QCPTextElement *graph2Title = new QCPTextElement(ui->Graph2, "Charge", QFont("sans", 17, QFont::Bold));
  ui->Graph2->plotLayout()->addElement(0, 0, graph2Title);
  
  ui->Graph2->xAxis->setLabel("x Axis label (e.g. time)");
  ui->Graph2->yAxis->setLabel("y Axis label (e.g. velocity)");
  ui->Graph2->legend->setVisible(true);
  //QFont legendFont = font();
  legendFont.setPointSize(10);
  ui->Graph2->legend->setFont(legendFont);
  ui->Graph2->legend->setSelectedFont(legendFont);
  ui->Graph2->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items
  this->ui->Graph2->addGraph();
  //addRandomGraph();
  //addRandomGraph();
  //addRandomGraph();
  //addRandomGraph();
  ui->Graph2->rescaleAxes();
  
  // connect slot that ties some axis selections together (especially opposite axes):
  connect(ui->Graph2, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
  // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
  connect(ui->Graph2, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
  connect(ui->Graph2, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
  
  // make bottom and left axes transfer their ranges to top and right axes:
  connect(ui->Graph2->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->Graph2->xAxis2, SLOT(setRange(QCPRange)));
  connect(ui->Graph2->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->Graph2->yAxis2, SLOT(setRange(QCPRange)));
  
  // connect some interaction slots:
  connect(ui->Graph2, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
  connect(ui->Graph2, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));
  connect(graph2Title, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));
  
  // connect slot that shows a message in the status bar when a graph is clicked:
  connect(ui->Graph2, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*,int)));
  
  // setup policy and connect slot for context menu popup:
  ui->Graph2->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui->Graph2, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest2(QPoint)));

//Graph3
  ui->Graph3->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                  QCP::iSelectLegend | QCP::iSelectPlottables);
  ui->Graph3->xAxis->setRange(-10, 10);
  ui->Graph3->yAxis->setRange(-10, 10);
  ui->Graph3->axisRect()->setupFullAxesBox();
  
  ui->Graph3->plotLayout()->insertRow(0);
  QCPTextElement *graph3Title = new QCPTextElement(ui->Graph3, "Power", QFont("sans", 17, QFont::Bold));
  ui->Graph3->plotLayout()->addElement(0, 0, graph3Title);
  
  ui->Graph3->xAxis->setLabel("Time");
  ui->Graph3->yAxis->setLabel("Power");
  ui->Graph3->legend->setVisible(true);
  //QFont legendFont = font();
  legendFont.setPointSize(10);
  ui->Graph3->legend->setFont(legendFont);
  ui->Graph3->legend->setSelectedFont(legendFont);
  ui->Graph3->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items
  this->ui->Graph3->addGraph();
  //addRandomGraph();
  //addRandomGraph();
  //addRandomGraph();
  //addRandomGraph();
  ui->Graph3->rescaleAxes();
  
  // connect slot that ties some axis selections together (especially opposite axes):
  connect(ui->Graph3, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
  // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
  connect(ui->Graph3, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
  connect(ui->Graph3, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
  
  // make bottom and left axes transfer their ranges to top and right axes:
  connect(ui->Graph3->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->Graph3->xAxis2, SLOT(setRange(QCPRange)));
  connect(ui->Graph3->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->Graph3->yAxis2, SLOT(setRange(QCPRange)));
  
  // connect some interaction slots:
  connect(ui->Graph3, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
  connect(ui->Graph3, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));
  connect(graph3Title, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));
  
  // connect slot that shows a message in the status bar when a graph is clicked:
  connect(ui->Graph3, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*,int)));
  
  // setup policy and connect slot for context menu popup:
  ui->Graph3->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui->Graph3, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest3(QPoint)));
  
  //graph 4
  ui->Graph4->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                  QCP::iSelectLegend | QCP::iSelectPlottables);
  ui->Graph4->xAxis->setRange(-10, 10);
  ui->Graph4->yAxis->setRange(-10, 10);
  ui->Graph4->axisRect()->setupFullAxesBox();
  
  ui->Graph4->plotLayout()->insertRow(0);
  QCPTextElement *graph4Title = new QCPTextElement(ui->Graph4, "Energy", QFont("sans", 17, QFont::Bold));
  ui->Graph4->plotLayout()->addElement(0, 0, graph4Title);
  
  ui->Graph4->xAxis->setLabel("Time");
  ui->Graph4->yAxis->setLabel("Power");
  ui->Graph4->legend->setVisible(true);
  //QFont legendFont = font();
  legendFont.setPointSize(10);
  ui->Graph4->legend->setFont(legendFont);
  ui->Graph4->legend->setSelectedFont(legendFont);
  ui->Graph4->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items
  this->ui->Graph4->addGraph();
  //addRandomGraph();
  //addRandomGraph();
  //addRandomGraph();
  //addRandomGraph();
  ui->Graph4->rescaleAxes();
  
  // connect slot that ties some axis selections together (especially opposite axes):
  connect(ui->Graph4, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
  // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
  connect(ui->Graph4, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
  connect(ui->Graph4, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
  
  // make bottom and left axes transfer their ranges to top and right axes:
  connect(ui->Graph4->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->Graph4->xAxis2, SLOT(setRange(QCPRange)));
  connect(ui->Graph4->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->Graph4->yAxis2, SLOT(setRange(QCPRange)));
  
  // connect some interaction slots:
  connect(ui->Graph4, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
  connect(ui->Graph4, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));
  connect(graph4Title, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));
  
  // connect slot that shows a message in the status bar when a graph is clicked:
  connect(ui->Graph4, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*,int)));
  
  // setup policy and connect slot for context menu popup:
  ui->Graph4->setContextMenuPolicy(Qt::CustomContextMenu);
  //connect(ui->Graph4, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest3(QPoint)));
  
  //graph 5
  ui->Graph5->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                  QCP::iSelectLegend | QCP::iSelectPlottables);
  ui->Graph5->xAxis->setRange(-10, 10);
  ui->Graph5->yAxis->setRange(-10, 10);
  ui->Graph5->axisRect()->setupFullAxesBox();
  
  ui->Graph5->plotLayout()->insertRow(0);
  QCPTextElement *graph5Title = new QCPTextElement(ui->Graph5, "Current", QFont("sans", 17, QFont::Bold));
  ui->Graph5->plotLayout()->addElement(0, 0, graph5Title);
  
  ui->Graph5->xAxis->setLabel("Time");
  ui->Graph5->yAxis->setLabel("Current");
  ui->Graph5->legend->setVisible(true);
  //QFont legendFont = font();
  legendFont.setPointSize(10);
  ui->Graph5->legend->setFont(legendFont);
  ui->Graph5->legend->setSelectedFont(legendFont);
  ui->Graph5->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items
  this->ui->Graph5->addGraph();
  //addRandomGraph();
  //addRandomGraph();
  //addRandomGraph();
  //addRandomGraph();
  ui->Graph5->rescaleAxes();
  
  // connect slot that ties some axis selections together (especially opposite axes):
  connect(ui->Graph5, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
  // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
  connect(ui->Graph5, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
  connect(ui->Graph5, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
  
  // make bottom and left axes transfer their ranges to top and right axes:
  connect(ui->Graph5->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->Graph5->xAxis2, SLOT(setRange(QCPRange)));
  connect(ui->Graph5->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->Graph5->yAxis2, SLOT(setRange(QCPRange)));
  
  // connect some interaction slots:
  connect(ui->Graph5, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
  connect(ui->Graph5, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));
  connect(graph5Title, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));
  
  // connect slot that shows a message in the status bar when a graph is clicked:
  connect(ui->Graph5, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*,int)));
  
  // setup policy and connect slot for context menu popup:
  ui->Graph5->setContextMenuPolicy(Qt::CustomContextMenu);
  //connect(ui->Graph4, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest3(QPoint)));
  
  //graph 6
  ui->Graph6->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                  QCP::iSelectLegend | QCP::iSelectPlottables);
  ui->Graph6->xAxis->setRange(-10, 10);
  ui->Graph6->yAxis->setRange(-10, 10);
  ui->Graph6->axisRect()->setupFullAxesBox();
  
  ui->Graph6->plotLayout()->insertRow(0);
  QCPTextElement *graph6Title = new QCPTextElement(ui->Graph6, "Voltage", QFont("sans", 17, QFont::Bold));
  ui->Graph6->plotLayout()->addElement(0, 0, graph6Title);
  
  ui->Graph6->xAxis->setLabel("Time");
  ui->Graph6->yAxis->setLabel("Voltage");
  ui->Graph6->legend->setVisible(true);
  //QFont legendFont = font();
  legendFont.setPointSize(10);
  ui->Graph6->legend->setFont(legendFont);
  ui->Graph6->legend->setSelectedFont(legendFont);
  ui->Graph6->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items
  this->ui->Graph6->addGraph();
  //addRandomGraph();
  //addRandomGraph();
  //addRandomGraph();
  //addRandomGraph();
  ui->Graph6->rescaleAxes();
  
  // connect slot that ties some axis selections together (especially opposite axes):
  connect(ui->Graph6, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
  // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
  connect(ui->Graph6, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
  connect(ui->Graph6, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
  
  // make bottom and left axes transfer their ranges to top and right axes:
  connect(ui->Graph6->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->Graph6->xAxis2, SLOT(setRange(QCPRange)));
  connect(ui->Graph6->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->Graph6->yAxis2, SLOT(setRange(QCPRange)));
  
  // connect some interaction slots:
  connect(ui->Graph6, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
  connect(ui->Graph6, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));
  connect(graph6Title, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));
  
  // connect slot that shows a message in the status bar when a graph is clicked:
  connect(ui->Graph6, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*,int)));
  
  // setup policy and connect slot for context menu popup:
  ui->Graph6->setContextMenuPolicy(Qt::CustomContextMenu);
  //connect(ui->Graph4, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest3(QPoint)));
  
  //graph 7
  ui->Graph7->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                  QCP::iSelectLegend | QCP::iSelectPlottables);
  ui->Graph7->xAxis->setRange(-10, 10);
  ui->Graph7->yAxis->setRange(-10, 10);
  ui->Graph7->axisRect()->setupFullAxesBox();
  
  ui->Graph7->plotLayout()->insertRow(0);
  QCPTextElement *graph7Title = new QCPTextElement(ui->Graph7, "Amp Hours", QFont("sans", 12, QFont::Bold));
  ui->Graph7->plotLayout()->addElement(0, 0, graph7Title);
  
  ui->Graph7->xAxis->setLabel("Time");
  ui->Graph7->yAxis->setLabel("Amp Hours");
  ui->Graph7->legend->setVisible(true);
  //QFont legendFont = font();
  legendFont.setPointSize(10);
  ui->Graph7->legend->setFont(legendFont);
  ui->Graph7->legend->setSelectedFont(legendFont);
  ui->Graph7->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items
  this->ui->Graph7->addGraph();
  //addRandomGraph();
  //addRandomGraph();
  //addRandomGraph();
  //addRandomGraph();
  ui->Graph7->rescaleAxes();
  
  // connect slot that ties some axis selections together (especially opposite axes):
  connect(ui->Graph7, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
  // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
  connect(ui->Graph7, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
  connect(ui->Graph7, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
  
  // make bottom and left axes transfer their ranges to top and right axes:
  connect(ui->Graph7->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->Graph7->xAxis2, SLOT(setRange(QCPRange)));
  connect(ui->Graph7->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->Graph7->yAxis2, SLOT(setRange(QCPRange)));
  
  // connect some interaction slots:
  connect(ui->Graph7, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
  connect(ui->Graph7, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));
  connect(graph7Title, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));
  
  // connect slot that shows a message in the status bar when a graph is clicked:
  connect(ui->Graph7, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*,int)));
  
  // setup policy and connect slot for context menu popup:
  ui->Graph7->setContextMenuPolicy(Qt::CustomContextMenu);
  //connect(ui->Graph4, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest3(QPoint)));
  
  //graph 8
  ui->Graph8->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                  QCP::iSelectLegend | QCP::iSelectPlottables);
  ui->Graph8->xAxis->setRange(-10, 10);
  ui->Graph8->yAxis->setRange(-10, 10);
  ui->Graph8->axisRect()->setupFullAxesBox();
  
  ui->Graph8->plotLayout()->insertRow(0);
  QCPTextElement *graph8Title = new QCPTextElement(ui->Graph8, "Temperature", QFont("sans", 12, QFont::Bold));
  ui->Graph8->plotLayout()->addElement(0, 0, graph8Title);
  
  ui->Graph8->xAxis->setLabel("Time");
  ui->Graph8->yAxis->setLabel("Temperature");
  ui->Graph8->legend->setVisible(true);
  //QFont legendFont = font();
  legendFont.setPointSize(10);
  ui->Graph8->legend->setFont(legendFont);
  ui->Graph8->legend->setSelectedFont(legendFont);
  ui->Graph8->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items
  this->ui->Graph8->addGraph();
  //addRandomGraph();
  //addRandomGraph();
  //addRandomGraph();
  //addRandomGraph();
  ui->Graph8->rescaleAxes();
  
  // connect slot that ties some axis selections together (especially opposite axes):
  connect(ui->Graph8, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
  // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
  connect(ui->Graph8, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
  connect(ui->Graph8, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
  
  // make bottom and left axes transfer their ranges to top and right axes:
  connect(ui->Graph8->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->Graph8->xAxis2, SLOT(setRange(QCPRange)));
  connect(ui->Graph8->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->Graph8->yAxis2, SLOT(setRange(QCPRange)));
  
  // connect some interaction slots:
  connect(ui->Graph8, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
  connect(ui->Graph8, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));
  connect(graph8Title, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));
  
  // connect slot that shows a message in the status bar when a graph is clicked:
  connect(ui->Graph8, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*,int)));
  
  // setup policy and connect slot for context menu popup:
  ui->Graph8->setContextMenuPolicy(Qt::CustomContextMenu);
  //connect(ui->Graph4, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest3(QPoint)));
  
  //make a call to a function that will re-plot the graphs every 0.1 seconds
  startPlotting(10);
  
  //initialize the coordinate vectors
  
  //speedCoordinates = new QVector<QVector<double>>; //contains (references to) the full history of speed coordinates
	QVector<double> emptyVectorA; //not sure how to initialize this without putting in any values that don't belong in there
	speedCoordinates.append(emptyVectorA); //I'm afraid to reuse the same vector every time because vectors are weird about pointers
  //batteryCoordinates = new QVector<QVector<double>>; //contains (references to) the full history of battery level coordinates
	QVector<double> emptyVectorB; 
	chargeCoordinates.append(emptyVectorB);
 // powerCoordinates = new QVector<QVector<double>>; //contains (references to) the full history of power coordinates
  QVector<double> emptyVectorC; 
	powerCoordinates.append(emptyVectorC);
	QVector<double> emptyVectorD; 
	  energyCoordinates.append(emptyVectorD);
	QVector<double> emptyVectorE;
	currentCoordinates.append(emptyVectorE);
	QVector<double> emptyVectorF;
	voltageCoordinates.append(emptyVectorF);
	QVector<double> emptyVectorG;
	ampHourCoordinates.append(emptyVectorG);
	QVector<double> emptyVectorH;
	temperatureCoordinates.append(emptyVectorH);

    // for now, open the TM file by creating a TMMessage object in the constructor

}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::startPlotting(int f) { //repeatedly calls setData() at frequency f
	qDebug() << "I got to startPlotting; the frequency is" << f;
	if (f < 0) { //check input for validity
		f = f * -1;
	}
	else if (f == 0) {
		f = 1;
	}
	
	double period = 1000 * 1/(double)f; //now period is in terms of milliseconds
	qDebug() << "about to do the timer thing I think; the period is" << period << "milliseconds";
	QTimer *timer = new QTimer(this);
	   connect(timer, SIGNAL(timeout()), this, SLOT(getData())); //this line is weird, it thinks there's an error
	   timer->start((int)period);
}

void MainWindow::titleDoubleClick(QMouseEvent* event)
{
  Q_UNUSED(event)
  if (QCPTextElement *title = qobject_cast<QCPTextElement*>(sender()))
  {
    // Set the plot title by double clicking on it
    bool ok;
    QString newTitle = QInputDialog::getText(this, "QCustomPlot example", "New plot title:", QLineEdit::Normal, title->text(), &ok);
    if (ok)
    {
      title->setText(newTitle);
      ui->Graph1->replot();
    }
  }
}

void MainWindow::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  // Set an axis label by double clicking on it
  if (part == QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
  {
    bool ok;
    QString newLabel = QInputDialog::getText(this, "QCustomPlot example", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
    if (ok)
    {
      axis->setLabel(newLabel);
      ui->Graph1->replot();
    }
  }
}

void MainWindow::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
  // Rename a graph by double clicking on its legend item
  Q_UNUSED(legend)
  if (item) // only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0)
  {
    QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
    bool ok;
    QString newName = QInputDialog::getText(this, "QCustomPlot example", "New graph name:", QLineEdit::Normal, plItem->plottable()->name(), &ok);
    if (ok)
    {
      plItem->plottable()->setName(newName);
      ui->Graph1->replot();
    }
  }
}

void MainWindow::selectionChanged()
{
  /*
   normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
   the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
   and the axis base line together. However, the axis label shall be selectable individually.
   
   The selection state of the left and right axes shall be synchronized as well as the state of the
   bottom and top axes.
   
   Further, we want to synchronize the selection of the graphs with the selection state of the respective
   legend item belonging to that graph. So the user can select a graph by either clicking on the graph itself
   or on its legend item.
  */
  
  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->Graph1->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->Graph1->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->Graph1->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->Graph1->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    this->ui->Graph1->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->Graph1->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->Graph1->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->Graph1->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->Graph1->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->Graph1->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->Graph1->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->Graph1->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  
  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<ui->Graph1->graphCount(); ++i)
  {
    QCPGraph *graph = ui->Graph1->graph(i);
    QCPPlottableLegendItem *item = ui->Graph1->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
    }
  }
}

void MainWindow::mousePress()
{
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged
  
  if (ui->Graph1->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->Graph1->axisRect()->setRangeDrag(ui->Graph1->xAxis->orientation());
  else if (ui->Graph1->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->Graph1->axisRect()->setRangeDrag(ui->Graph1->yAxis->orientation());
  else
    ui->Graph1->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::mouseWheel()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed
  
  if (ui->Graph1->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->Graph1->axisRect()->setRangeZoom(ui->Graph1->xAxis->orientation());
  else if (ui->Graph1->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->Graph1->axisRect()->setRangeZoom(ui->Graph1->yAxis->orientation());
  else
    ui->Graph1->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::addRandomGraph()
{
  int n = 50; // number of points in graph
  double xScale = (rand()/(double)RAND_MAX + 0.5)*2;
  double yScale = (rand()/(double)RAND_MAX + 0.5)*2;
  double xOffset = (rand()/(double)RAND_MAX - 0.5)*4;
  double yOffset = (rand()/(double)RAND_MAX - 0.5)*10;
  double r1 = (rand()/(double)RAND_MAX - 0.5)*2;
  double r2 = (rand()/(double)RAND_MAX - 0.5)*2;
  double r3 = (rand()/(double)RAND_MAX - 0.5)*2;
  double r4 = (rand()/(double)RAND_MAX - 0.5)*2;
  QVector<double> x(n), y(n);
  for (int i=0; i<n; i++)
  {
    x[i] = (i/(double)n-0.5)*10.0*xScale + xOffset;
    y[i] = (qSin(x[i]*r1*5)*qSin(qCos(x[i]*r2)*r4*3)+r3*qCos(qSin(x[i])*r4*2))*yScale + yOffset;
  }
  
  ui->Graph1->addGraph();
  ui->Graph1->graph()->setName(QString("New graph %1").arg(ui->Graph1->graphCount()-1));
  ui->Graph1->graph()->setData(x, y);
  ui->Graph1->graph()->setLineStyle((QCPGraph::LineStyle)(rand()%5+1));
  if (rand()%100 > 50)
    ui->Graph1->graph()->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(rand()%14+1)));
  QPen graphPen;
  graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
  graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
  ui->Graph1->graph()->setPen(graphPen);
  ui->Graph1->replot();
}

void MainWindow::removeSelectedGraph()
{
  if (ui->Graph1->selectedGraphs().size() > 0)
  {
    ui->Graph1->removeGraph(ui->Graph1->selectedGraphs().first());
    ui->Graph1->replot();
  }
}

void MainWindow::removeAllGraphs1()
{
  ui->Graph1->clearGraphs();
  ui->Graph1->replot();
}

void MainWindow::removeAllGraphs2()
{
  ui->Graph2->clearGraphs();
  ui->Graph2->replot();
}

void MainWindow::removeAllGraphs3()
{
  ui->Graph3->clearGraphs();
  ui->Graph3->replot();
}

void MainWindow::contextMenuRequest1(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);
  
  if (ui->Graph1->legend->selectTest(pos, false) >= 0) // context menu on legend requested
  {
    menu->addAction("Move to top left", this, SLOT(moveLegend1()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    menu->addAction("Move to top center", this, SLOT(moveLegend1()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
    menu->addAction("Move to top right", this, SLOT(moveLegend1()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
    menu->addAction("Move to bottom right", this, SLOT(moveLegend1()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
    menu->addAction("Move to bottom left", this, SLOT(moveLegend1()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
  } else  // general context menu on graphs requested
  {
   // menu->addAction("Add random graph", this, SLOT(addRandomGraph()));
    if (ui->Graph1->selectedGraphs().size() > 0)
      menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
    if (ui->Graph1->graphCount() > 0)
      menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs1()));
  }
  
  menu->popup(ui->Graph1->mapToGlobal(pos));
}

void MainWindow::contextMenuRequest2(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);
  
  if (ui->Graph2->legend->selectTest(pos, false) >= 0) // context menu on legend requested
  {
    menu->addAction("Move to top left", this, SLOT(moveLegend2()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    menu->addAction("Move to top center", this, SLOT(moveLegend2()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
    menu->addAction("Move to top right", this, SLOT(moveLegend2()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
    menu->addAction("Move to bottom right", this, SLOT(moveLegend2()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
    menu->addAction("Move to bottom left", this, SLOT(moveLegend2()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
  } else  // general context menu on graphs requested
  {
   // menu->addAction("Add random graph", this, SLOT(addRandomGraph()));
    if (ui->Graph2->selectedGraphs().size() > 0)
      menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
    if (ui->Graph2->graphCount() > 0)
      menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs2()));
  }
  
  menu->popup(ui->Graph2->mapToGlobal(pos));
}

void MainWindow::contextMenuRequest3(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);
  
  if (ui->Graph3->legend->selectTest(pos, false) >= 0) // context menu on legend requested
  {
    menu->addAction("Move to top left", this, SLOT(moveLegend3()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    menu->addAction("Move to top center", this, SLOT(moveLegend3()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
    menu->addAction("Move to top right", this, SLOT(moveLegend3()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
    menu->addAction("Move to bottom right", this, SLOT(moveLegend3()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
    menu->addAction("Move to bottom left", this, SLOT(moveLegend3()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
  } else  // general context menu on graphs requested
  {
  //  menu->addAction("Add random graph", this, SLOT(addRandomGraph()));
    if (ui->Graph3->selectedGraphs().size() > 0)
      menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
    if (ui->Graph3->graphCount() > 0)
      menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs3()));
  }
  
  menu->popup(ui->Graph3->mapToGlobal(pos));
}

void MainWindow::moveLegend1()
{
  if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
      ui->Graph1->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
      ui->Graph1->replot();
    }
  }
}

void MainWindow::moveLegend2()
{
  if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
      ui->Graph2->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
      ui->Graph2->replot();
    }
  }
}

void MainWindow::moveLegend3()
{
  if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
      ui->Graph3->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
      ui->Graph3->replot();
    }
  }
}

void MainWindow::graphClicked(QCPAbstractPlottable *plottable, int dataIndex)
{
  // since we know we only have QCPGraphs in the plot, we can immediately access interface1D()
  // usually it's better to first check whether interface1D() returns non-zero, and only then use it.
  double dataValue = plottable->interface1D()->dataMainValue(dataIndex);
  QString message = QString("Clicked on graph '%1' at data point #%2 with value %3.").arg(plottable->name()).arg(dataIndex).arg(dataValue);
  ui->statusBar->showMessage(message, 2500);
}


void MainWindow::toggleViewFrame1(bool state){ //toggles whether or not to show Frame/graph 1
	//qDebug() << "It got called and state is" << state;
	if (state == true) {
		viewFrame1 = true;
		this->ui->Frame1->show();
	}
	else {
		viewFrame1 = false;
		this->ui->Frame1->hide();
	}
	/*switch (state) {
		case 0: //unchecked
			viewFrame1 = false;
			this->ui->Frame1->hide();			
			break;
			
		case 2:	//checked
			viewFrame1 = true;
			this->ui->Frame1->show();
			break;
			
		default:
			break;
	} */
} 

void MainWindow::toggleViewFrame2(bool state){ //toggles whether or not to show Frame/graph 2
	//qDebug() << "It got called and state is" << state;
	if (state == true) {
		viewFrame2 = true;
		this->ui->Frame2->show();
	}
	else {
		viewFrame2 = false;
		this->ui->Frame2->hide();
	}
	/*switch (state) {
		case 0: //unchecked
			viewFrame2 = false;
			this->ui->Frame2->hide();			
			break;
			
		case 2:	//checked
			viewFrame2 = true;
			this->ui->Frame2->show();
			break;
			
		default:
			break;
	}  */
} 

void MainWindow::toggleViewFrame3(bool state){ //toggles whether or not to show Frame/graph 3
	//qDebug() << "It got called and state is" << state;
	if (state == true) {
		viewFrame3 = true;
		this->ui->Frame3->show();
	}
	else {
		viewFrame3 = false;
		this->ui->Frame3->hide();
	}
	/*switch (state) {
		case 0: //unchecked
			viewFrame3 = false;
			this->ui->Frame3->hide();			
			break;
			
		case 2:	//checked
			viewFrame3 = true;
			this->ui->Frame3->show();
			break;
			
		default:
			break;
	}  */
} 

bool MainWindow::getUSBData(){ //this will read from the "file", i.e. the stuff coming across the USB
	//should this function be called at a fixed interval to check for new data (and just record previous value otherwise), 
	//or should it only recieve data as it comes across?
	
	//read from the USB "file"
	//QFileInfo fi(usbFileName); 
	qDebug("getUSBData");
    //qDebug() << "Here's usbFileName:" << usbFileName;
    //QFile USB(usbFileName);//usbFileName gives the location of the USB thing
    bool cont = true;
    bool op = usbfp.is_open();
    if (!op) {
        qDebug("usb file is not open");
        return false;
    }
	else {qDebug("I opened the file");}
	
    std::string str;
    // get the next line from the file
    std::getline(usbfp,str);

    if (!usbfp.good()) {
        return false;
    }

    auto pos = str.find(",");   // read past the comma in the string
    std::string token = str.substr(0,pos);
    std::string rem = str.substr(pos+1);
    std::stringstream srem(rem);
	
	if (token == "Speed"){
        double Speed;
        srem >> Speed;              // assuming the units are MPH
        qDebug() << " Speed is " << Speed;
        //QVector<double> speedCoordinate(2);
        //speedCoordinate.append(speedCoordinates.length() - 1);
        //speedCoordinate.append(Speed);
        //speedCoordinates.append(speedCoordinate);
		speedCoordinates[0].append(speedCoordinates[0].length());
		//speedCoordinates.at(1).append(Speed);
		speedCoordinates[1].append(Speed);
		
    }
    if (token == "Watt Hours"){
        double WattHours;
        srem >> WattHours;              // kWh
        qDebug() << " Watt hours is " << WattHours;
        //QVector<double> energyCoordinate(2);
        //energyCoordinate.append(energyCoordinates.length() - 1);
        //energyCoordinate.append(WattHours);
        //energyCoordinates.append(energyCoordinate);
		//energyCoordinates.at(0).append(energyCoordinates.length() - 1);
		//energyCoordinates.at(1).append(WattHours);
		energyCoordinates[0].append(energyCoordinates[0].length());
		energyCoordinates[1].append(WattHours);
    }
    if (token == "Pack Current"){
        double PackCurrent;
        srem >> PackCurrent;            // Amps
        qDebug() << " PackCurrent is " << PackCurrent ;
        //QVector<double> currentCoordinate(2);
        //currentCoordinate.append(currentCoordinates.length() - 1);
        //currentCoordinate.append(PackCurrent); //this value could be negative, so make sure that's possible in the graph
        //currentCoordinates.append(currentCoordinate);
		currentCoordinates[0].append(currentCoordinates[0].length());
		currentCoordinates[1].append(PackCurrent);
		//currentCoordinates.at(0).append(currentCoordinates.length() - 1);
		//currentCoordinates.at(1).append(PackCurrent);
    }
    if (token == "Pack Instant Voltage"){
        double PackInstantVoltage;
        srem >> PackInstantVoltage;     // Volts
        qDebug() << " PackInstantVoltage is " << PackInstantVoltage;
        //QVector<double> voltageCoordinate(2);
        //voltageCoordinate.append(voltageCoordinates.length() - 1);
        //voltageCoordinate.append(PackInstantVoltage);
        //voltageCoordinates.append(voltageCoordinate);
		voltageCoordinates[0].append(voltageCoordinates[0].length());
		voltageCoordinates[1].append(PackInstantVoltage);
		//voltageCoordinates.at(0).append(voltageCoordinates.length() - 1);
		//voltageCoordinates.at(1).append(PackInstantVoltage);
    }
    if (token == "State Of Charge"){
        double StateOfCharge;
        srem >> StateOfCharge;          // %
        qDebug() << " StateOfCharge is " << StateOfCharge ;
        /*QVector<double> chargeCoordinate(2);
        chargeCoordinate.append(chargeCoordinates.length() - 1);
        chargeCoordinate.append(StateOfCharge);
        chargeCoordinates.append(chargeCoordinate);*/
		chargeCoordinates[0].append(chargeCoordinates[0].length());
		chargeCoordinates[1].append(StateOfCharge);
		//chargeCoordinates.at(0).append(chargeCoordinates.length() - 1);
		//chargeCoordinates.at(1).append(StateOfCharge);
    }
    if (token == "Relay Status"){
        double     RelayStatus;
        srem >> RelayStatus;
        qDebug()  << " RelayStatus is " << RelayStatus ;
        emit this->sig_Relay_Status(RelayStatus);
    }
    if (token == "Pack Amp Hours"){
        double PackAmpHours;
        srem >> PackAmpHours;           // Ah
        qDebug() << " PackAmpHours is " << PackAmpHours ;
        /*QVector<double> ampHourCoordinate(2);
        ampHourCoordinate.append(ampHourCoordinates.length() - 1);
        ampHourCoordinate.append(PackAmpHours);
        ampHourCoordinates.append(ampHourCoordinate); */
		ampHourCoordinates[0].append(ampHourCoordinates[0].length());
		ampHourCoordinates[1].append(PackAmpHours);
		//ampHourCoordinates.at(0).append(ampHourCoordinates.length() - 1);
		//ampHourCoordinates.at(1).append(PackAmpHours);
    }
    if (token == "Current Limit Status"){
        int     CurrentLimitStatus;
        srem >> CurrentLimitStatus;
        qDebug()  << " CurrentLimitStatus is " << CurrentLimitStatus ;
    }
    if (token == "High Temperature"){
        int     HighTemperature;        // C
        srem >> HighTemperature;        // C
        qDebug()  << " HighTemperature is " << HighTemperature ;
    }
    if (token == "Low Temperature"){
        int     LowTemperature;         // C
        srem >> LowTemperature;         // C
        qDebug()  << " LowTemperature is " << LowTemperature ;
    }

    qDebug("A0");
	//it's not opening the file for some reason...
    //if (USB.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //if (cont) {
    //    qDebug("A0.5");
        //QTextStream in(&USB);
		//in >>
    //	QStringList line;
		/*if(!in.atEnd()) {
			//qDebug("it's at the end for some reason");
		}*/
      //  qDebug("A1");


/*		while (!(in.atEnd())) {
			line = in.readLine().split(" ");
			qDebug("A2");			
			if (line.at(0) == "Watt Hours,") { //add a point to energyCoordinates
				QVector<double> energyCoordinate(2);
				energyCoordinate.append(energyCoordinates.length() - 1);
				energyCoordinate.append((line.at(1)).toDouble());
				energyCoordinates.append(energyCoordinate);
			}
			else if (line.at(0) == "Pack" && line.at(1) == "Current") { //add a point to currentCoordinates
				QVector<double> currentCoordinate(2);
				currentCoordinate.append(currentCoordinates.length() - 1);
				currentCoordinate.append((line.at(2)).toDouble()); //this value could be negative, so make sure that's possible in the graph
				currentCoordinates.append(currentCoordinate);
			}
			else if (line.at(0) == "Pack" && line.at(1) == "Instant" && line.at(2) == "Voltage") { //add a point to voltageCoordinates
				QVector<double> voltageCoordinate(2);
				voltageCoordinate.append(voltageCoordinates.length() - 1);
				voltageCoordinate.append((line.at(3)).toDouble()); 
				voltageCoordinates.append(voltageCoordinate);
			}
			else if (line.at(0) == "State" && line.at(1) == "of" && line.at(2) == "Charge") { //add a point to chargeCoordinates
				QVector<double> chargeCoordinate(2);
				chargeCoordinate.append(chargeCoordinates.length() - 1);
				chargeCoordinate.append((line.at(3)).toDouble()); 
				chargeCoordinates.append(chargeCoordinate);
			}
			else if (line.at(0) == "Pack_Amp_Hours") { 
				QVector<double> ampHourCoordinate(2);
				ampHourCoordinate.append(ampHourCoordinates.length() - 1);
				ampHourCoordinate.append((line.at(2)).toDouble()); 
				ampHourCoordinates.append(ampHourCoordinate);
			}
			
			//Messages
			else if (line.at(0) == "DCL_Low_SOC") { //DCL_Low_SOC message
				if (line.at(2) == "0") {
					this->DCL_Low_SOC = false;					
					emit this->sig_DCL_Low_SOC(0);
				}
				else {
					this->DCL_Low_SOC = true;										
					emit this->sig_DCL_Low_SOC(1);
				}
			}
			else if (line.at(0) == "DCL_High_Cell_Resistance") { 
				if (line.at(2) == "0") {
					this->DCL_High_Cell_Resistance = false;					
					emit this->sig_DCL_High_Cell_Resistance(0);
				}
				else {
					emit this->sig_DCL_High_Cell_Resistance(1);					
					this->DCL_High_Cell_Resistance = true;					
				}
			}
			else if (line.at(0) == "DCL_Temperature") { 
				if (line.at(2) == "0") {
					this->DCL_Temperature = false; //this probably isn't applicable					
					emit this->sig_DCL_Temperature(0);
				}
				else {
					this->DCL_Temperature = true; //this probably isn't applicable										
					emit this->sig_DCL_Temperature(1);
				}
			}
			else if (line.at(0) == "DCL_Low_Cell_Voltage") { 
				if (line.at(2) == "0") {
					emit this->sig_DCL_Low_Cell_Voltage(0);
					this->DCL_Low_Cell_Voltage = false;
				}
				else {
					emit this->sig_DCL_Low_Cell_Voltage(1);
					this->DCL_Low_Cell_Voltage = true;					
				}
			}
			else if (line.at(0) == "DCL_Low_Pack_Voltage") { 
				if (line.at(2) == "0") {
					this->DCL_Low_Pack_Voltage = false;										
					emit this->sig_DCL_Low_Pack_Voltage(0);
				}
				else {
					this->DCL_Low_Pack_Voltage = true;															
					emit this->sig_DCL_Low_Pack_Voltage(1);
				}
			}
			else if (line.at(0) == "DCL_Voltage_Failsafe") { 
				if (line.at(2) == "0") {
					this->DCL_Voltage_Failsafe = false;
					emit this->sig_DCL_Voltage_Failsafe(0);
				}
				else {
					this->DCL_Voltage_Failsafe = true;					
					emit this->sig_DCL_Voltage_Failsafe(1);
				}
			}
			else if (line.at(0) == "CCL_High_SOC") { 
				if (line.at(2) == "0") {
					this->CCL_High_SOC = false;					
					emit this->sig_CCL_High_SOC(0);
				}
				else {
					this->CCL_High_SOC = true;										
					emit this->sig_CCL_High_SOC(1);
				}
			}
			else if (line.at(0) == "CCL_High_Cell_Resistance") { 
				if (line.at(2) == "0") {
					this->CCL_High_Cell_Resistance = false;
					emit this->sig_CCL_High_Cell_Resistance(0);
				}
				else {
					this->CCL_High_Cell_Resistance = true;					
					emit this->sig_CCL_High_Cell_Resistance(1);
				}
			}
			else if (line.at(0) == "CCL_Temperature") { 
				if (line.at(2) == "0") {
					this->CCL_Temperature = false;					
					emit this->sig_CCL_Temperature(0);
				}
				else {
					this->CCL_Temperature = true;										
					emit this->sig_CCL_Temperature(1);
				}
			}
			else if (line.at(0) == "CCL_High_Cell_Voltage") { 
				if (line.at(2) == "0") {
					this->CCL_High_Cell_Voltage = false;															
					emit this->sig_CCL_High_Cell_Voltage(0);
				}
				else {
					this->CCL_High_Cell_Voltage = true;																				
					emit this->sig_CCL_High_Cell_Voltage(1);
				}
			}
			else if (line.at(0) == "CCL_High_Pack_Voltage") { 
				if (line.at(2) == "0") {
					this->CCL_High_Pack_Voltage = false;																				
					emit this->sig_CCL_High_Pack_Voltage(0);
				}
				else {
					this->CCL_High_Pack_Voltage = true;																									
					emit this->sig_CCL_High_Pack_Voltage(1);
				}
			}
			else if (line.at(0) == "CCL_Charger_Latch") { 
				if (line.at(2) == "0") {
					this->CCL_Charger_Latch = false;																									
					emit this->sig_CCL_Charger_Latch(0);
				}
				else {
					this->CCL_Charger_Latch = true;																														
					emit this->sig_CCL_Charger_Latch(1);
				}
			}
			else if (line.at(0) == "discharge_relay_disabled") { 
				if (line.at(2) == "0") {
					this->discharge_relay_disabled = false;																														
					emit this->sig_discharge_relay_disabled(0);
				}
				else {
					this->discharge_relay_disabled = true;																																			
					emit this->sig_discharge_relay_disabled(1);
				}
			}
			else if (line.at(0) == "charge_relay_disabled") { 
				if (line.at(2) == "0") {
					this->charge_relay_disabled = false;																																			
					emit this->sig_charge_relay_disabled(0);
				}
				else {
					this->charge_relay_disabled = true;																																			
					emit this->sig_charge_relay_disabled(1);
				}
			}
			else if (line.at(0) == "charger_safety_disabled") { 
				if (line.at(2) == "0") {
					this->charger_safety_disabled = false;																																								
					emit this->sig_charger_safety_disabled(0);
				}
				else {
					this->charger_safety_disabled = true;																																													
					emit this->sig_charger_safety_disabled(1);
				}
			}
			else if (line.at(0) == "diagnostic_trouble_code_active") { 
				if (line.at(2) == "0") {
					this->diagnostic_trouble_code_active = false;																																													
					emit this->sig_diagnostic_trouble_code_active(0);
				}
				else {
					this->diagnostic_trouble_code_active = true;																																																		
					emit this->sig_diagnostic_trouble_code_active(1);
				}
			}
			else if (line.at(0) == "always_on_power_status") { 
				if (line.at(2) == "0") {
					this->always_on_power_status = false;																																																		
					emit this->sig_always_on_power_status(0);
				}
				else {
					this->always_on_power_status = true;																																																							
					emit this->sig_always_on_power_status(1);
				}
			}
			else if (line.at(0) == "is_ready_power_status") { 
				if (line.at(2) == "0") {
					this->is_ready_power_status = false;																																																		
					emit this->sig_is_ready_power_status(0);
				}
				else {
					this->is_ready_power_status = true;																																																							
					emit this->sig_is_ready_power_status(1);
				}
			}
			else if (line.at(0) == "is_charging_power_status") { 
				if (line.at(2) == "0") {
					this->is_charging_power_status = false;																																																							
					emit this->sig_is_charging_power_status(0);
				}
				else {
					this->is_charging_power_status = true;																																																												
					emit this->sig_is_charging_power_status(1);
				}
			}
			else if (line.at(0) == "High_Temperature") { 
				if (line.at(2) == "0") {
					this->High_Temperature = false;																																																												
					emit this->sig_High_Temperature(0);
				}
				else {
					this->High_Temperature = true;																																																																	
					emit this->sig_High_Temperature(1);
				}
			}
			else if (line.at(0) == "Low_Temperature") { 
				if (line.at(2) == "0") {
					this->Low_Temperature = false;																																																																	
					emit this->sig_Low_Temperature(0);
				}
				else {
					this->Low_Temperature = true;																																																																						
					emit this->sig_Low_Temperature(1);
				}
			}
			else if (line.at(0) == "Relay_Status") { 
				emit this->sig_Relay_Status((line.at(2)).toDouble());
			}
			qDebug("A3");			
        } */
    //}
	//add points to speedCoordinates, batteryCoordinates, etc.
	qDebug("nope");	
} 

void MainWindow::getData() { //gets data and adds points to the coordinate vectors
	//this should get called whenever there's new data, or maybe just at a fixed time interval
	//gonna need to use the CAN thing
	//might need to remove the parameter, not sure
	//setData(newPoint);
	qDebug("I got to getData");
    getUSBData();
	index++;
	qDebug() << "length of numberVectors is" << numberVectors.length();
	for (int i = 0; i < numberVectors.length(); i++) {
		qDebug() << "i =" << i;
		if (numberVectors[i] == 0) { //if there have been no values transmitted yet
			qDebug() << "numberVectors[" << i <<"] =" << numberVectors[i];
			setZero(i); //initialize the variable to 0
		}
		
		if(numberVectors[i] < index) {
			qDebug() << "numberVectors[" << i <<"] =" << numberVectors[i];			
			addRepeatValue(i); //the variable will hold the previous value for the current frame
		} 
	}
	//add the coordinates to the graph data
	qDebug("also nope");
	//maybe it's not working because the data's being set too late so it's trying to access data that hasn't been written yet 
	//(and that's why it's saying index out of bounds)
	qDebug() << "here is the length of speedCoordinates:" << speedCoordinates.length();
	if(speedCoordinates.length() == 2) {
	//if(speedCoordinates.length() >= (index + 1)) {
        this->ui->Graph1->graph(0)->setData(speedCoordinates.at(0), speedCoordinates.at(1));
	}
	qDebug("A");
	if(chargeCoordinates.length() == 2) {	
		this->ui->Graph2->graph(0)->setData(chargeCoordinates.at(0), chargeCoordinates.at(1));
	}
	if(powerCoordinates.length() == 2) {
		this->ui->Graph3->graph(0)->setData(powerCoordinates.at(0), powerCoordinates.at(1));
	}
	if(energyCoordinates.length() == 2) {
		this->ui->Graph4->graph(0)->setData(energyCoordinates.at(0), energyCoordinates.at(1));
	}
	if(currentCoordinates.length() == 2) {
		this->ui->Graph5->graph(0)->setData(currentCoordinates.at(0), currentCoordinates.at(1));
	}
	if(voltageCoordinates.length() == 2) {
		this->ui->Graph6->graph(0)->setData(voltageCoordinates.at(0), voltageCoordinates.at(1));
	}
	if(ampHourCoordinates.length() == 2) {
		this->ui->Graph7->graph(0)->setData(ampHourCoordinates.at(0), ampHourCoordinates.at(1));
	}
	if(temperatureCoordinates.length() == 2) {
		this->ui->Graph8->graph(0)->setData(temperatureCoordinates.at(0), temperatureCoordinates.at(1));
	}
	
	/*for (int i = 0; i < 101; ++i) {
		const double d = i/50.0 - 1;
		//this stuff isn't working and I don't know why, it's really annoying
		speedCoordinates[0].append(d); // x goes from -1 to 1       
		speedCoordinates[1].append(speedCoordinates[0][i] * speedCoordinates[0][i]); // let's plot a quadratic function
	} 
	
	this->ui->Graph1->graph(0)->setData(speedCoordinates.at(0),speedCoordinates.at(1)); */
	
	
	if (!(speedCoordinates.at(0).empty())) {
		qDebug("speedCoordinates is not empty");
		plotData();
	}
	qDebug("B");
	
}

void MainWindow::setZero(int var) {
	qDebug() << "setZero(" <<var<<")";
	const QVector<double> q;
	
	switch(var) {
		case 0: //speed
			speedCoordinates[0].append(0);
			
			
			speedCoordinates.append(q);
			
			qDebug() << "speedCoordinates has length" << speedCoordinates.length();
			
			speedCoordinates[1].append(0);
			qDebug() << "speedCoordinates[1] has length" << speedCoordinates[1].length();
			
			break;
		case 1: //charge
			chargeCoordinates[0].append(0);
			chargeCoordinates.append(q);
			qDebug() << "chargeCoordinates[0] has length" << chargeCoordinates[0].length();
			qDebug() << "chargeCoordinates has length" << chargeCoordinates.length();
			
			chargeCoordinates[1].append(0);
			break;
		case 2: //power
			powerCoordinates[0].append(0);
			powerCoordinates.append(q);
			
			powerCoordinates[1].append(0);
			break;
		case 3: //energy
			qDebug("blah");
			qDebug() << "energyCoordinates has length" << energyCoordinates.length();						
			qDebug() << "energyCoordinates[0] has length" << energyCoordinates[0].length();
			qDebug() << "energyCoordinates[0] has length" << energyCoordinates[0].length();
			
			energyCoordinates[0].append(0);
			energyCoordinates.append(q);
			qDebug() << "energyCoordinates has length" << energyCoordinates.length();			
			energyCoordinates[1].append(0);
			break;
		case 4: //current
			currentCoordinates[0].append(0);
			currentCoordinates.append(q);
			
			currentCoordinates[1].append(0);
			break;
		case 5: //voltage
			voltageCoordinates[0].append(0);
			voltageCoordinates.append(q);
			
			voltageCoordinates[1].append(0);
			break;
		case 6: //ampHours
			ampHourCoordinates[0].append(0);
			ampHourCoordinates.append(q);
			
			ampHourCoordinates[1].append(0);
			break;
		case 7: //temperature
			temperatureCoordinates[0].append(0);
			temperatureCoordinates.append(q);
			
			temperatureCoordinates[1].append(0);
			break;
	}
}

void MainWindow::addRepeatValue(int var) {
	//see key in MainWindow.h for variables
	switch (var) { 
		case 0: //add another speed coordinate with the same value as the previous frame
			speedCoordinates[0].append(speedCoordinates[0].length());
			speedCoordinates[1].append(speedCoordinates[1].at(speedCoordinates[1].length()-1));
			break;
		case 1: //charge
			chargeCoordinates[0].append(chargeCoordinates[0].length());
			chargeCoordinates[1].append(chargeCoordinates[1].at(chargeCoordinates[1].length()-1));
			break;
		case 2: //power
			powerCoordinates[0].append(powerCoordinates[0].length());
			powerCoordinates[1].append(powerCoordinates[1].at(powerCoordinates[1].length()-1));
			break;
		case 3: //energy
			energyCoordinates[0].append(energyCoordinates[0].length());
			energyCoordinates[1].append(energyCoordinates[1].at(energyCoordinates[1].length()-1));
			break;
		case 4: //current
			currentCoordinates[0].append(currentCoordinates[0].length());
			currentCoordinates[1].append(currentCoordinates[1].at(currentCoordinates[1].length()-1));
			break;
		case 5: //voltage
			voltageCoordinates[0].append(voltageCoordinates[0].length());
			voltageCoordinates[1].append(voltageCoordinates[1].at(voltageCoordinates[1].length()-1));
			break;
		case 6: //ampHours
			ampHourCoordinates[0].append(ampHourCoordinates[0].length());
			ampHourCoordinates[1].append(ampHourCoordinates[1].at(ampHourCoordinates[1].length()-1));
			break;
		case 7: //temperature
			temperatureCoordinates[0].append(temperatureCoordinates[0].length());
			temperatureCoordinates[1].append(temperatureCoordinates[1].at(temperatureCoordinates[1].length()-1));
			break;
	}
}

/*void MainWindow::setData(QPoint thePoint) {
	//add data to be plotted
	QVector<double> x(101), y(101);
	for (int i = 0; i < 101; ++i) {
		x[i] = i/50.0 - 1; // x goes from -1 to 1
		  y[i] = x[i]*x[i]; // let's plot a quadratic function
	}
}*/

void MainWindow::plotData() {
	//this should plot the new point(s?) on the graph 
	//Update data in graph by calling this->ui->Graph1->setData(xval, yval) and then customPlot()->replot() (allegedly)
	//this->ui->Graph1->setData
	
	qDebug() << "I got to plotData";
	//check to make sure the coordinate things aren't empty
	if (!(speedCoordinates.at(0).empty()) && !(speedCoordinates.at(1).empty())) { //graph 1 = speed
		this->ui->Graph1->replot();
	}
	if (!(chargeCoordinates.at(0).empty()) && !(chargeCoordinates.at(1).empty())) { //graph 2 = charge
		this->ui->Graph2->replot();
	}
	if (!(powerCoordinates.at(0).empty()) && !(powerCoordinates.at(1).empty())) { //graph 3 = power
		this->ui->Graph3->replot();
	}
	if (!(energyCoordinates.at(0).empty()) && !(energyCoordinates.at(1).empty())) { //graph 4 = energy
		this->ui->Graph4->replot();
	}
	if (!(currentCoordinates.at(0).empty()) && !(currentCoordinates.at(1).empty())) { //graph 5 = current
		this->ui->Graph5->replot();
	}
	if (!(voltageCoordinates.at(0).empty()) && !(voltageCoordinates.at(1).empty())) { //graph 6 = voltage
		this->ui->Graph6->replot();
	}
	if (!(ampHourCoordinates.at(0).empty()) && !(ampHourCoordinates.at(1).empty())) { //graph 7 = amp hours
		this->ui->Graph7->replot();
	}
	if (!(temperatureCoordinates.at(0).empty()) && !(temperatureCoordinates.at(1).empty())) { //graph 8 = temperature
		this->ui->Graph8->replot();
	}
	qDebug() << "B";
}

void MainWindow::updateMessages(int message, int value) {
	switch(message) {
		case 1: //DCL_Low_SOC
			if (this->DCL_Low_SOC && value == 0) { //if the message was already showing and now must go away
				//eliminate the message from the QTextBrowser
			}
			else if (!this->DCL_Low_SOC && value == 1) {//if the message wasn't showing before but needs to be
			//add the message to the QTextBrowser
				this->ui->messagesPane->append("<br>-Low S.O.C.");
			}
			break;
		case 2:  //DCL_High_Cell_Resistance
			if (this->DCL_High_Cell_Resistance && value == 0) { //if the message was already showing and now must go away
				//eliminate the message from the QTextBrowser
			}
			else if (!this->DCL_High_Cell_Resistance && value == 1) {//if the message wasn't showing before but needs to be
			//add the message to the QTextBrowser
				this->ui->messagesPane->append("<br>-High Cell Resistance");
			}
			break;
		case 3:  //DCL_Temperature
			if (this->DCL_Temperature && value == 0) { //if the message was already showing and now must go away
				//eliminate the message from the QTextBrowser
			}
			else if (!this->DCL_Temperature && value == 1) {//if the message wasn't showing before but needs to be
			//add the message to the QTextBrowser
				this->ui->messagesPane->append("<br>-Temperature is a thing?");				
			}
			break;
		case 4:  //DCL_Low_Cell_Voltage
			if (this->DCL_Low_Cell_Voltage && value == 0) { //if the message was already showing and now must go away
				//eliminate the message from the QTextBrowser
			}
			else if (!this->DCL_Low_Cell_Voltage && value == 1) {//if the message wasn't showing before but needs to be
			//add the message to the QTextBrowser
			}
			break;
		case 5:  //DCL_Low_Pack_Voltage
			if (this->DCL_Low_Pack_Voltage && value == 0) { //if the message was already showing and now must go away
				//eliminate the message from the QTextBrowser
			}
			else if (!this->DCL_Low_Pack_Voltage && value == 1) {//if the message wasn't showing before but needs to be
			//add the message to the QTextBrowser
			}
			break;
		case 6:  //DCL_Voltage_Failsafe
			if (this->DCL_Voltage_Failsafe && value == 0) { //if the message was already showing and now must go away
				//eliminate the message from the QTextBrowser
			}
			else if (!this->DCL_Voltage_Failsafe && value == 1) {//if the message wasn't showing before but needs to be
			//add the message to the QTextBrowser
			}
			break;
		case 7:  //CCL_High_SOC
			if (this->CCL_High_SOC && value == 0) { //if the message was already showing and now must go away
				//eliminate the message from the QTextBrowser
			}
			else if (!this->CCL_High_SOC && value == 1) {//if the message wasn't showing before but needs to be
			//add the message to the QTextBrowser
			}
			break;
		case 8:  //CCL_High_Cell_Resistance
			if (this->CCL_High_Cell_Resistance && value == 0) { //if the message was already showing and now must go away
				//eliminate the message from the QTextBrowser
			}
			else if (!this->CCL_High_Cell_Resistance && value == 1) {//if the message wasn't showing before but needs to be
			//add the message to the QTextBrowser
			}
			break;
		case 9:  //CCL_Temperature
			if (this->CCL_Temperature && value == 0) { //if the message was already showing and now must go away
				//eliminate the message from the QTextBrowser
			}
			else if (!this->CCL_Temperature && value == 1) {//if the message wasn't showing before but needs to be
			//add the message to the QTextBrowser
			}
			break;
		case 10: //CCL_High_Cell_Voltage
			if (this->CCL_High_Cell_Voltage && value == 0) { //if the message was already showing and now must go away
				//eliminate the message from the QTextBrowser
			}
			else if (!this->CCL_High_Cell_Voltage && value == 1) {//if the message wasn't showing before but needs to be
			//add the message to the QTextBrowser
			}
			break;
		case 11: //CCL_High_Pack_Voltage
			if (this->CCL_High_Pack_Voltage && value == 0) { //if the message was already showing and now must go away
				//eliminate the message from the QTextBrowser
			}
			else if (!this->CCL_High_Pack_Voltage && value == 1) {//if the message wasn't showing before but needs to be
			//add the message to the QTextBrowser
			}
			break;
		case 12: //CCL_Charger_Latch
			if (this->CCL_Charger_Latch && value == 0) { //if the message was already showing and now must go away
				//eliminate the message from the QTextBrowser
			}
			else if (!this->CCL_Charger_Latch && value == 1) {//if the message wasn't showing before but needs to be
			//add the message to the QTextBrowser
			}
			break;
		case 13: //discharge_relay_disabled
			if (this->discharge_relay_disabled && value == 0) { //if the message was already showing and now must go away
				//eliminate the message from the QTextBrowser
			}
			else if (!this->discharge_relay_disabled && value == 1) {//if the message wasn't showing before but needs to be
			//add the message to the QTextBrowser
			}
			break;
		case 14: //charge_relay_disabled
			if (this->charge_relay_disabled && value == 0) { //if the message was already showing and now must go away
				//eliminate the message from the QTextBrowser
			}
			else if (!this->charge_relay_disabled && value == 1) {//if the message wasn't showing before but needs to be
			//add the message to the QTextBrowser
			}
			break;
		case 15: //charger_safety_disabled
			if (this->charger_safety_disabled && value == 0) { //if the message was already showing and now must go away
				//eliminate the message from the QTextBrowser
			}
			else if (!this->charger_safety_disabled && value == 1) {//if the message wasn't showing before but needs to be
			//add the message to the QTextBrowser
			}
			break;
		case 16: //diagnostic_trouble_code_active
			if (this->diagnostic_trouble_code_active && value == 0) { //if the message was already showing and now must go away
				//eliminate the message from the QTextBrowser
			}
			else if (!this->diagnostic_trouble_code_active && value == 1) {//if the message wasn't showing before but needs to be
			//add the message to the QTextBrowser
			}
			break;
		case 17: //always_on_power_status
			if (this->always_on_power_status && value == 0) { //if the message was already showing and now must go away
				//eliminate the message from the QTextBrowser
			}
			else if (!this->always_on_power_status && value == 1) {//if the message wasn't showing before but needs to be
			//add the message to the QTextBrowser
			}
			break;
		case 18: //is_ready_power_status
			if (this->is_ready_power_status && value == 0) { //if the message was already showing and now must go away
				//eliminate the message from the QTextBrowser
			}
			else if (!this->is_ready_power_status && value == 1) {//if the message wasn't showing before but needs to be
			//add the message to the QTextBrowser
			}
			break;
		case 19: //is_charging_power_status
			if (this->is_charging_power_status && value == 0) { //if the message was already showing and now must go away
				//eliminate the message from the QTextBrowser
			}
			else if (!this->is_charging_power_status && value == 1) {//if the message wasn't showing before but needs to be
			//add the message to the QTextBrowser
			}
			break;
		case 20: //High_Temperature
			if (this->High_Temperature && value == 0) { //if the message was already showing and now must go away
				//eliminate the message from the QTextBrowser
			}
			else if (!this->High_Temperature && value == 1) {//if the message wasn't showing before but needs to be
			//add the message to the QTextBrowser
			}
			break;
		case 21: //Low_Temperature
			if (this->Low_Temperature && value == 0) { //if the message was already showing and now must go away
				//eliminate the message from the QTextBrowser
			}
			else if (!this->Low_Temperature && value == 1) {//if the message wasn't showing before but needs to be
			//add the message to the QTextBrowser
			}
			break;
		case 22: //Relay_Status
			/*if (this->Relay_Status && value == 0) { //if the message was already showing and now must go away
				//eliminate the message from the QTextBrowser
			}
			else if (!this->Relay_Status && value == 1) {//if the message wasn't showing before but needs to be
			//add the message to the QTextBrowser
			}*/
			//do something with the relay status...?
			break;
		default:
			QMessageBox::warning(
						this, 
						"Error", 
						"There was an error displaying a message.",
						QMessageBox::Ok);
	}
}

void MainWindow::lol(bool unused) {
	QMessageBox::warning(
				this, 
				"Ruh-Roh!", 
				"Sorry kiddo, you know as much as I do.",
				QMessageBox::Ok);	
}
