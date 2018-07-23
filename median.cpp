#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

void print_a(uchar *a)
{
    cout << "------" << endl;
    for (int i = 0; i < 3; i++)
        cout << a[i] << " ";
    cout << endl;
}

void print_b(uchar *a)
{
    cout << "------" << endl;
    for (int i = 0; i < 6; i++)
        cout << a[i] << " ";
    cout << endl;
}

void print_c(uchar *a)
{
    cout << "------" << endl;
    for (int i = 0; i < 8; i++)
        cout << a[i] << " ";
    cout << endl;
}

void sort3i(uchar *a, uchar *b)
{
    if (a[0] < a[1])
    {
        if (a[0] < a[2])
        {
            b[0] = a[0];
            if (a[1] < a[2])
            {
                b[1] = a[1];
                b[2] = a[2];
                return;
            }
            b[2] = a[1];
            b[1] = a[2];
            return;
        }
        b[0] = a[2];
        b[1] = a[0];
        b[2] = a[1];
        return;
    }
    // a[0] > a[1];
    if (a[0] < a[2])
    {
        b[0] = a[1];
        b[1] = a[0];
        b[2] = a[2];
        return;
    }
    // a[1] < a[0]; a[2] < a[0];
    b[2] = a[0];
    if (a[1] < a[2])
    {
        b[0] = a[1];
        b[1] = a[2];
        return;
    }
    b[0] = a[2];
    b[1] = a[1];
}

/*
      merge two ordered integer arrays
*/
void merge_i_manually(uchar *a, int n, uchar *b, int m, uchar *c)
{
    if(a[0] < b[0]){
        c[0] = a[0];
        if(a[1] < b[0]){
            c[1] = a[1];
            if(a[2] < b[0]){
                c[2] = a[2];
                c[3] = b[0];
                c[4] = b[1];
                c[5] = b[2];
            }else{
                c[2] = b[0];
                if(a[2] < b[1]){
                    c[3] = a[2];
                    c[4] = b[1];
                    c[5] = b[2];
                }else{
                    c[3] = b[1];
                    if(a[2] < b[2]){
                        c[4] = a[2];
                        c[5] = b[2];
                    }else{
                        c[4] = b[2];
                        c[5] = a[2];
                    }
                }
            }
        }else{
            c[1] = b[0];
            if(a[1] < b[1]){
                c[2] = a[1];
                if(a[2] < b[1]){
                    c[3] = a[2];
                    c[4] = b[1];
                    c[5] = b[2]; 
                }else{
                    c[3] = b[1];
                    if(a[2] < b[2]){
                        c[4] = a[2];
                        c[5] = b[2];
                    }else{
                        c[4] = b[2];
                        c[5] = a[2];
                    }
                }
            }else{
                c[2] = b[1];
                if(b[2] < a[1]){
                    c[3] = b[2];
                    c[4] = a[1];
                    c[5] = a[2]; 
                }else{
                    c[3] = a[1];
                    if(a[2] < b[2]){
                        c[4] = a[2];
                        c[5] = b[2];
                    }else{
                        c[4] = b[2];
                        c[5] = a[2];
                    }
                }
            }
        }
    }else{
        c[0] = b[0];
        if(b[1] < a[0]){
            c[1] = b[1];
            if(b[2] < a[0]){
                c[2] = b[2];
                c[3] = a[0];
                c[4] = a[1];
                c[5] = a[2];
            }else{
                c[2] = a[0];
                if(b[2] < a[1]){
                    c[3] = b[2];
                    c[4] = a[1];
                    c[5] = a[2];
                }else{
                    c[3] = a[1];
                    if(a[2] < b[2]){
                        c[4] = a[2];
                        c[5] = b[2];
                    }else{
                        c[4] = b[2];
                        c[5] = a[2];
                    }
                }
            }
        }else{
            c[1] = a[0];
            if(a[1] < b[1]){
                c[2] = a[1];
                if(a[2] < b[1]){
                    c[3] = a[2];
                    c[4] = b[1];
                    c[5] = b[2]; 
                }else{
                    c[3] = b[1];
                    if(a[2] < b[2]){
                        c[4] = a[2];
                        c[5] = b[2];
                    }else{
                        c[4] = b[2];
                        c[5] = a[2];
                    }
                }
            }else{
                c[2] = b[1];
                if(b[2] < a[1]){
                    c[3] = b[2];
                    c[4] = a[1];
                    c[5] = a[2]; 
                }else{
                    c[3] = a[1];
                    if(a[2] < b[2]){
                        c[4] = a[2];
                        c[5] = b[2];
                    }else{
                        c[4] = b[2];
                        c[5] = a[2];
                    }
                }
            }
        }
    }
}

/*
      merge two ordered integer arrays
*/
void merge_i(uchar *a, int n, uchar *b, int m, uchar *c)
{
    uchar *ae = a + n, *be = b + m;
    while (a < ae && b < be)
    {
        if (*a < *b)
            *c++ = *a++;
        else
            *c++ = *b++;
    }
    while (a < ae)
        *c++ = *a++;
    while (b < be)
        *c++ = *b++;
}

/*
/	Find median from two ordered arrays a[3] and b[6]. Note that the median is the 5th element in the merged 
/	array, and we will use some tricks to reduce the number of comparissons from 1 (best case scenario) to 4 (worst case scenario)
*/
int median_from36(uchar a[], uchar b[])
{
    if (a[0] > b[4])
        return b[4];
    if (a[1] > b[3])
    { //median either a[0] or b[3]
        if (a[0] > b[3])
            return a[0];
        return b[3];
    }
    // Now we know that median has to be in a[0-2] or b[0-2].
    // If we merge this array it has six elements, and median is the fifth, i.e. the second biggest

    if (a[2] > b[2])
    { //a[2] is the biggest, so the median is either b[2] or a[1]
        if (a[1] >= b[2])
            return a[1];
        return b[2];
    } // b[2] is the biggest, so median is either a[2] or b[1];
    if (a[2] >= b[1])
        return a[2];
    return b[1];
}

void median2di(uchar **img, int m, int n, uchar **med)
{
    int i, j, k, kk;
    uchar vert[3], temp[3], c[8], *ptrSrc = img[0], *ptrDst = med[0];
    uchar a[4][3], b[2][6];
    *ptrDst++ = *ptrSrc;
    //    boundary conditions
    //    first row
    while (ptrSrc < img[0] + (n - 2))
    {
        sort3i(ptrSrc, temp);
        *ptrDst++ = temp[1];
        ptrSrc++;
    }
    *ptrDst = img[0][n - 1];
    //    last row
    ptrSrc = img[m - 1];
    ptrDst = med[m - 1];
    *ptrDst++ = *ptrSrc;
    while (ptrSrc < img[m - 1] + (n - 2))
    {
        sort3i(ptrSrc, temp);
        *ptrDst++ = temp[1];
        ptrSrc++;
    }
    *ptrDst = img[m - 1][n - 1];
    //    first and last column
    for (i = 1; i < m - 1; ++i)
    {
        vert[0] = img[i - 1][0];
        vert[1] = img[i][0];
        vert[2] = img[i + 1][0];
        sort3i(vert, temp);
        med[i][0] = temp[1];

        vert[0] = img[i - 1][n - 1];
        vert[1] = img[i][n - 1];
        vert[2] = img[i + 1][n - 1];
        sort3i(vert, temp);
        med[i][n - 1] = temp[1];
    }

    //  fill the rest of the matrix, two verticals, by two verticals
    for (j = 0; j < n - 2; j++)
    {
        sort3i(&img[0][j], a[0]);
        sort3i(&img[1][j], a[1]);
        merge_i_manually(a[0], 3, a[1], 3, b[0]);
        k = 2;
        kk = 1;
        for (i = 2; i < m - 1; i += 2)
        {
            sort3i(&img[i][j], a[k]);
            sort3i(&img[i + 1][j], a[k + 1]);
            merge_i_manually(a[k], 3, a[k + 1], 3, b[kk]);
            // get the first median
            med[i - 1][j + 1] = median_from36(a[(i)&0x3], b[1 - kk]);
            // get the second median
            med[i][j + 1] = median_from36(a[(i - 1) & 0x3], b[kk]);
            k = 2 - k;
            kk = 1 - kk;
        }
    }
}

bool validate(cv::Mat &med_opencv, uchar **med)
{
    int m = med_opencv.rows;
    int n = med_opencv.cols;
    for (int i = 1; i < m - 1; ++i)
    {
        for (int j = 1; j < n - 1; ++j)
        {
            if (med[i][j] != med_opencv.at<uchar>(i, j))
            {
                return false;
            }
        }
    }
    return true;
}

int main()
{
    int n = 320;
    int m = 240;
    std::clock_t start, end;
    uchar **img = (uchar **)malloc(n * sizeof(uchar *));

    Mat A(m, n, CV_8UC1);
    randu(A, Scalar::all(0), Scalar::all(255));
    //     memcpy(A.data, img, n*n*sizeof(char));

    Mat med_opencv;
    start = std::clock();
    medianBlur(A, med_opencv, 3);
    std::cout << "Time OpenCV: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;

    for (int i = 0; i < m; i++)
    {
        img[i] = (uchar *)malloc(n * sizeof(uchar));
        for (int j = 0; j < n; j++)
        {
            img[i][j] = A.at<uchar>(i, j);
        }
    }

    uchar **med = (uchar **)malloc(n * sizeof(uchar *));
    for (int i = 0; i < m; i++)
    {
        med[i] = (uchar *)malloc(n * sizeof(uchar));
    }
    start = std::clock();
    median2di(img, m, n, med);
    end = std::clock();
    std::cout << "Time median2di: " << (end - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;

    bool isSame = validate(med_opencv, med);

    //Printing Logic
    if (n <= 20)
    {
        //OpenCV matrix
        cout << A << endl;

        cout << "----------------" << endl;

        //OpenCV median matrix
        cout << med_opencv << endl;

        cout << "----------------" << endl;

        //C++ 2D matrix
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                cout << img[i][j] << " ";
            }
            cout << endl;
        }

        cout << "----------------" << endl;
        /// C++ median 2D matrix
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                cout << med[i][j] << " ";
            }
            cout << endl;
        }
    }
    if (isSame)
        cout << "Median matrix using both the algorithms is the same" << endl;
    else
        cout << "Algorithm fails" << endl;
    return 0;
}