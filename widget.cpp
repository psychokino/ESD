
#include "widget.h"
#include <QtGui/QApplication>
#include <QtGui/QPushButton>
#include <QtGui/QFont>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QImage>
#include <QPixmap>  
#include <QPaintEvent>  
#include <QPainter>
#include <QtGui>
#include <QTimer>
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <fstream>
#include <QThread>
#include <cstdlib>
#include <time.h>
#include<semaphore.h>
//#include <QMediaPlayer>
using namespace cv;
using namespace std;
#define FramePerSecond 20.0
#define NoteStep 20
#define Step 7

#define bias 175
sem_t signal;


void random_x_y(int& ranx, int& rany, int& area)
{
	
	int a = rand() % 8 ;
	switch( a )
	{
		case 0 : ranx = Step ; rany = Step ; area = 0 ; break ;
		case 1 : ranx = Step ; rany = 0    ; area = 1 ; break ;
		case 2 : ranx = Step ; rany = -Step; area = 2 ; break ;
		case 3 : ranx = 0 ; rany = -Step ;   area = 3 ; break ;
		case 4 : ranx = -Step ; rany = -Step;area = 4 ; break ;
		case 5 : ranx = -Step ; rany = 0 ;   area = 5 ; break ;
		case 6 : ranx = -Step ; rany = Step ;area = 6 ; break ;
		case 7 : ranx = 0 ; rany = Step ;    area = 7 ; break ;
		default : ranx = 0 ; rany = 0 ; area = -1 ; 
	}
}

PainterWidget::PainterWidget()
{
	//cvNamedWindow("window",0);
	//cvResizeWindow("window",1500,800);
	//cvShowImage("window",img);
	//cvWaitKey(0);
	ROI = cvRect(0, 0 , 0 , 0);
	ROI_open = false ; 
	//sem_init(&signal,1,0);
	
	FPS = 1000.0 / FramePerSecond ; 
	srand(time(NULL) );
	sheet.open("musicsheet.txt");
	if(!sheet.is_open())exit(1);
	sheet>>music_tempo ;
	int xxx;
	char input;
	while( sheet.get(input) )
	{
		
		
		
		if( (input == '0') || (input == '1') )
		{
			//cout<<static_cast<int>(input)-48<<endl;
			tempo_sheet.push_back(static_cast<int>(input)-48);
		}
	}
	cout<<tempo_sheet.size()<<endl;
	//for(int i = 0 ; i < tempo_sheet.size() ; i++)cout<<tempo_sheet[i];
	sheet_iterator = 0;
	mode = 0;
	/*
	QPushButton button("Animated Button");
	button.show();

	QPropertyAnimation animation(&button, "geometry");
	animation.setDuration(10000);
	animation.setStartValue(QRect(0, 0, 100, 30));
	animation.setEndValue(QRect(250, 250, 100, 30));

	animation.start();*/
////******************////
////  button setting  ////
////******************////

//// detect on  button // 750 * 475
	btn_detect=new QPushButton(this);
    btn_detect->setText("detect on");
	btn_detect->setFont(QFont("Courier", 18, QFont::Bold));
	btn_detect->setGeometry(315,425,120,20);
	QObject::connect(btn_detect, SIGNAL(clicked()), this, SLOT(detect()));
//// start game 
	btn_start=new QPushButton(this);
    btn_start->setText("start game");
	btn_start->setFont(QFont("Courier", 18, QFont::Bold));
	btn_start->setGeometry(315,400,120,20);
	QObject::connect(btn_start, SIGNAL(clicked()), this, SLOT(game_start()));
//// exit 
	btn_exit=new QPushButton(this);
    btn_exit->setText("exit");
	btn_exit->setFont(QFont("Courier", 18, QFont::Bold));
	btn_exit->setGeometry(315,450,120,20);
	QObject::connect(btn_exit, SIGNAL(clicked()), this, SLOT(game_exit()));
////*******************////
//// music player code ////
////*******************////
		/*Phonon::MediaObject *music =
        Phonon::createPlayer(Phonon::MusicCategory,
                             Phonon::MediaSource("Dream - Malody Theme Song.mp3"));
		music->play();*/
	
////	
	
	is_detect=0;
	
	stoptime = new QTimer(this);
	connect(stoptime, SIGNAL(timeout()),this, SLOT( game_exit() ));
	stoptime->setSingleShot(true);
	
	tempo_timer = new QTimer(this);
	connect(tempo_timer, SIGNAL(timeout()), SLOT(musicnote_load()));
	double tempo_speed = 60.0 / music_tempo *1000 / 2 ;
	tempo_timer->start( tempo_speed );
	cout<<"test constructor tempo timer id = "<<tempo_timer->timerId()<<endl;
	cout<< tempo_speed <<endl;
	pic_timer = new QTimer(this);
	
	
	/*
	if( !( pCapture = cvCaptureFromCAM(0) ) ) cout<<"Cannot open webcam"<<endl;
	cvSetCaptureProperty( pCapture, CV_CAP_PROP_FRAME_WIDTH,160);//640
	cvSetCaptureProperty( pCapture, CV_CAP_PROP_FRAME_HEIGHT,192);//480
	pImage = cvQueryFrame( pCapture );*/
	pImage = cvLoadImage("DSC_0716.JPG",1);
	small = cvCreateImage(cvSize(400,480) , pImage->depth , pImage->nChannels);
	
	cascade =(CvHaarClassifierCascade*)cvLoad("haarcascade_frontalface_alt.xml",0,0,0); 
	storage =cvCreateMemStorage(0);
	
	//picture = new QRect(this);
	//picture1 = new QRect( 501, 284 ,22,33); //原本的圖片方格
	//picture2 = new QRect( 501, 284 ,22,33);
	countdown = new QLabel(this);

	//picture->setText("moement");
	//picture->setFont(QFont("Courier", 20, QFont::Bold));
	//picture->setGeometry(650,150,110,50);
	
    
	
	//cout<<"123";
	//catImg = new QPixmap("handsome.jpg");
	QPixmap *temp;
	temp = new QPixmap("doggg.jpg");
	catImg.push_back(temp);
	//int width = catImg[0]->width ();
	//int height = catImg[0]->height ();
	width = 364;//750/2 - 22 / 2;
	height = 222;//475/2 - 33/2;
	pic_w = 22;
	pic_h = 33;
	x_pos = width + NoteStep * Step ;
	x_neg = width - NoteStep * Step;
	x_ori = width ;
	y_pos = height + NoteStep * Step;
	y_neg = height - NoteStep * Step;
	y_ori = height ;
	//picture_count = 0;
	
	count_to_zero = 3 ;
	
	
	/*QPixmap temp;
	temp.load("handsome.jpg");
	catImg.push_back(temp);
	int width = catImg[0].width();
	int height = catImg[0].height();*/
	//cout<<"test"<<endl;
	
	
	int_score = 0;
	score = new QLabel(this);
	score->setText("0");
	score->setFont(QFont("Courier", 64, QFont::Bold));
	score->setGeometry(10 ,10 ,140 ,100 );
}
PainterWidget::~PainterWidget()
{
	//cvReleaseCapture(&pCapture);
}

void PainterWidget::ROI_detect(int area)
{
	//sem_wait(&signal);
	switch(area)
	{
		case 0 : ROI = cvRect( x_pos - bias, y_pos ,pic_w , pic_h) ; cout<<ROI.x<<" "<<ROI.y<<endl;break ;
		case 1 : ROI = cvRect( x_pos - bias, y_ori ,pic_w , pic_h) ; cout<<ROI.x<<" "<<ROI.y<<endl;break ;
		case 2 : ROI = cvRect( x_pos - bias, y_neg ,pic_w , pic_h) ; cout<<ROI.x<<" "<<ROI.y<<endl;break ;
		case 3 : ROI = cvRect( x_ori - bias, y_neg ,pic_w , pic_h) ; cout<<ROI.x<<" "<<ROI.y<<endl;break ;
		case 4 : ROI = cvRect( x_neg - bias, y_neg ,pic_w , pic_h) ; cout<<ROI.x<<" "<<ROI.y<<endl;break ;
		case 5 : ROI = cvRect( x_neg - bias, y_ori ,pic_w , pic_h) ; cout<<ROI.x<<" "<<ROI.y<<endl;break ;
		case 6 : ROI = cvRect( x_neg - bias, y_pos ,pic_w , pic_h) ; cout<<ROI.x<<" "<<ROI.y<<endl;break ;
		case 7 : ROI = cvRect( x_ori - bias, y_pos ,pic_w , pic_h) ; cout<<ROI.x<<" "<<ROI.y<<endl<<endl;break ;
		default : cout<<"area error !! "<<endl;exit(1);
		
		
	}
	ROI_open = true ;
	//sem_post(&signal);
}

void PainterWidget::frame_per_second()
{

	//cout<<"frame_per_second function , picture_count size :"<<picture_count.size()<<" tempo timer id =  "<<tempo_timer->timerId()
	//<<" frame timer id = "<<pic_timer->timerId()<<endl;
	
	for(int index = 0 ; index < picture_count.size() ; index ++)
	{
		
		if( picture_count[index].is_open )
		{
			if(picture_count[index].frame_count >= NoteStep)
			{

				////把資料都存在vector struct 用queue存idle的struct
				///////////******************//////////
				(picture_count[index].pic_frame)->setX (width);
				(picture_count[index].pic_frame)->setY (height);
				(picture_count[index].pic_frame)->setWidth (pic_w);
				(picture_count[index].pic_frame)->setHeight (pic_h);
				picture_count[index].frame_count = 0 ;
				picture_count[index].is_open = false ;
				ROI_detect(picture_count[index].area);
				
				//cout << width <<" " << height <<" "<<picture1->width()<<"  "<<picture1->height()<<endl;
				
			}
			else
			{
				(picture_count[index].pic_frame)->setX ( (picture_count[index].pic_frame)->x() + picture_count[index].ranx );
				(picture_count[index].pic_frame)->setY ( (picture_count[index].pic_frame)->y() + picture_count[index].rany );
				(picture_count[index].pic_frame)->setWidth( (picture_count[index].pic_frame)->width() + picture_count[index].ranx );
				(picture_count[index].pic_frame)->setHeight( (picture_count[index].pic_frame)->height() + picture_count[index].rany );
				//picture1->setY ((picture1->y()) + rany );
				//picture1->setWidth (picture1->width()+ranx);
				//picture1->setHeight (picture1->height()+rany);
				
				picture_count[index].frame_count++;
			}
		}
	}
	
    update();
	//repaint ();
}





void PainterWidget::counting_down_to_start()
{
	cout<<count_to_zero<<endl;
	if(count_to_zero == 3)countdown->setText("3");
	if(count_to_zero == 2)countdown->setText("2");
	if(count_to_zero == 1)countdown->setText("1");
	
	countdown->setFont(QFont("Courier", 72, QFont::Bold));
	countdown->setGeometry(345,198,80,80);
	if(count_to_zero == 0)
	{
		disconnect(pic_timer, SIGNAL(timeout()), this, SLOT(counting_down_to_start()));
		delete countdown;
		mode = 1;
		connect(pic_timer, SIGNAL(timeout()), this, SLOT(frame_per_second()));
		pic_timer->start(FPS);
		
	}
	count_to_zero-- ;
	
}

void PainterWidget::game_start()
{
	delete btn_start,btn_detect ;
	btn_exit->setGeometry(655,440,80,25);

	connect(pic_timer, SIGNAL(timeout()), this, SLOT(counting_down_to_start()));
	pic_timer->start(1000);

	
}

void PainterWidget::game_exit(){
	exit(1);
}

int PainterWidget::MusicSheet(){
	return 1;
}

void PainterWidget::paintEvent(QPaintEvent*event) {
	
	
	if(mode == 1)
	{
		QPainter  painter(this);
		//pImage = cvQueryFrame( pCapture );
		cvResize(pImage, small, CV_INTER_AREA );
		
		if(ROI_open == true)
		{
			//sem_wait()
			ROI_open = false ;
			cout<<ROI.x<<" , "<<ROI.y<<endl;
			cvSetImageROI(small,ROI);
			//CvSeq* detect(IplImage* ROI);
			face_detect(small,cascade,storage);
			cvResetImageROI(small);

			
			
			cvRectangle( small, cvPoint(x_pos - bias, y_pos) , cvPoint(pic_w + x_pos - bias ,pic_h + y_pos) , CV_RGB(0,255,0), 3 , 8 , 0);
			cvRectangle( small, cvPoint(x_pos - bias, y_ori) , cvPoint(pic_w + x_pos - bias ,pic_h + y_ori) , CV_RGB(0,255,0), 3 , 8 , 0);
			cvRectangle( small, cvPoint(x_pos - bias, y_neg) , cvPoint(pic_w + x_pos - bias ,pic_h + y_neg) , CV_RGB(0,255,0), 3 , 8 , 0);
			cvRectangle( small, cvPoint(x_ori - bias, y_neg) , cvPoint(pic_w + x_ori - bias ,pic_h + y_neg) , CV_RGB(0,255,0), 3 , 8 , 0);
			cvRectangle( small, cvPoint(x_neg - bias, y_neg) , cvPoint(pic_w + x_neg - bias ,pic_h + y_neg) , CV_RGB(0,255,0), 3 , 8 , 0);
			cvRectangle( small, cvPoint(x_neg - bias, y_ori) , cvPoint(pic_w + x_neg - bias ,pic_h + y_ori) , CV_RGB(0,255,0), 3 , 8 , 0);
			cvRectangle( small, cvPoint(x_neg - bias, y_pos) , cvPoint(pic_w + x_neg - bias ,pic_h + y_pos) , CV_RGB(0,255,0), 3 , 8 , 0);
			cvRectangle( small, cvPoint(x_ori - bias, y_pos) , cvPoint(pic_w + x_ori - bias, pic_h + y_pos) , CV_RGB(0,255,0), 3 , 8 , 0);
			
			cvRectangle( small, cvPoint( ROI.x , ROI.y) , cvPoint(pic_w + ROI.x, pic_h + ROI.y) , CV_RGB(255,0,0), 3 , 8 , 0);
		    cvxCopyIplImage(small,img);
			painter.drawImage(bias,0,img);
			
			int_score += 1;
			Q_score = QString::number(int_score, 10);
			score->setText(Q_score);
		}
		else
		{
			cvRectangle( small, cvPoint(x_pos - bias, y_pos) , cvPoint(pic_w + x_pos - bias ,pic_h + y_pos) , CV_RGB(0,255,0), 3 , 8 , 0);
			cvRectangle( small, cvPoint(x_pos - bias, y_ori) , cvPoint(pic_w + x_pos - bias ,pic_h + y_ori) , CV_RGB(0,255,0), 3 , 8 , 0);
			cvRectangle( small, cvPoint(x_pos - bias, y_neg) , cvPoint(pic_w + x_pos - bias ,pic_h + y_neg) , CV_RGB(0,255,0), 3 , 8 , 0);
			cvRectangle( small, cvPoint(x_ori - bias, y_neg) , cvPoint(pic_w + x_ori - bias ,pic_h + y_neg) , CV_RGB(0,255,0), 3 , 8 , 0);
			cvRectangle( small, cvPoint(x_neg - bias, y_neg) , cvPoint(pic_w + x_neg - bias ,pic_h + y_neg) , CV_RGB(0,255,0), 3 , 8 , 0);
			cvRectangle( small, cvPoint(x_neg - bias, y_ori) , cvPoint(pic_w + x_neg - bias ,pic_h + y_ori) , CV_RGB(0,255,0), 3 , 8 , 0);
			cvRectangle( small, cvPoint(x_neg - bias, y_pos) , cvPoint(pic_w + x_neg - bias ,pic_h + y_pos) , CV_RGB(0,255,0), 3 , 8 , 0);
			cvRectangle( small, cvPoint(x_ori - bias, y_pos) , cvPoint(pic_w + x_ori - bias, pic_h + y_pos) , CV_RGB(0,255,0), 3 , 8 , 0);
			cvxCopyIplImage(small,img);
			painter.drawImage(bias,0,img);
		}
		
		for(int x = 0 ; x < picture_count.size() ; x++ )
			if(picture_count[x].is_open) painter.drawPixmap( *(picture_count[x].pic_frame), *catImg[0] );
	}
	



}

void PainterWidget::detect()
{
	if(is_detect==0)
	{
		btn_detect->setText("detect off");
	}
	else
	{
		btn_detect->setText("detect on");
	}
	is_detect=!is_detect;
	update();
}

void PainterWidget::musicnote_load()
{
	//cout<<"test clock musicnote_load "<<endl;
	
	if( mode == 1 )
	{
		cout<<"musicload function : picture_count :　"<<picture_count.size()<<endl;
		if( tempo_sheet[sheet_iterator] == 1 )
		{
			frame_data temp ;
			random_x_y(temp.ranx, temp.rany, temp.area);
			temp.is_open = true ;
			temp.frame_count = 0 ;
			
			
			bool all_is_full = true ; 
			for(int x = 0 ; x < picture_count.size() ; x++)
			{
				if(!picture_count[x].is_open)
				{
					all_is_full  = false ;
					temp.pic_frame = new QRect( width, height ,pic_w , pic_h);
					picture_count[x] = temp ; 
					break ;
				}
			}
			//cout<<"test musicnote_load() , all_is_full = "<<all_is_full<<" picture_count.size() = "<<picture_count.size()<<endl;
			if(all_is_full)
			{
				temp.pic_frame = new QRect( width, height ,pic_w , pic_h );
				picture_count.push_back(temp);
				
			}
		}
		
		
		if( !(sheet_iterator > (tempo_sheet.size() -1) ) )
		{
			sheet_iterator++;
		}
		else
		{
			stoptime->start(2000);
			mode = 0 ;
			cout<<"sheet end , process will be killed "<<endl;
			
		}
	}
	//cout<<" musicload function  sheet_iterator = "<<sheet_iterator<<"   , tempo_sheet size = "<<tempo_sheet.size()<<endl;
	
}

IplImage* PainterWidget::face_detect(IplImage* img,CvHaarClassifierCascade *cascade,CvMemStorage *storage){

	if( !cascade ){
	    fprintf(stderr, "ERROR: Could not load classifier cascade\n");
	    return img;
	}
	// Be careful size of detection area
	CvSeq* faces=cvHaarDetectObjects(img,cascade,storage,1.1,2,CV_HAAR_DO_CANNY_PRUNING,cvSize(0,0),cvSize(100,100));

	

	CvPoint pt1, pt2;
	if(faces){

	    for(int a = 0 ; a < faces->total ; a++){
		
			CvRect* rectangle = (CvRect*)cvGetSeqElem(faces, a);
			pt1.x = rectangle->x;
			pt2.x = rectangle->x + rectangle->width;
			pt1.y = rectangle->y;
			pt2.y = rectangle->y + rectangle->height;
			CvRect rect1 = cvRect(pt1.x,pt1.y,rectangle->width,rectangle->height);			
	
			cvRectangle(img, pt1, pt2, CV_RGB(255,0,0), 3, 8, 0);

		}	
	}
	return img;
}

void PainterWidget::cvxCopyIplImage(const IplImage *pIplImage,QImage &qImage)
{   //implement the conversion function here

	if(qImage.isNull())
        {
                int w = pIplImage->width;
                int h = pIplImage->height;
                qImage = QImage(w, h, QImage::Format_RGB32);
        }

	int x, y;
	for(x = 0; x < pIplImage->width; ++x)
        {
	   for(y = 0; y < pIplImage->height; ++y)
	   {
	      CvScalar color = cvGet2D(pIplImage, y, x);

	      int r = color.val[2];
	      int g = color.val[1];
	      int b = color.val[0];

	      qImage.setPixel(x, y, qRgb(r,g,b));
	   }
    }
}
