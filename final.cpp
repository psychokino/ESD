#include "widget.h"
#include <QApplication>
#include <QPushButton>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PainterWidget *widget=new PainterWidget();
    widget->setGeometry(0,0,750,475);//800*480

    widget->setWindowTitle("Demo");


    widget->show();
	//cout<<"123"<<endl;
    return a.exec();
}
