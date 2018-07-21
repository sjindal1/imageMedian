#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

void print_a(int *a)
{
      cout << "------" << endl;
      for (int i = 0; i < 3; i++)
            cout << a[i] << " ";
      cout << endl;
}

void print_b(int *a)
{
      cout << "------" << endl;
      for (int i = 0; i < 6; i++)
            cout << a[i] << " ";
      cout << endl;
}

void print_c(int *a)
{
      cout << "------" << endl;
      for (int i = 0; i < 8; i++)
            cout << a[i] << " ";
      cout << endl;
}

void sort3i(int *a, int *b)
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
void merge_i(int *a, int n, int *b, int m, int *c)
{
      int *ae = a + n, *be = b + m;
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

void median2di(int **img, int m, int n, int **med)
{
      int i, j, k, kk;
      int vert[3], temp[3], c[8], *ptrSrc = img[0], *ptrDst = med[0];
      int a[4][3], b[2][6];
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
            merge_i(a[0], 3, a[1], 3, b[0]);
            k = 2;
            kk = 1;
            for (i = 2; i < m - 1; i += 2)
            {
                  sort3i(&img[i][j], a[k]);
                  sort3i(&img[i + 1][j], a[k + 1]);
                  merge_i(a[k], 3, a[k + 1], 3, b[kk]);
                  // get the first median
                  merge_i(b[1 - kk], 5, a[(i)&0x3], 3, c);
                  med[i - 1][j + 1] = c[4];
                  // get the second median
                  merge_i(b[kk], 5, a[(i - 1) & 0x3], 3, c);
                  med[i][j + 1] = c[4];
                  k = 2 - k;
                  kk = 1 - kk;
            }
      }
}

bool validate(cv::Mat &med_opencv, int** med){
      int m = med_opencv.rows;
      int n = med_opencv.cols;
      for(int i = 1; i < m -1; ++i){
            for(int j = 1; j < n -1; ++j){
                  if(med[i][j] != med_opencv.at<uchar>(i,j)){
                        return false;
                  }
            }
      }
      return true;
}

int main()
{
      int n = 10000;
      int **img = (int **)malloc(n * sizeof(int *));
      cout << "start " << endl;

      Mat A(n, n, CV_8UC1);
      randu(A, Scalar::all(0), Scalar::all(255));
      //     memcpy(A.data, img, n*n*sizeof(char));

      Mat med_opencv;
      medianBlur(A, med_opencv, 3);

      for (int i = 0; i < n; i++)
      {
            img[i] = (int *)malloc(n * sizeof(int));
            for (int j = 0; j < n; j++)
            {
                  img[i][j] = A.at<uchar>(i, j);
            }
      }

      int **med = (int **)malloc(n * sizeof(int *));
      for (int i = 0; i < n; i++)
      {
            med[i] = (int *)malloc(n * sizeof(int));
      }
      median2di(img, n, n, med);

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
            for (int i = 0; i < n; i++)
            {
                  for (int j = 0; j < n; j++)
                  {
                        cout << img[i][j] << " ";
                  }
                  cout << endl;
            }

            cout << "----------------" << endl;
            /// C++ median 2D matrix
            for (int i = 0; i < n; i++)
            {
                  for (int j = 0; j < n; j++)
                  {
                        cout << med[i][j] << " ";
                  }
                  cout << endl;
            }
      }
      if(isSame)
            cout << "Median matrix using both the algorithms is the same" << endl;
      else  
            cout << "Algorithm fails" << endl;
      return 0;
}