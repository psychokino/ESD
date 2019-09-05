#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QFont>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QImage>
#include <QtGui/QPushButton>
#include <vector>
#include <QPropertyAnimation>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <QtGui>
#include <vector>
#include <QPixmap>  
#include <QPaintEvent>  
#include <fstream>
#include <QThread>
#include <cstdlib>
#include <time.h>
//#include <QMediaPlayer>
//#include <phonon> 	
using namespace std;


class PainterWidget : public QWidget
{
    Q_OBJECT
	
	protected:
        void paintEvent(QPaintEvent*);

	public slots:
		void detect();  //// TA's code
		void frame_per_second(); //// process the gaming interface , decide graphic processing 
		void game_start(); /// switch mode from start interface to gaming interface
		void game_exit(); //// kill the program when you click it 
		int MusicSheet(); //// not use
		IplImage* face_detect(IplImage* img,CvHaarClassifierCascade *cascade,CvMemStorage *storage);   /// TA's code
		void cvxCopyIplImage(const IplImage *pIplImage,QImage &qImage);  //// TA's code
		void musicnote_load();  /// sheet's iterator , point to the music sheet
		void counting_down_to_start(); /// count from 3 to 0 when game mode ( mode = 1 )
		
		
	public:
		PainterWidget();
		~PainterWidget();
		
    
	private:

		struct frame_data //// store every note data
		{
			int frame_count;
			int ranx;
			int rany;
			int area;
			bool is_open;
			QRect *pic_frame;
			
		};
		//queue<int> idle_frame_sturct; // 如果為空 picture_count要新增一個 否則拿裡面的picture_count就好
		QTimer *pic_timer,*tempo_timer,*stoptime; //pic_timer一開始先從3數到0 在當成FPS // tempo_timer計算樂譜的間格
		QPushButton *btn_detect,*btn_start,*btn_exit;
		QPropertyAnimation *animation;
		IplImage *pImage,*small;
		vector<QPixmap*> catImg;
		vector<bool> MusicSheet_Data; // bool only use 1 bit , using bool type in order to reduce resource consuming
		//vector<QPixmap> catImg;
		QLabel *label, *score;
		QRect *picture1,*picture2 ;
		CvRect ROI;
		QLabel *countdown;
		QImage img;
		int count_to_zero;
		
		bool is_detect,ROI_open;//,picture1_open,picture2_open;
		CvHaarClassifierCascade *cascade;
		CvMemStorage *storage;
		int width,height,pic_w,pic_h;
		vector<frame_data> picture_count;
		int ranx,rany ;
		int pic_dir;
		void ROI_detect(int );
		
		
        CvCapture* pCapture;
		ifstream sheet;
		int sheet_iterator;
		int mode;
		double music_tempo,FPS;
		vector<int> tempo_sheet;
		
		int target_x ,target_y ,ROI_width, ROI_height,x0 , y0 ,int_score ,area;
		QString Q_score;
		
		int x_pos,x_neg,y_pos,y_neg,x_ori,y_ori;
};

#endif // WIDGET_H
