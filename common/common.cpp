#include "common.h"
//#include <dirent.h>

using namespace std;
using namespace cv;

// This function returns every line of 'filename' as a string
vector<string> ReadFileLinesAsString(string filename)
{
    vector<string> str_arr;
    string line;
    ifstream myfile(filename.c_str());
    if (myfile.is_open()) {
        while ( getline(myfile, line)) {
            str_arr.push_back(line);
        }
        myfile.close();
    }
    else {
        cout << "Unable to open file " << filename << endl;
    }
    
    return str_arr;
}

int BuildPath(string basePath, string namePrefix, string name, string ext, string &outputPath)
{
	outputPath = basePath + namePrefix + "_" + name + ext;
	return 1;

}

//int LoadVideo(string videoPath, vector<Mat> &video, vector<pair<Point2f, Point2f> > &eyes_coords, vector<string> &frames_names, int max_count)
//{
//    DIR *dir;
//    struct dirent *ent;
//    
//    if ((dir = opendir (videoPath.c_str())) != NULL) 
//    {
//        while ((ent = readdir (dir)) != NULL && max_count>0) 
//        {
//            string extension = get_extension(ent->d_name);
//
//            if (extension == ".jpg" || extension == ".png")
//            {
//                string frm_path = videoPath + ent->d_name;
//                Mat frm_orig, gray;
//                frm_orig = imread(frm_path, CV_LOAD_IMAGE_COLOR);
//                if (frm_orig.empty())
//                    break;
//
//                /* Storing video */
//                video.push_back(frm_orig/*gray*/);
//                frames_names.push_back(ent->d_name);
//                max_count--;
//
//                replace_last(frm_path, extension, ".txt");
//                auto eyes_coord_file = ifstream(frm_path, ios::in);
//                if (eyes_coord_file.is_open()) {
//                    int left_x, left_y, right_x, right_y;
//                    eyes_coord_file >> left_x;
//                    eyes_coord_file >> left_y;
//                    eyes_coord_file >> right_x;
//                    eyes_coord_file >> right_y;
//
//                    /* Storing eyes coords */
//                    eyes_coords.push_back(pair<Point2f, Point2f>(Point2f(left_x, left_y), Point2f(right_x, right_y)));
//                }
//            }
//        }
//        closedir (dir);
//    }
//    if (video.size()==0)
//        //cout << "ERROR: Cannot open video " << videoPath << endl;
//        return -1;
//    
//return 1;
//}

int SaveDescrBinary(string path, vector<float> descr, int dim_desc)
{
	FILE* f = fopen(path.c_str(), "wb");
	if (f == NULL)
		return -1;
	
	fwrite(descr.data() , sizeof(float), descr.size(), f);
	fclose(f);

	string t_path = path;
	size_t idx = path.find_last_of('.');						
	t_path.insert(idx, "_info");
	f = fopen(t_path.c_str(), "w");
	fprintf(f, "%d", (int)descr.size());
	fclose(f);

	return 1;
}

int LoadDescrBinary(string path, vector<float> &descr, int size)
{
	FILE* f = fopen(path.c_str(), "rb");
	if (f == NULL)
		return -1;

	descr.resize(size);
	fread(descr.data() , sizeof(float), size, f);

	fclose(f);

	return 1;
}

int LoadDescrBinaryInfo(string path, int &size)
{
	std::ifstream f(path);
	if(!f.is_open())
		return -1;

	string s;
	f >> s ;
	size = stod(s);

	return 1;
}

int LoadTextFile(string path, vector<float> &output)
{
	ifstream in(path);
	if (in.fail())
		return -1;

	string line;	
	if (in) {
		while (in >> line)
			output.push_back(stod(line));
	}	

	return 1;
}

int SaveDescrAsText(string path, vector<float> descr, int dim_desc)
{
	int d = 1;
    std::ofstream fout(path);
	if (fout.fail()){
		cout << "ERROR opening file " << path << endl;
		return -1;
	}

	for (int f = 0 ; f < descr.size() ; f++)
	{
		fout << descr[f] << " "   ;	
		if (f == (d * dim_desc - 1))
		{
			fout << endl;
			d++;					
		}				
	}

	return 1;
}

int SaveDescrAsText(string path, vector<float> descr)
{
    std::ofstream fout(path);
    if (fout.fail()){
        cout << "ERROR opening file " << path << endl;
        return -1;
    }
    
    for (int f = 0 ; f < descr.size() ; f++)
        fout << descr[f] << endl;
    
    return 1;
}

int SaveDescrAsTextInRow(string path, vector<float> descr, std::ios_base::openmode mode)
{
    std::ofstream fout(path, mode);
	if (fout.fail()){
		cout << "ERROR opening file " << path << endl;
		return -1;
	}

	for (int f = 0 ; f < descr.size() ; f++)
		fout << descr[f] << " ";
    fout << endl;

	return 1;
}


void WriteMatToFile(std::ofstream &f, cv::Mat &m)
{
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            f << m.at<float>(i,j) << " ";
        }
        f << endl;
    }    
}

//-- This function loads features stored in a text file
//-- Every feature takes 1 line, and its values separated by a space
Mat LoadFeaturesFromTextFile(string filename)
{
    std::ifstream file(filename);
    
    std::string   line;
    
    Mat features;
    
    // Read one line at a time into the variable line:
#ifdef DEBUG
	uint i = 0;
#endif
	while(std::getline(file, line))
    {
        std::vector<float>   lineData;
        std::stringstream  lineStream(line);
        
        float value;
        // Read a float at a time from the line
		while(lineStream >> value)
        {
            // Add the float from a line to a 1D array (vector)
            lineData.push_back(value);
		}
        
        Mat aux(1, (int)lineData.size(), CV_32FC1, lineData.data());
        features.push_back(aux);
#ifdef DEBUG
		i++;
		if (i >= 100)
			break;
#endif
	}
    
    return features;
}

int GetNumberOfLinesInTextFile(string path)
{
	std::ifstream file(path);
	int number_of_lines = 0;
	if (file.is_open()){
		string line;
		while (std::getline(file, line))
			++number_of_lines;
	}
	return number_of_lines;
}

string to_string_(int value)
{
    char buf[128];
    sprintf(buf, "%d", value);
    return string(buf);
}

vector<string> &split_string(const string &s, char delim, vector<string> &elems) 
{
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }

    return elems;
}

void replaceAll(std::string &str, const std::string &from, const std::string &to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

//fbecerra
void draw_points(Mat img, vector<Point2f> points)
{
//	char tex_point[4];
//	for (int i = 0; i < points.size(); i++)
//	{
//		itoa(i, tex_point, 10);
//		string number = string(tex_point);
//
//		putText(img, number, Point(points[i].x, points[i].y), FONT_HERSHEY_PLAIN, 0.5, CV_RGB(0, 255, 0));
//	}
}

//fbecerra 
void replace_last(std::string &str, const std::string &from, const std::string &to)
{
	size_t start_pos = str.rfind(from);
	if (start_pos != std::string::npos)
		str.replace(start_pos, from.length(), to);
}

//fbecerra
std::string get_extension(const std::string &file_path)
{
	string ext(strrchr(file_path.c_str(), '.'));
	return ext;
}

//fbecerra
void save_filter_mask(const string filter_mask_path, const Mat filter_mask, const string ID)
{
	cv::FileStorage fs(filter_mask_path, cv::FileStorage::WRITE);
	fs << ID << filter_mask;
	fs.release();
}

//fbecerra
int load_filter_mask(const string filter_mask_path, Mat& filter_mask, const string ID)
{
	cv::FileStorage fs(filter_mask_path, FileStorage::READ);
	fs[ID] >> filter_mask;
	fs.release();

	return !filter_mask.empty();
}

////fbecerra
//int load_video(const string video_path, vector<Mat> &video, vector<Mat> &masks, vector<pair<Point2f, Point2f> > &eyes_coords, vector<string> &frames_names, int max_count, const bool load_masks)
//{
//    DIR *dir;
//    struct dirent *ent;
//
//    if ((dir = opendir(video_path.c_str())) != NULL)
//    {
//        while ((ent = readdir(dir)) != NULL && max_count > 0)
//        {
//            string extension = get_extension(ent->d_name);
//
//            if (extension == ".jpg" || extension == ".png")
//            {
//                string frm_path = video_path + ent->d_name;
//                Mat frm_orig, gray;
//                frm_orig = imread(frm_path, CV_LOAD_IMAGE_COLOR);
//                if (frm_orig.empty())
//                    break;
//
//                /* Storing video */
//                video.push_back(frm_orig);
//                frames_names.push_back(ent->d_name);
//                max_count--;
//
//                string eyes_coords_path = frm_path;
//                replace_last(eyes_coords_path, extension, ".txt");
//                auto eyes_coord_file = ifstream(eyes_coords_path, ios::in);
//                if (eyes_coord_file.is_open()) {
//                    int left_x, left_y, right_x, right_y;
//                    eyes_coord_file >> left_x;
//                    eyes_coord_file >> left_y;
//                    eyes_coord_file >> right_x;
//                    eyes_coord_file >> right_y;
//
//                    /* Storing eyes coords */
//                    eyes_coords.push_back(pair<Point2f, Point2f>(Point2f(left_x, left_y), Point2f(right_x, right_y)));
//
//                }
//
//                //-----glasses mask-----
//
//                /* If LOAD_MASKS is activated, an existing mask is stored or an empty one if no mask */
//                if (load_masks)
//                {
//                    string mask_path = frm_path;
//                    replace_last(mask_path, extension, "_mask.yml");
//                    Mat m;
//                    load_filter_mask(mask_path, m, "glasses_mask");
//
//                    /* Storing mask */
//                    masks.push_back(m);
//                }
//            }
//        }
//        closedir(dir);
//    }
//    if (video.size() == 0)
//        //cout << "ERROR: Cannot open video " << videoPath << endl;
//        return -1;
//
//    return 1;
//}

////fbecerra
//int get_files(const string folder_path, const string extension, vector<string> &files, int max_number, const bool deeper_search)
//{
//    DIR *dir;
//    struct dirent *ent;
//
//    if ((dir = opendir(folder_path.c_str())) != NULL)
//    {
//        while ((ent = readdir(dir)) != NULL && max_number > 0)
//        {
//            if (ent->d_type == DT_DIR)
//            {
//                if (strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..") && deeper_search)
//                    get_files(folder_path + "/" + ent->d_name, extension, files, max_number, deeper_search);
//            }
//            else
//            {
//                string current_ext = get_extension(ent->d_name);
//                if (extension == current_ext)
//                {
//                    files.push_back(folder_path + "/" + ent->d_name);
//                    max_number--;
//                }
//            }
//        }
//        closedir(dir);
//    }
//    if (files.size() == 0)
//        //cout << "ERROR: Cannot open folder path " << folder_path << endl;
//        return -1;
//
//    return 1;
//}

int ComputeDistance(vector<float> x, vector<float> y, int type_dist, float &dist)
{
	CV_Assert(x.size() == y.size());
	
	if (type_dist == 0) // EUCLIDEANA
	{
		float sum = 0;
		for (int i = 0 ; i < x.size() ; i++) {
			sum = sum + (x[i] - y[i]) * (x[i] - y[i]);	
		}

		dist = sqrt(sum);
	}
		
	return 1;
}

int ComputeMaxScales(Size sz, int patch_size, float featureScaleMul)
{
	int max_level_scale = ceil( log ( float( min (sz.width, sz.height) / patch_size) ) / log(featureScaleMul) ); 
	return max_level_scale ;
}

float* convertImagetoFloat(const Mat& img) 
{
	int cont = 0;
	int w = img.cols;
	int h = img.rows;
	int l = w * h;
	float* result = new float[l];

	for (int i = 0; i < h; i++) {		
		for (int j = 0; j < w; j++){
			result[cont] = img.at<uchar>(i,j);			
			cont = cont + 1;
		}
	}

	return result;
}


float* convertImagetoFloat2(const Mat& img)
{
	int cols = img.cols, rows = img.rows, cont = 0;
	float* result = new float[cols * rows];
	if (img.isContinuous())
	{
		cols *= rows;
		rows = 1;
	}
	for (int i = 0; i < rows; i++)
	{
		const uchar* Mi = img.ptr<uchar>(i);
		for (int j = 0; j < cols; j++)
			result[cont++] = Mi[j];
	}
	return result;
}

float* convertImagetoFloat2_CV32F(const Mat& img)
{
    int cols = img.cols, rows = img.rows, cont = 0;
    float* result = new float[cols * rows];
    if (img.isContinuous())
    {
        cols *= rows;
        rows = 1;
    }
    for (int i = 0; i < rows; i++)
    {
        const float* Mi = img.ptr<float>(i);
        for (int j = 0; j < cols; j++)
            result[cont++] = Mi[j];
    }
    return result;
}

float* convertImagetoFloat3(const Mat& img)
{
	int cols = img.cols, rows = img.rows, cont = 0;
	float* result = new float[cols * rows];
	MatConstIterator_<uchar> it = img.begin<uchar>(), it_end = img.end<uchar>();
	for (; it != it_end; ++it)
		result[cont++] = *it;
	return result;
}

void ComputeNormL2(const vector<float> &descr_denseSift, int dim_descr, vector<float> &feat_norm)
{
	int cant_descr = (int)descr_denseSift.size() / dim_descr;
	feat_norm.reserve(cant_descr);

	float sum = powf(descr_denseSift[0], 2.) ;

	for (int i = 1 ; i < descr_denseSift.size() ; i++)
	{				
		if (i % dim_descr == 0){
			feat_norm.push_back(sqrt(sum));
			sum = 0;
		}	

		sum += powf(descr_denseSift[i], 2.);
	}

	feat_norm.push_back(sqrt(sum));

}


float L2_Dist(Mat &a, int i, int j, float thres)
{
    float d = 0;
    float sq_thres = thres * thres;
    
    const float* Ma = a.ptr<float>(i);
    const float* Mb = a.ptr<float>(j);
    
    for (int i = 0; i < a.cols; i++) {
        d += (Ma[i]-Mb[i])*(Ma[i]-Mb[i]);
        if (d > sq_thres)
            return sqrt(d);
    }
    
    return sqrt(d);
}

// This is the vectorized version of the L2_Dist function
// which is ~2x faster
float L2_Dist_vectorized(Mat &a, int i, int j, float thres)
{
    float sq_thres = thres * thres;
    
    const float* Ma = a.ptr<float>(i);
    const float* Mb = a.ptr<float>(j);
    
    float d1 = 0, d2 = 0, d3 = 0, d4 = 0;
    
    int ii = 0;
    for(; ii < a.cols; ii += 4) {
        d1 += (Ma[ii]-Mb[ii])*(Ma[ii]-Mb[ii]);
        d2 += (Ma[ii + 1]-Mb[ii + 1])*(Ma[ii + 1]-Mb[ii + 1]);
        d3 += (Ma[ii + 2]-Mb[ii + 2])*(Ma[ii + 2]-Mb[ii + 2]);
        d4 += (Ma[ii + 3]-Mb[ii + 3])*(Ma[ii + 3]-Mb[ii + 3]);
        if (d1 + d2 + d3 + d4 > sq_thres)
            return sqrt(d1 + d2 + d3 + d4);
    }
    
    for (; ii < a.cols; ii++)
        d1 += (Ma[ii]-Mb[ii])*(Ma[ii]-Mb[ii]);
    
    return sqrt(d1 + d2 + d3 + d4);
}

//Leyanis hat and shoulder
Mat EstandarizeImg(Mat& img, int target_heght)
{
	if (img.rows > target_heght)
	{
		Mat resize_img;
		float scale = static_cast<float>(target_heght) / img.rows;
		resize(img, resize_img, Size(img.cols*scale, target_heght));
		return resize_img;
	}
	return img;
}

//Leyanis
///<summary>
///This method normalize value between [0 1]
///The formula is a parabola with vertice in (middle; 1)
///and forced to pass trough (deviation; 0.5).
///</summary>
/// <param name="value">Value to be normalized</param>
/// <param name="middle">Vertex of the parabola</param>
/// <param name="deviation"></param>
/// <returns>A value between 0 and 1</returns>
double NormalizeValue_between_01(double value, double middle, double deviation)
{
	double temp_value = value - middle;
	double result = temp_value * temp_value / (-2 * (deviation * deviation)) + 1;
	if (result < 0) return 0;
	if (result > 1) return 1;
	return result;
}
//Bouza
double CalculateScore(double min, double max, double thresh, double value)
{
	if (value < min) return 0;
	if (value > max) return 1;
	if (value < thresh)
		return (value - min) / (thresh - min)*0.5;
	return (value - thresh) / (max - thresh)*0.5 + 0.5;
}

//Leyanis from Janus_FacePointsDetector
std::vector<Point2f> GetPointEyes(std::vector<Point2f> obj)
{
	int i;
	std::vector<Point2f> point;
	for (i = 36; i <= 47; i++)
	{
		Point2f b(obj[i].x, obj[i].y);
		point.push_back(b);
	}
	return point;
}

//Leyanis from Janus_FacePointsDetector
// The former method had some problems with lines intersection, 
// instead, here we use centroid
std::vector<Point2f> GetCentroEyes(std::vector<Point2f> a)
{
	std::vector<Point2f> result;
	int lidxs[4] = { 1, 4, 2, 5 };
	int ridxs[4] = { 7, 10, 8, 11 };

	Point2f pt(0, 0);
	for (size_t i = 0; i < 4; i++)
		pt += a[lidxs[i]];

	result.push_back(pt * 0.25);

	pt = Point2f(0, 0);
	for (size_t i = 0; i < 4; i++)
		pt += a[ridxs[i]];

	result.push_back(pt * 0.25);

	return result;
}

//Leyanis from Janus_FacePointsDetector
std::vector<Point2f> GetPointMouth(std::vector<Point2f> obj)
{
	int i;
	std::vector<Point2f> point;
	for (i = 48; i <= 67; i++)
	{
		Point2f b(obj[i].x, obj[i].y);
		point.push_back(b);
	}
	return point;
}

void DrawLabelToRectangle(cv::Mat &img, cv::Rect r, string text, cv::Scalar text_color, cv::Scalar bg_color, double fontScale, int thickness)
{
	int fontFace = FONT_HERSHEY_TRIPLEX;

	int baseline = 0;
	cv::Size textSize = cv::getTextSize(text, fontFace, fontScale, thickness, &baseline);

	int offset = 10;
	cv::Rect bck_grnd(r.x + r.width, r.y + r.height - 2 * offset - textSize.height,
		2 * offset + textSize.width, 2 * offset + textSize.height);
	cv::rectangle(img, bck_grnd, bg_color, -1);

	// center the text
	cv::Point textOrg(bck_grnd.x + offset, bck_grnd.y + offset + textSize.height);

	// then put the text itself
	cv::putText(img, text, textOrg, fontFace, fontScale,
		text_color, thickness, 8);
}

float IntersectionOverUnionRatio(Rect a, Rect b)
{
	Rect intersection_rect = a & b;
	float union_area = a.area() + b.area() - intersection_rect.area();
	return (float)intersection_rect.area() / union_area;
}

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

vector<float> Mat2VectorFloat(Mat mat)
{
    std::vector<float> array;
    if (mat.isContinuous()) {
        array.assign((float*)mat.datastart, (float*)mat.dataend);
    } else {
        for (int i = 0; i < mat.rows; ++i) {
            array.insert(array.end(), mat.ptr<float>(i), mat.ptr<float>(i)+mat.cols);
        }
    }
    return array;
}

void GetInterval(int size, int block_count, int block_idx, int &block_init, int &block_end)
{
    int block_sz = cvCeil((float)size / block_count);
    block_init = block_sz * block_idx;
    block_end = min(size, block_sz * (block_idx + 1));
}

Mat my_ramdom_crop(Mat im, RNG &rng, float min_scale, float max_scale,
                           float min_rotation, float max_rotation,
                           float min_translation, float max_translation)
{
    Mat nimg;
    
    //-- Compute scaling
    float s = rng.uniform(min_scale, max_scale);
    
    //-- Compute rotation
    double angle = rng.uniform(min_rotation, max_rotation) * (rand()%2 == 0 ? 1 : -1);
    
    //-- Compute translation
    Point2f t((rand()%2 == 0 ? 1 : -1) * rng.uniform(min_translation, max_translation), (rand()%2 == 0 ? 1 : -1) * rng.uniform(min_translation, max_translation));
    
//    cout << s << " " << angle << " " << t.x << " " << t.y << endl;
    
    //-- Set transformation matrix
    Point rp = angle < 0 ? Point(0, 0) : Point(im.cols, 0);
    Mat mtx = getRotationMatrix2D(rp, angle, s);
    mtx.at<double>(0, 2) += t.x;
    mtx.at<double>(1, 2) += t.y;
    
    //-- Perform affine transformation
    Size aSize(im.cols * s * 2, im.rows * s * 2);
    if (s > 1)   // For better visual results, use CV_INTER_LINEAR for image zooming
        warpAffine(im, nimg, mtx, aSize, CV_INTER_LINEAR, 0, Scalar(255, 255, 255));
    else         // For better visual results, use CV_INTER_AREA for image decimation
        warpAffine(im, nimg, mtx, aSize, CV_INTER_AREA, 0, Scalar(255, 255, 255));
    
    return nimg;
}

vector<Mat> my_jitter_img(Mat im, int count,
                                  float min_scale, float max_scale,
                                  float min_rotation, float max_rotation,
                                  float min_translation, float max_translation)
{
    RNG rng;
    vector<Mat> im_arr;
    for (int i = 0; i < count; i++) {
        Mat im_i = my_ramdom_crop(im, rng, min_scale, max_scale, min_rotation, max_rotation, min_translation, max_translation);
        im_arr.push_back(im_i);
    }
    return im_arr;
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

// Angle of p2 relative to p1
// (Vectors p1 and p2 must be normalized)
double AngleBetweenTwoVectors(Point2f p1, Point2f p2)
{
    return atan2(p2.y, p2.x) - atan2(p1.y, p1.x);
}

// Angle of p2 relative to p1
// (Vectors p1 and p2 must be normalized)
double AngleBetweenTwoVectors(Point p1, Point p2)
{
    return atan2(double(p2.y), double(p2.x)) - atan2(double(p1.y), double(p1.x));
}

// Angle betwwen p2 and p1
// (Vectors p1 and p2 must be normalized)
double AngleBetweenTwoVectors_orderless(Point2f p1, Point2f p2)
{
    return acos(p1.x * p2.x + p1.y * p2.y);
}

// Angle of vector p1 relative to X axis [-pi/2, +pi/2]
double AngleBetweenVectorAndXAxis(Point p1)
{
    return atan(double(p1.y/p1.x));
}

// Angle of vector p1 relative to X axis using atan2 [-pi, +pi]
double AngleBetweenVectorAndXAxis2(Point p1)
{
    return atan2(float(p1.y), float(p1.x));
}


// Rotate and scale a 2D point about another
Point TransformedPoint(Point p, Point origin, Point traslation, float angle, float scale)
{
    Point tp, aux_p;
    aux_p = p - origin;
    tp.x = aux_p.x * cos(angle) - aux_p.y * sin(angle);
    tp.y = aux_p.y * cos(angle) + aux_p.x * sin(angle);
    tp *= scale;
    tp += traslation;
    return tp;
}

Point2f NormalizeVector(Point2f v)
{
    float magnitude = cvSqrt((float)(v.x * v.x + v.y * v.y));
    return v * (1.f/magnitude);
}

Point2f NormalizeVector(Point v)
{
    float magnitude = cvSqrt((float)(v.x * v.x + v.y * v.y));
    return Point2f(v.x, v.y) * (1.f/magnitude);
}

Point2f NormalizeVector(Point v, float &magnitude)
{
    magnitude = cvSqrt((float)(v.x * v.x + v.y * v.y));
    return v * (1.f/magnitude);
}

Point2f VectorComplement(Point2f v)
{
    return v * -1.f;
}

// Join two images,
Mat JoinImages(Mat im1, Mat im2, int orientation /* 0:horizontal, 1:vertical*/)
{
    CV_Assert(im1.type() == im2.type());
    
    Size newSize;
    Mat roi1, roi2;
    if (orientation == 0) {
        newSize = Size(max(im1.rows, im2.rows), im1.cols + im2.cols);
    }
    else {
        newSize = Size(im1.rows + im2.rows, max(im1.cols,im2.cols));
    }
    
    Mat img_g = Mat::zeros(newSize, im1.type());
    
    roi1 = Mat(img_g, Rect(0, 0, im1.cols, im1.rows));
    roi2 = Mat(img_g, Rect(im1.cols, 0, im2.cols, im2.rows));
    
    
    
    Mat roi(img_g, Rect(0, 0, im1.cols, im1.rows));
    im1.copyTo(roi);
    roi = Mat(img_g, Rect(im1.cols, 0, im2.cols, im2.rows));
    im2.copyTo(roi);
    Mat img;
    cvtColor(img_g, img, CV_GRAY2BGR);
    return img;
}



