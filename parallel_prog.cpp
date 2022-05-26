#include <omp.h>
#include <iostream>
#include <Magick++.h>
#include <string>
#include <vector>
#include <intrin.h>
#include <chrono>

using namespace std;
using namespace Magick;

void extract_grain() {
    
}

int main()
{
    int start_time = clock();
    string file1 = "950x950_1.png";
    string file2 = "950x950_2.png";
    Image img1, img2;
    int row1, col1, row2, col2;
    try {
        img1.read(file1);
        img2.read(file2);

        img1.modifyImage();
        img2.modifyImage();

        img1.type(TrueColorType);
        img2.type(TrueColorType);

        img1.modifyImage();
        img2.modifyImage();


        row1 = img1.rows();
        col1 = img1.columns();
        row2 = img2.rows();
        col2 = img2.columns();

        Pixels view1(img1);
        Quantum* pixels1 = view1.get(0, 0, col1, row1);

        Pixels view2(img2);
        Quantum* pixels2 = view2.get(0, 0, col2, row2);

        
        //последовательно
        for (ssize_t i = 0; i < row1 * col1 * 3; i++) {
          //pixels1[i] = pixels1[i] - pixels2[i] + 128;
          //pixels1[i] = pixels1[i] + pixels2[i] - 128;
          pixels1[i] = min(pixels1[i] + pixels2[i],255);
          //pixels1[i] = max(pixels1[i] - pixels2[i],0);
        }

        //OpenMP
        /*ssize_t i;
        #pragma omp parallel for shared(pixels1, pixels2, row1, col1) private(i)
        for (i = 0; i < row1 * col1*3; ++i){
            pixels1[i] = pixels1[i] - pixels2[i] + 128;
        }
        */

        //векторизация
        /*vector < Quantum > arr1(8);
        vector < Quantum > arr2(8);
        for (ssize_t i = 0; i < row1 * col1 * 3; i += 8) {
            for (int j = 0; j < 8; j++) {
                arr1[j] = pixels1[i + j];
                arr2[j] = pixels2[i + j];
            }
            arr1[0] = arr1[0] - arr2[0] + 128;
            arr1[1] = arr1[1] - arr2[1] + 128;
            arr1[2] = arr1[2] - arr2[2] + 128;
            arr1[3] = arr1[3] - arr2[3] + 128;
            arr1[4] = arr1[4] - arr2[4] + 128;
            arr1[5] = arr1[5] - arr2[5] + 128;
            arr1[6] = arr1[6] - arr2[6] + 128;
            arr1[7] = arr1[7] - arr2[7] + 128;
            for (int j = 0; j < 8; j++) {
                pixels1[i + j] = arr1[j];
                pixels2[i + j] = arr2[j];
            }
        }
        */
        
        

      


        /*ssize_t i;
        //#pragma omp parallel for shared(pixels1, pixels2, row1, col1) private(i)
        for (i = 0; i < row1 * col1 * 3; i += 8) {
          
            __m256i  meow1 = _mm256_loadu_epi8(pixels1 + i);
            __m256i  meow2 = _mm256_loadu_epi8(pixels2 + i);
            

            //meow1 = _mm256_adds_epu8(meow1, meow2);
            meow1 = _mm256_subs_epu8(meow1, meow2);
            //
            _mm256_storeu_epi8(pixels1 + i, meow1);
        }
        */



        view1.sync();
        img1.write("11.png");
        
        int last_time = clock();

        cout << last_time - start_time;
    }
    catch (Magick::Exception& error_) {
        std::cout << error_.what() << std::endl;
    }
}

