#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>

using namespace cv;
using namespace std;

Mat img[13];
Mat masks[13];
Mat girlImg[13];
Mat girlMasks[13];
Mat bg;
Mat bg_org;
int idx=0;
int transformScale = 10;
Mat bgCrop;

int dx1, dx2, gx1, gx2, dy1, dy2, gy1, gy2;



//should work on mouse drag event
void onMouseClick(int event, int x, int y, int flags, void* userdata) 
{
    //instead of drag,moving both dino and girl images on left mouse click now
    //should fix and change the functionality to onDrag
    if (event == EVENT_LBUTTONDOWN) {
        cout << x << "-" << y << endl;

        if (x + img[idx].cols + girlImg[idx].cols < bg.cols && y + img[idx].rows < bg.rows) {

            dx1 = y;
            dx2 = y + img[idx].rows;
            gx1 = y;
            gx2 = y + girlImg[idx].rows;
            dy1 = x;
            dy2 = x + img[idx].cols;
            gy1 = x + img[idx].cols;
            gy2 = x + img[idx].cols + girlImg[idx].cols;

            bg_org.copyTo(bgCrop);
        }
        else {
            cout << "Selection out of Boundary. Reset to the Centre of the screen" << endl;
            //reset to center of the screen
            dx1 = bg.rows / 2;
            dx2 = dx1 + img[0].rows;
            gx1 = bg.rows / 2;
            gx2 = gx1 + girlImg[0].rows;
            dy1 = bg.cols / 4;
            dy2 = dy1 + img[0].cols;
            gy1 = bg.cols / 4 + img[0].cols;
            gy2 = gy1 + girlImg[0].cols;

        }

    }
    else if (event == EVENT_MOUSEMOVE && EVENT_FLAG_LBUTTON) {
        //should work on drag
    }
    else if (event == EVENT_LBUTTONUP) {
        //should work on drag event
    }
 }

char filenames[200];
char girlFilenames[200];

void loadImages(int i) {
    sprintf_s(filenames, "C:/Users/surya/Downloads/imgs/dinosaur%02d.jpg", i);
    img[i] = imread(filenames);
    sprintf_s(girlFilenames, "C:/Users/surya/Downloads/imgs/girl%02d.jpg", i);
    girlImg[i] = imread(girlFilenames);
}

int main(int argc, char** argv)
{
    namedWindow("animation", 1);
    setMouseCallback("animation", onMouseClick, NULL);

    bg = imread("C:/Users/surya/Downloads/imgs/bg_scene.jpg");
    bg.copyTo(bg_org);

    /*char filenames[200];
    char girlFilenames[200];*/

    for(int i = 0; i < 13; i++) {
        loadImages(i);

        masks[i].create(img[i].rows, img[i].cols, CV_8UC1);
        
        for (int y = 0; y < masks[0].rows; y++) {
            for (int x = 0; x < masks[0].cols; x++) {
                if(img[i].ptr<uchar>(y)[3*x]>220 && img[i].ptr<uchar>(y)[3 * x+1] > 220 && img[i].ptr<uchar>(y)[3 * x+2] > 220 ){
                    masks[i].ptr<uchar>(y)[x] = 0;
                }else masks[i].ptr<uchar>(y)[x] = 255;
            }
        }

        girlMasks[i].create(girlImg[i].rows, girlImg[i].cols, CV_8UC1);
        
        for (int y = 0; y < girlMasks[0].rows; y++) {
            for (int x = 0; x < girlMasks[0].cols; x++) {
                if (girlImg[i].ptr<uchar>(y)[3 * x] > 220 && girlImg[i].ptr<uchar>(y)[3 * x + 1] > 220 && girlImg[i].ptr<uchar>(y)[3 * x + 2] > 220) {
                    girlMasks[i].ptr<uchar>(y)[x] = 0;
                }
                else girlMasks[i].ptr<uchar>(y)[x] = 255;
            }
        }
    }


    dx1 = bg.rows / 2;
    dx2 = dx1 + img[0].rows;
    gx1 = bg.rows / 2;
    gx2 = gx1 + girlImg[0].rows;
    dy1 = bg.cols / 4;
    dy2 = dy1 + img[0].cols;
    gy1 = bg.cols / 4 + img[0].cols;
    gy2 = gy1 + girlImg[0].cols;

    while (1) {
        Rect bgrec(0,0,bg.cols/10,bg.rows);
        Rect balrec(bg.cols/10, 0, bg.cols-bg.cols / 10, bg.rows);

        Mat cutRec(bg, bgrec);
        Mat balRec(bg, balrec);
       
        balRec.copyTo(bg(Rect(0, 0, bg.cols - bg.cols / 10, bg.rows)));
        cutRec.copyTo(bg(Rect(bg.cols - bg.cols / 10, 0, bg.cols / 10, bg.rows)));

        bg.copyTo(bgCrop);

        if (idx > 12) {
            idx = 0;
        }

        img[idx].copyTo(bg(Range(dx1, dx2), Range(dy1, dy2)), masks[idx]);
        girlImg[idx].copyTo(bg(Range(gx1, gx2), Range(gy1, gy2)),girlMasks[idx]);
        idx++;

        imshow("animation", bg);

        bgCrop.copyTo(bg);

        uchar c = waitKey(30);
       
        if (c == 'a' || c == 'A') {
            if (dy1-transformScale <= 0) {
               
                dy1 = 0;
                dy2 = 0 + img[idx].cols;
                gy1 = dy2;
                gy2 = dy2 + girlImg[idx].cols;
            }
            else {
                dy1 -= transformScale;
                dy2 -= transformScale;
                gy1 -= transformScale;
                gy2 -= transformScale; 
            }
        }
        else if (c == 'd' || c == 'D') {
            if (gy2+transformScale >= bg.cols) {
               
                dy1 = bg.cols-girlImg[idx].cols-img[idx].cols;
                dy2 = bg.cols - girlImg[idx].cols;
                gy1 = bg.cols - girlImg[idx].cols;
                gy2 = bg.cols;
            }
            else {  
                dy1 += transformScale;
                dy2 += transformScale;
                gy1 += transformScale;
                gy2 += transformScale;
            }
        }
        else if (c == 'w' || c == 'W') {
            if(dx1-transformScale<=0 || gx1-transformScale<=0){
                dx1 = 0;
                gx1 = 0;
                dx2 = img[idx].rows;
                gx2 = girlImg[idx].rows;
            }
            else{
                dx1 -= transformScale;
                gx1 -= transformScale;
                dx2 -= transformScale;
                gx2 -= transformScale;
            }
        }
        else if (c == 's' || c == 'S') {
            if (dx2 + transformScale >= bg.rows || gx2 + transformScale >= bg.rows)
            {
                dx1 = bg.rows-img[idx].rows;
                gx1 = bg.rows-girlImg[idx].rows;
                dx2 = bg.rows;
                gx2 = bg.rows;
            }
            else {
                dx1 += transformScale;
                gx1 += transformScale;
                dx2 += transformScale;
                gx2 += transformScale;
                
            }
        }
        if (c == 27) break;  
    }
    return 1;
}