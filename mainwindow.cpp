#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include <QDebug>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	
	//Graphs menu
	this->connect(this->ui->viewGraph1_checkbox, SIGNAL(toggled(bool)), this, SLOT(toggleViewFrame1(bool)));	
	this->connect(this->ui->viewGraph2_checkbox, SIGNAL(toggled(bool)), this, SLOT(toggleViewFrame2(bool)));
	this->connect(this->ui->viewGraph3_checkbox, SIGNAL(toggled(bool)), this, SLOT(toggleViewFrame3(bool)));
	
	//Help menu
	this->connect(this->ui->actionHelp_me, SIGNAL(triggered(bool)), this, SLOT(lol(bool)));
	
	this->viewFrame1 = true;
	this->viewFrame2 = true;
	
	this->ui->Frame1->show();
	this->ui->Frame2->show();
	

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
  
  ui->Graph1->xAxis->setLabel("x Axis");
  ui->Graph1->yAxis->setLabel("y Axis");
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
  QCPTextElement *graph2Title = new QCPTextElement(ui->Graph2, "Battery Level", QFont("sans", 17, QFont::Bold));
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
  QCPTextElement *graph3Title = new QCPTextElement(ui->Graph3, "Power?", QFont("sans", 17, QFont::Bold));
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
  
  //make a call to a function that will re-plot the graphs every 0.1 seconds
  startPlotting(10);
  
  //initialize the coordinate vectors
  
  //speedCoordinates = new QVector<QVector<double>>; //contains (references to) the full history of speed coordinates
	QVector<double> emptyVectorA; //not sure how to initialize this without putting in any values that don't belong in there
	speedCoordinates.append(emptyVectorA); //I'm afraid to reuse the same vector every time because vectors are weird about pointers
  //batteryCoordinates = new QVector<QVector<double>>; //contains (references to) the full history of battery level coordinates
	QVector<double> emptyVectorB; 
	batteryCoordinates.append(emptyVectorB);
 // powerCoordinates = new QVector<QVector<double>>; //contains (references to) the full history of power coordinates
  QVector<double> emptyVectorC; 
	powerCoordinates.append(emptyVectorC);
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


void MainWindow::getData() { //gets data and adds points to the coordinate vectors
	//this should get called whenever there's new data, or maybe just at a fixed time interval
	//gonna need to use the CAN thing
	//might need to remove the parameter, not sure
	//setData(newPoint);
	qDebug("I got to getData");
	
	//add the coordinates to the graph data
	this->ui->Graph1->graph(0)->setData(speedCoordinates.at(0), speedCoordinates.at(1));
	
	this->ui->Graph2->graph(0)->setData(batteryCoordinates.at(0), batteryCoordinates.at(1));

	this->ui->Graph3->graph(0)->setData(powerCoordinates.at(0), powerCoordinates.at(1));
	
	
	/*for (int i = 0; i < 101; ++i) {
		const double d = i/50.0 - 1;
		//this stuff isn't working and I don't know why, it's really annoying
		speedCoordinates[0].append(d); // x goes from -1 to 1       
		speedCoordinates[1].append(speedCoordinates[0][i] * speedCoordinates[0][i]); // let's plot a quadratic function
	} 
	
	this->ui->Graph1->graph(0)->setData(speedCoordinates.at(0),speedCoordinates.at(1)); */
	
	
	if (!(speedCoordinates.at(0).empty())) {
		plotData();
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
	if (!(speedCoordinates.at(0).empty()) && !(speedCoordinates.at(1).empty())) {
		this->ui->Graph1->replot();
	}
	if (!(batteryCoordinates.at(0).empty()) && !(batteryCoordinates.at(1).empty())) {
		this->ui->Graph2->replot();
	}
	if (!(powerCoordinates.at(0).empty()) && !(powerCoordinates.at(1).empty())) {
		this->ui->Graph3->replot();
	}
	qDebug() << "B";
}

void MainWindow::lol(bool unused) {
	QMessageBox::warning(
				this, 
				"Ruh-Roh!", 
				"Sorry kiddo, you know as much as I do.",
				QMessageBox::Ok);	
}
