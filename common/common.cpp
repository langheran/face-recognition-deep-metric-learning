//
//  common.cpp
//  xcode_proj
//
//  Created by LChang on 1/9/18.
//  Copyright © 2018 MIT. All rights reserved.
//

#include "common.h"
using namespace std;
using namespace cv;

void DrawContour(Mat &img, vector<Point2f> pts, Scalar color, int thickness, bool closed_contour)
{
	for (size_t i = 0; i < pts.size() - 1; i++)	{
		line(img, Point(pts[i]), Point(pts[i + 1]), color, thickness);
	}
	if (closed_contour)
		line(img, Point(pts[0]), Point(pts[pts.size() - 1]), color, thickness);
}

void DrawFaceLines(Mat &img, vector<Point2f> pts)
{
	vector<Point2f> outerline(pts.begin(), pts.begin() + 17);
	DrawContour(img, outerline, CV_RGB(0, 255, 0), 2, false);

	vector<Point2f> nose(pts.begin() + 27, pts.begin() + 31);
	DrawContour(img, nose, CV_RGB(0, 255, 0), 2, false);

	vector<Point2f> nosetip(pts.begin() + 30, pts.begin() + 36);
	DrawContour(img, nosetip, CV_RGB(0, 255, 0), 2, true);

	vector<Point2f> leb(pts.begin() + 17, pts.begin() + 22);
	DrawContour(img, leb, CV_RGB(0, 255, 0), 2, false);

	vector<Point2f> reb(pts.begin() + 22, pts.begin() + 27);
	DrawContour(img, reb, CV_RGB(0, 255, 0), 2, false);

	vector<Point2f> le(pts.begin() + 36, pts.begin() + 42);
	DrawContour(img, le, CV_RGB(0, 255, 0), 2, true);

	vector<Point2f> re(pts.begin() + 42, pts.begin() + 48);
	DrawContour(img, re, CV_RGB(0, 255, 0), 2, true);

	vector<Point2f> ul(pts.begin() + 48, pts.begin() + 60);
	DrawContour(img, ul, CV_RGB(0, 255, 0), 2, true);

	vector<Point2f> ll(pts.begin() + 60, pts.begin() + 68);
	DrawContour(img, ll, CV_RGB(0, 255, 0), 2, true);
}

float Dist_L2(Point2f p1, Point2f p2)
{
    return sqrt((float)((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)));
}

float Dist_L2(Point p1, Point p2)
{
    return sqrt((float)((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)));
}

void ClosestPoints(Rect A, Rect B, Point &ptA, Point &ptB)
{
    Point arrA[] = {A.tl(), A.br(), A.tl() + Point(A.width,0), A.tl() + Point(0, A.height)};
    Point arrB[] = {B.tl(), B.br(), B.tl() + Point(B.width,0), B.tl() + Point(0, B.height)};
    
    float min_dist = 1e+10;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            float distAB = Dist_L2(arrA[i], arrB[j]);
            if (distAB < min_dist) {
                min_dist = distAB;
                ptA = arrA[i];
                ptB = arrB[j];
            }
        }
    }
}
