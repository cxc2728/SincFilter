#define _CRT_SECURE_NO_WARNINGS
// name of the project: ImageReconstruction2024
#include <iostream>
#include <fstream>
#include <string>
#include <io.h>
#include <dos.h>
#include <conio.h>
#include <stdlib.h>
#include <sstream>
#include <stdio.h>
#include <iomanip>
#include <istream>
#include <math.h>

using namespace std;

#define ft_SCALE 1;

const double z_f = 0.1;

unsigned char filterType = 'l';
unsigned char filter = 'b';
unsigned char filter_kz = 'k';

const double ft_SCALE_z = 1.0;

void OnFourierTransform_scale(char imageFilename[], int rcxres, int rcyres);
void OnZTransform_scale(char imageFilename[], int rcxres, int rcyres, double m_Real, double m_Imaginary);

void OnTransferFunctionBesselKspace(char filterType, int rcxres, int rcyres, int n_poles, double m_cof, double m_Bandwidth, double m_TheSamplingRate);
void OnTransferFunctionButterworthKspace(char filterType, int m_rcxres, int m_rcyres, int n_poles, double m_cof, double Gain, double m_Bandwidth, double m_TheSamplingRate);
void OnTransferFunctionChebyshevKspace(char filterType, int rcxres, int rcyres, int n_poles, double m_cof, double m_eps, double m_Bandwidth, double m_TheSamplingRate);

void OnTransferFunctionBesselZspace(char filterType, int rcxres, int rcyres, int n_poles, double m_cof, double m_Bandwidth, double m_TheSamplingRate, double m_Real, double m_Imaginary);
void OnTransferFunctionButterworthZspace(char filterType, int rcxres, int rcyres, int n_poles, double m_cof, double Gain, double m_Bandwidth, double m_TheSamplingRate, double m_Real, double m_Imaginary);
void OnTransferFunctionChebyshevZspace(char filterType, int rcxres, int rcyres, int n_poles, double m_cof, double m_eps, double m_Bandwidth, double m_TheSamplingRate, double m_Real, double m_Imaginary);

void OnInverseFourierTransformTransferFunctionKspace(int rcyres, int rcxres);
void OnInverseZTransformTransferFunction(int rcyres, int rcxres, double m_Real, double m_Imaginary);

void OnFrequencyDomain(char filterType, char filter_kz, char imageFilename[], int rcxres, int rcyres, double m_Real, double m_Imaginary, int m_angles, double Sinc_SamplingRate);

// declare a class by the name 'ImageReconstruction2024'
class ImageReconstruction2024 {

	// the point is to assign to 'n1' and 'n2' 
	// the correct values from the command console
	int n1; // matrix size x (number of pixels along the x direction)
	int n2; // matrix size y (number of pixels along the y direction)

	// declare the class methods
	// and embed the methods body
	// in the class specification
public:

	// declare a method that returns the number
	// of pixels of the image along the x direction
	int getNofPixelsX(void) { return this->n1; };

	// declare a method that returns the number
	// of pixels of the image along the y direction
	int getNofPixelsY(void) { return this->n2; };

	// declare a method that sets the number
	// of pixels of the image along the x direction
	void setNofPixelsX(int x) { this->n1 = x; };

	// declare a method that sets the number 
	// of pixels of the image along the y direction
	void setNofPixelsY(int y) { this->n2 = y; };

public:
	// declare a structure ' data ' that defines the
	// pointers to pointers to the image
	struct data {

		double** Signal; // declare the pointer to pointer to the matrix (image) 

	}*pointer; // pointer to the element of the structure 'data'
	// the pointer points to the memory address of the
	// pointers to pointers 

public:

	// constructor of the class 'ImageReconstruction2024'
	ImageReconstruction2024(int x, int y) : n1(x), n2(y) { pointer = 0; };

	// declare the prototype of the 
	// function 'allocateData'
	// the function belongs to the 
	// sets of methods of the class 'ImageReconstruction2024'
	void allocateData();

	// destructor of the class 'ImageReconstruction2024'
	~ImageReconstruction2024() { }

};

void ImageReconstruction2024::allocateData() { // allocate data


	// (1) allocate struct 'data' (begin)
	// define a pointer by the name 'pointer'
	// and assign to it the instance of the
	// structure 'data' (the instance is a
	// memory address
	pointer = new data;

	// assign to the pointer to a pointer 'Signal' (pointer->Signal)
	// the instance of the memory address of the pointer to pointer
	// *[this->n2]. Practically define the memory address of the 
	// rows of the matrix containing the image 'Signal'.
	pointer->Signal = new double* [this->n2];


	for (int v = 0; v < this->n2; v++) { // (1)
		// at each iteration of the for loop
		// assign to the pointer 'Signal[v]' the instance 
		// of the memory address of the pointer [this-n1].
		// Practically define the memory address of the 
		// columns of the matrices containing the image:
		// 'Signal.

		pointer->Signal[v] = new double[this->n1];

	} // (1) allocate struct 'data' (end)


  // (2) initialize (begin)
	for (int v = 0; v < this->n2; v++) { // (a)

		for (int f = 0; f < this->n1; f++) { // (b)

			// at each iteration of the two for loops
			// initializes the value of the pixel of
			// the image to zero. This is done for the
			// image 'Signal'.
			pointer->Signal[v][f] = (double)0.0;

		} //(b)

	} //(a)
   // (2) initialize (end)

} // allocate data

// read the input parameters from the
// console command line
int main(int argc, char* argv[]) {

	// assign to the char string 'outputFile'
	// the value "ImageReconstruction2024.log"
	char outputFile[128] = "ImageReconstruction2024.log";

	// declare a pointer to a file by the
	// name 'savedata'
	FILE* savedata;

	// tell the user of the list of input parameters necessary to tun the program:
	if (argc < 16) {

		std::cout << endl;
		std::cout << "Please type the image file name" << endl;
		std::cout << "Please make sure that the image format is Analyze 'double': 64 bits real" << endl;
		std::cout << "Please enter the number of pixels along the X direction (integer)" << endl;
		std::cout << "Please enter the number of pixels along the Y direction (integer)" << endl;

		std::cout << "Enter 'l' to calculate LPF or 'h' to calculate HPF" << endl;

		std::cout << "The cut-off frequency as double precision number in the range: [2.45, 2.9]" << endl;
		std::cout << "The number of poles of the filter, (int): 4, 5, or 6" << endl;
		std::cout << "The Bandwidth (double)" << endl;
		std::cout << "The Sampling Rate (double)" << endl;

		std::cout << "Enter 'b' to calculate Bessel or 'w' to calculate Butterworth or 'c' to calculate Chebyshev" << endl;
		std::cout << "The gain of the filter, (double) in: ]0.0, 1.0]" << endl;
		std::cout << "Enter 'k' to k-space filter or 'z' to Z-space filter" << endl;

		std::cout << "The value of the real part of the complex number, (double) in: [0.01, 1]" << endl;
		std::cout << "The value of the imaginary part of the complex number, (double) in: [0, 1]" << endl;

		std::cout << "Enter the number of angles used to calculate the SincFilter (int) [1, 10]" << endl;

		std::cout << "The Sampling Rate the Sinc function (double) in: [1.0, 3.0]" << endl;

		std::cout << "Example parameters for LPF: l 2.5 5 0.0287 0.287 b 0.5 k 0.5 0.5 5 0.8" << endl;
		std::cout << "Example parameters for HPF: h 2.5 5 0.0287 0.287 b 0.5 k 0.5 0.5 5 0.8" << endl;
		std::cout << endl;
		exit(0);
	}

	else { // run the program (begin)

		// opens the log file which name is 
		// contained in 'outputFile', opens 
		// in write mode
		if ((savedata = fopen(outputFile, "w")) == NULL)
		{
			// alert the user of possible failure in opening the file
			std::cout << "Cannot open output file, Now Exit..." << endl;
			exit(0);

		}
		else { // processing (begin)

			// declare an array of char to contain a string
			char imageFileName[128];

			// transfer into the array 'imageFileName'
			// the image file name as per input from
			// the command console. The image file name
			// is 'argv[1]'
			sprintf(imageFileName, "%s", argv[1]);

			// reads from the command console the
			// value of the image size (number of rows
			// and number of columns)
			int n1 = atoi(argv[2]);
			int n2 = atoi(argv[3]);

			char filterType = *argv[4];
			double m_cof = atof(argv[5]);
			int n_poles = atoi(argv[6]);

			double m_eps = (double)0.5; // fixed for better performance

			double m_Bandwidth = atof(argv[7]);
			double m_TheSamplingRate = atof(argv[8]);

			char filter = *argv[9];
			double Gain = atof(argv[10]);

			char filter_kz = *argv[11];

			double m_Real = atof(argv[12]);
			double m_Imaginary = atof(argv[13]);

			int m_angles = atoi(argv[14]);

			double Sinc_SamplingRate = atof(argv[15]);


			if ( (double)Sinc_SamplingRate < 1.0 || (double)Sinc_SamplingRate > 3.0 )
			{
			
				std::cout << "Enter the Sampling Rate the Sinc function (double) in: [1.0, 3.0]" << endl;
				exit(0);

			}


			if ((int)m_angles < 1 || (int)m_angles > 10)
			{

				std::cout << "Enter the number of angles used to calculate the SincFilter (int) [1, 10]" << endl;
				exit(0);
			}


			if (tolower(filterType) != 'l' && tolower(filterType) != 'h')
			{

				std::cout << "Enter 'l' to calculate LPF or 'h' to calculate HPF" << endl;
				exit(0);
			}


			if (m_Real < (double)0.01 || m_Real >(double) 1.0)
			{

				std::cout << "To use the filter please enter" << endl;
				std::cout << "the real part of the complex number, (double) in: [0.01, 1]" << endl;
				std::cout << "The suggested value is 0.5" << endl;
				exit(0);

			}

			if (m_Imaginary < (double)0.0 || m_Imaginary >(double) 1.0)
			{

				std::cout << "To use the filter please enter" << endl;
				std::cout << "the imaginary part of the complex number, (double) in: [0, 1]" << endl;
				std::cout << "The suggested value is 0.5" << endl;
				exit(0);

			}


			if (tolower(filter_kz) == 'k')
			{ // if one of the three k-space filters is chosen

				if (tolower(filter) == 'c')
				{ // if Chebyshev

					if (m_cof < (double)2.45 || m_cof >(double) 2.9)
					{

						std::cout << "To use the filter please enter" << endl;
						std::cout << "the cut-off frequency as double precision number in the range: [2.45, 2.9]" << endl;
						exit(0);

					}


					if (n_poles != (int)4 && n_poles != (int)5 && n_poles != (int)6)
					{

						std::cout << "To use the filter please enter" << endl;
						std::cout << "the number of poles as an integer: 4, 5, or 6" << endl;
						exit(0);

					}

					if (n_poles == (int)4)
					{
						if ((double)m_cof == (double)2.55
							) {

						}
						else {

							std::cout << "Please reset the Cut-Off frequency to 2.55" << endl;
							exit(0);
						}

					}
					else if (n_poles == (int)5)

					{
						if ((double)m_cof == (double)2.5 ||
							(double)m_cof == (double)2.45 ||
							(double)m_cof == (double)2.455 ||
							(double)m_cof == (double)2.87 ||
							(double)m_cof == (double)2.9 ||
							(double)m_cof == (double)2.78
							) {

						}
						else {

							std::cout << "Please reset the Cut-Off frequency to: " << endl;
							std::cout << "2.5, 2.45, 2.455, 2.78, 2.87, or 2.9" << endl;
							exit(0);
						}
					}
					else if (n_poles == (int)6)

					{
						if ((double)m_cof == (double)2.45 ||
							(double)m_cof == (double)2.455 ||
							(double)m_cof == (double)2.5 ||
							(double)m_cof == (double)2.55 ||
							(double)m_cof == (double)2.8 ||
							(double)m_cof == (double)2.87 ||
							(double)m_cof == (double)2.9 ||
							(double)m_cof == (double)2.78
							) {

						}
						else {

							std::cout << "Please reset the Cut-Off frequency to: " << endl;
							std::cout << "2.5, 2.55, 2.45, 2.455, 2.8, 2.78, 2.87, or 2.9" << endl;
							exit(0);
						}
					}

				} // if Chebyshev

				else if (tolower(filter) == 'w')
				{ // if Butterworth

					if (m_cof < (double)2.5 || m_cof >(double) 2.8)
					{

						std::cout << "To use the filter please enter" << endl;
						std::cout << "the cut-off frequency as double precision number in the range: [2.5, 2.8]" << endl;
						exit(0);

					}

					if (n_poles != (int)3 && n_poles != (int)5 && n_poles != (int)7)
					{

						std::cout << "To use the filter please enter" << endl;
						std::cout << "the number of poles as an integer: 3, 5, or 7" << endl;
						exit(0);

					}

					if (n_poles == (int)3)
					{
						if (m_cof == (double)2.5 ||
							m_cof == (double)2.6 ||
							m_cof == (double)2.65 ||
							m_cof == (double)2.7 ||
							m_cof == (double)2.8
							) {
						}

						else {

							std::cout << "Please reset the Cut-Off frequency to:" << endl;
							std::cout << "2.5, 2.6, 2.65, 2.7, or 2.8" << endl;
							exit(0);

						}

					}

					else if (n_poles == (int)5)
					{

						if (m_cof == (double)2.5 ||
							m_cof == (double)2.55 ||
							m_cof == (double)2.6 ||
							m_cof == (double)2.65 ||
							m_cof == (double)2.7 ||
							m_cof == (double)2.8
							) {
						}

						else {

							std::cout << "Please reset the Cut-Off frequency to:" << endl;
							std::cout << "2.55, 2.6, 2.65, 2.7, or 2.8" << endl;
							exit(0);

						}

					}

					else if (n_poles == (int)7)
					{

						if (m_cof == (double)2.5 ||
							m_cof == (double)2.55 ||
							m_cof == (double)2.6 ||
							m_cof == (double)2.65 ||
							m_cof == (double)2.8
							) {
						}

						else {

							std::cout << "Please reset the Cut-Off frequency to:" << endl;
							std::cout << "2.5, 2.55, 2.6, 2.65, or 2.8" << endl;
							exit(0);

						}

					}

					if (Gain <= (double)0.0 || Gain > (double)1.0)
					{

						std::cout << "To use the filter please enter" << endl;
						std::cout << "the gain of the filter, (double) in: ]0.0, 1.0]" << endl;
						std::cout << "The suggested value is 1.0" << endl;
						exit(0);

					}

				} // if Butterworth
				else if (tolower(filter) == 'b')
				{ // if Bessel

					if (m_cof < (double)2.5 || m_cof >(double) 2.8)
					{

						std::cout << "To use the filter please enter" << endl;
						std::cout << "the cut-off frequency as double precision number in the range: [2.5, 2.8]" << endl;
						exit(0);

					}

					if (n_poles < (int)2 || n_poles >(int) 5)
					{

						std::cout << "To use the filter please enter" << endl;
						std::cout << "the number of poles as an integer in the range: [2, 5]" << endl;
						exit(0);

					}

					if (n_poles == (int)2)
					{
						if (m_cof == (double)2.5 ||
							m_cof == (double)2.6 ||
							m_cof == (double)2.7 ||
							m_cof == (double)2.8
							) {
						}

						else {

							std::cout << "Please reset the Cut-Off frequency to:" << endl;
							std::cout << "2.5, 2.6, 2.7, or 2.8" << endl;
							exit(0);

						}

					}

					else if (n_poles == (int)3)
					{

						if (m_cof == (double)2.5 ||
							m_cof == (double)2.6 ||
							m_cof == (double)2.7 ||
							m_cof == (double)2.8
							) {
						}

						else {

							std::cout << "Please reset the Cut-Off frequency to:" << endl;
							std::cout << "2.5, 2.6, 2.7, or 2.8" << endl;
							exit(0);

						}

					}

					else if (n_poles == (int)4)
					{

						if (m_cof == (double)2.5 ||
							m_cof == (double)2.6 ||
							m_cof == (double)2.7 ||
							m_cof == (double)2.8
							) {
						}

						else {

							std::cout << "Please reset the Cut-Off frequency to:" << endl;
							std::cout << "2.5, 2.6, 2.7, or 2.8" << endl;
							exit(0);

						}

					}

					else if (n_poles == (int)5)
					{

						if (m_cof == (double)2.5 ||
							m_cof == (double)2.6 ||
							m_cof == (double)2.7 ||
							m_cof == (double)2.8
							) {
						}

						else {

							std::cout << "Please reset the Cut-Off frequency to:" << endl;
							std::cout << "2.5, 2.6, 2.7, or 2.8" << endl;
							exit(0);

						}

					}

				} // if Bessel


			} // if one of the three k-space filters is chosen

			else if (tolower(filter_kz) == 'z')
			{ // if one of the three z-space filters is chosen

				if (tolower(filter) == 'b')
				{ // if Bessel  

					if (n_poles < (int)2 || n_poles >(int) 5)
					{

						std::cout << "To use the filter please enter" << endl;
						std::cout << "the number of poles as an integer in the range: [2, 5]" << endl;
						exit(0);

					}

					if (n_poles == (int)2)
					{
						if (m_cof == (double)2.5 ||
							m_cof == (double)2.6 ||
							m_cof == (double)2.7 ||
							m_cof == (double)2.8
							) {
						}

						else {

							std::cout << "Please reset the Cut-Off frequency to:" << endl;
							std::cout << "2.5, 2.6, 2.7, or 2.8" << endl;
							exit(0);

						}

					}

					else if (n_poles == (int)3)
					{

						if (m_cof == (double)2.5 ||
							m_cof == (double)2.6 ||
							m_cof == (double)2.7 ||
							m_cof == (double)2.8
							) {
						}

						else {

							std::cout << "Please reset the Cut-Off frequency to:" << endl;
							std::cout << "2.5, 2.6, 2.7, or 2.8" << endl;
							exit(0);

						}

					}

					else if (n_poles == (int)4)
					{

						if (m_cof == (double)2.5 ||
							m_cof == (double)2.6 ||
							m_cof == (double)2.7 ||
							m_cof == (double)2.8
							) {
						}

						else {

							std::cout << "Please reset the Cut-Off frequency to:" << endl;
							std::cout << "2.5, 2.6, 2.7, or 2.8" << endl;
							exit(0);

						}

					}

					else if (n_poles == (int)5)
					{

						if (m_cof == (double)2.5 ||
							m_cof == (double)2.6 ||
							m_cof == (double)2.7 ||
							m_cof == (double)2.8
							) {
						}

						else {

							std::cout << "Please reset the Cut-Off frequency to:" << endl;
							std::cout << "2.5, 2.6, 2.7, or 2.8" << endl;
							exit(0);

						}

					}


				}
				else 	if (tolower(filter) == 'c')
				{  // if Chebyshev

					if (m_cof < (double)2.45 || m_cof >(double) 2.7)
					{

						std::cout << "To use the filter please enter" << endl;
						std::cout << "the cut-off frequency as double precision number in the range: [2.45, 2.7]" << endl;
						exit(0);

					}


					if (n_poles != (int)4 && n_poles != (int)5 && n_poles != (int)6 && n_poles != (int)8)
					{

						std::cout << "To use the filter please enter" << endl;
						std::cout << "the number of poles as an integer: 4, 5, 6, or 8" << endl;
						exit(0);

					}

					if (n_poles == (int)4)
					{
						if ((double)m_cof == (double)2.55
							) {

						}
						else {

							std::cout << "Please reset the Cut-Off frequency to 2.55" << endl;
							exit(0);
						}

					}
					else if (n_poles == (int)5)

					{
						if ((double)m_cof == (double)2.5 ||
							(double)m_cof == (double)2.45 ||
							(double)m_cof == (double)2.455
							) {

						}
						else {

							std::cout << "Please reset the Cut-Off frequency to: " << endl;
							std::cout << "2.45, or 2.455" << endl;
							exit(0);
						}
					}
					else if (n_poles == (int)6)

					{
						if ((double)m_cof == (double)2.45 ||
							(double)m_cof == (double)2.455 ||
							(double)m_cof == (double)2.5 ||
							(double)m_cof == (double)2.55
							) {

						}
						else {

							std::cout << "Please reset the Cut-Off frequency to: " << endl;
							std::cout << "2.45 or 2.455 or 2.5 or 2.55" << endl;
							exit(0);
						}
					}
				}  // if Chebyshev

				else if (tolower(filter) == 'w')
				{ // if Butterworth

					if (m_cof < (double)2.5 || m_cof >(double) 2.8)
					{

						std::cout << "To use the filter please enter" << endl;
						std::cout << "the cut-off frequency as double precision number in the range: [2.5, 2.8]" << endl;
						exit(0);

					}

					if (n_poles != (int)3 && n_poles != (int)5 && n_poles != (int)7)
					{

						std::cout << "To use the filter please enter" << endl;
						std::cout << "the number of poles as an integer: 3, 5, or 7" << endl;
						exit(0);

					}

					if (n_poles == (int)3)
					{
						if (m_cof == (double)2.5 ||
							m_cof == (double)2.6 ||
							m_cof == (double)2.65 ||
							m_cof == (double)2.7 ||
							m_cof == (double)2.8
							) {
						}

						else {

							std::cout << "Please reset the Cut-Off frequency to:" << endl;
							std::cout << "2.5, 2.6, 2.65, 2.7, or 2.8" << endl;
							exit(0);

						}

					}

					else if (n_poles == (int)5)
					{

						if (m_cof == (double)2.5 ||
							m_cof == (double)2.55 ||
							m_cof == (double)2.6 ||
							m_cof == (double)2.65 ||
							m_cof == (double)2.7 ||
							m_cof == (double)2.8
							) {
						}

						else {

							std::cout << "Please reset the Cut-Off frequency to:" << endl;
							std::cout << "2.55, 2.6, 2.65, 2.7, or 2.8" << endl;
							exit(0);

						}

					}

					else if (n_poles == (int)7)
					{

						if (m_cof == (double)2.5 ||
							m_cof == (double)2.55 ||
							m_cof == (double)2.6 ||
							m_cof == (double)2.65 ||
							m_cof == (double)2.8
							) {
						}

						else {

							std::cout << "Please reset the Cut-Off frequency to:" << endl;
							std::cout << "2.5, 2.55, 2.6, 2.65, or 2.8" << endl;
							exit(0);

						}

					}


					if (Gain <= (double)0.0 || Gain > (double)1.0)
					{

						std::cout << "To use the filter please enter" << endl;
						std::cout << "the gain of the filter, (double) in: ]0.0, 1.0]" << endl;
						std::cout << "The suggested value is 1.0" << endl;
						exit(0);

					}

				} // if Butterworth

			} // if one of the three z-space filters is chosen


			if ((m_TheSamplingRate > 2.0 * m_Bandwidth))
			{ // if m_TheSamplingRate > 2.0*m_Bandwidth

			} // if m_TheSamplingRate > 2.0*m_Bandwidth
			else {

				std::cout << "The Sampling Rate must be >= 2.0 * Bandwidth" << endl;
				exit(0);

			}

			// inform the user of the image size 
			// (number of rows and number of columns
			// of the matrix containing the image)
			std::cout << endl;
			std::cout << "The number of pixels along the X direction is: " << atoi(argv[2]) << endl;
			std::cout << "The number of pixels along the Y direction is: " << atoi(argv[3]) << endl;
			std::cout << "The number of poles of the filter is: " << n_poles << endl;
			std::cout << "The value of the ripple factor is: " << m_eps << endl;
			std::cout << "The Bandwidth is: " << m_Bandwidth << endl;
			std::cout << "The Sampling Rate is: " << m_TheSamplingRate << endl;
			std::cout << "The cut-off frequency is: " << m_cof << endl;
			std::cout << "The value of m_Real is: " << m_Real << endl;
			std::cout << "The value of m_Imaginary is: " << m_Imaginary << endl;
			std::cout << "The value of m_angles is: " << m_angles << endl;
			std::cout << "The Sampling Rate of the Sinc function is: " << Sinc_SamplingRate << endl;

			double pi = 3.141592;
			double Zphase = (double)2.0 * pi * atan2((double)m_Imaginary, (double)m_Real) / (n1 * n2);
			double magnitude = (double)sqrt((double)m_Real * m_Real + (double)m_Imaginary * m_Imaginary);

			std::cout << "The magnitude of the Complex number is: " << magnitude << endl;
			std::cout << "The phase of the Complex number is: " << Zphase << endl;


			// save into the log file the image size 
			// (number of rows and number of columns
			// of the matrix containing the image)
			fprintf(savedata, "%s%d\n", "The number of pixels along the X direction is: ", n1);
			fprintf(savedata, "%s%d\n", "The number of pixels along the Y direction is: ", n2);
			fprintf(savedata, "%s%lf\n", "The cut-off frequency is: ", m_cof);
			fprintf(savedata, "%s%d\n", "The number of poles of the filter is: ", n_poles);
			fprintf(savedata, "%s%lf\n", "The value of the ripple factor is: ", m_eps);
			fprintf(savedata, "%s%lf\n", "The Bandwidth is: ", m_Bandwidth);
			fprintf(savedata, "%s%lf\n", "The Sampling Rate is: ", m_TheSamplingRate);
			fprintf(savedata, "%s%lf\n", "The value of m_Real is: ", m_Real);
			fprintf(savedata, "%s%lf\n", "The value of m_Imaginary is: ", m_Imaginary);
			fprintf(savedata, "%s%lf\n", "The magnitude of the Complex number is: ", magnitude);
			fprintf(savedata, "%s%lf\n", "The phase of the Complex number is: ", Zphase);
			fprintf(savedata, "%s%lf\n", "The Gain is set to: ", Gain);
			fprintf(savedata, "%s%d\n", "The value of m_angles is set to: ", m_angles);

			fprintf(savedata, "%s%lf\n", "The Sampling Rate of the Sinc function is: ", Sinc_SamplingRate);

			fprintf(savedata, "%s%c\n", "The filter is: ", filterType);
			fprintf(savedata, "%s%c\n", "The filter is: ", filter_kz);
			fprintf(savedata, "%s%c\n", "The filter is: ", filter);


			if (tolower(filterType) == 'l')
				std::cout << "The program applies a low pass filter to the Image Data " << filterType << endl;
			else if (tolower(filterType) == 'h')
				std::cout << "The program applies a high pass filter to the Image Data " << filterType << endl;

			if (tolower(filter_kz) == 'k')
				std::cout << "The program applies the k-Space Filter to the Image Data " << filter_kz << endl;
			else if (tolower(filter_kz) == 'z')
				std::cout << "The program applies the Z-Space Filter to the Image Data " << filter_kz << endl;

			if (tolower(filter) == 'b')
				std::cout << "The program applies the Bessel Filter to the Image Data " << filter << endl;
			else if (tolower(filter) == 'c')
				std::cout << "The program applies the Chebyshev Filter to the Image Data " << filter << endl;
			else if (tolower(filter) == 'w')
				std::cout << "The program applies the Butterworth Filter to the Image Data " << filter << endl;


			if (tolower(filterType) == 'l')
				fprintf(savedata, "%s\n", "The program applies the low pass Filter to the Image Data");
			else if (tolower(filterType) == 'h')
				fprintf(savedata, "%s\n", "The program applies the high pass Filter to the Image Data");

			if (tolower(filter_kz) == 'k')
				fprintf(savedata, "%s\n", "The program applies the k-Space Filter to the Image Data");
			else if (tolower(filter_kz) == 'z')
				fprintf(savedata, "%s\n", "The program applies the Z-Space Filter to the Image Data");

			if (tolower(filter) == 'b')
				fprintf(savedata, "%s\n", "The program applies the Bessel Filter to the Image Data");
			else if (tolower(filter) == 'c')
				fprintf(savedata, "%s\n", "The program applies the ChebyShev Filter to the Image Data");
			else if (tolower(filter) == 'w')
				fprintf(savedata, "%s\n", "The program applies the Butterworth Filter to the Image Data");




			// call to the constructor 'ImageReconstruction' so to create
			// an object of type 'ImageReconstruction'. The data type of 'ImageReconstruction'
			// is 'ImageReconstruction2024'
			ImageReconstruction2024 ImageReconstruction(n1, n2);

			// the object of type 'ImageReconstruction' 
			// sends a message (invokes)
			// to the method 'allocateData()'
			ImageReconstruction.allocateData();

			OnFrequencyDomain(filterType, filter_kz, imageFileName, n1, n2, m_Real, m_Imaginary, m_angles, Sinc_SamplingRate);

			// TF (begin)
			std::cout << "Now calculating the Transfer Function (TF)..." << endl;

			if (tolower(filter_kz) == 'k')
			{

				if (tolower(filter) == 'b')
					OnTransferFunctionBesselKspace(filterType, n1, n2, n_poles, m_cof, m_Bandwidth, m_TheSamplingRate);

				else if (tolower(filter) == 'w')
					OnTransferFunctionButterworthKspace(filterType, n1, n2, n_poles, m_cof, Gain, m_Bandwidth, m_TheSamplingRate);

				else if (tolower(filter) == 'c')
					OnTransferFunctionChebyshevKspace(filterType, n1, n2, n_poles, m_cof, m_eps, m_Bandwidth, m_TheSamplingRate);

			}
			else if (tolower(filter_kz) == 'z')
			{
				if (tolower(filter) == 'b')
					OnTransferFunctionBesselZspace(filterType, n1, n2, n_poles, m_cof, m_Bandwidth, m_TheSamplingRate, m_Real, m_Imaginary);

				else if (tolower(filter) == 'w')
					OnTransferFunctionButterworthZspace(filterType, n1, n2, n_poles, m_cof, Gain, m_Bandwidth, m_TheSamplingRate, m_Real, m_Imaginary);

				else if (tolower(filter) == 'c')
					OnTransferFunctionChebyshevZspace(filterType, n1, n2, n_poles, m_cof, m_eps, m_Bandwidth, m_TheSamplingRate, m_Real, m_Imaginary);

			}
			// TF (end)

		// DT of TF (begin)
			char TFfilename[300];
			sprintf(TFfilename, "%s", "TF.img");

			if (tolower(filter_kz) == 'k')
			{
				std::cout << "Now calculating the Direct Fourier Transform of TF..." << endl;
				OnFourierTransform_scale(TFfilename, n1, n2);

			}
			else if (tolower(filter_kz) == 'z')
			{
				std::cout << "Now calculating the Direct Z Transform of TF..." << endl;
				OnZTransform_scale(TFfilename, n1, n2, m_Real, m_Imaginary);
			}
			// DT of TF (end)

			// invT (begin)
			if (tolower(filter_kz) == 'k')
			{
				std::cout << "Now calculating the Inverse Fourier Transform..." << endl;
				OnInverseFourierTransformTransferFunctionKspace(n1, n2);

			}
			else if (tolower(filter_kz) == 'z')
			{
				std::cout << "Now calculating the Inverse Z Transform..." << endl;
				OnInverseZTransformTransferFunction(n1, n2, m_Real, m_Imaginary);
			}
			// invT (end)

			// alert the user of the end of the program
			std::cout << "End of Computation..." << endl;
			std::cout << endl;

			// save to log file
			fprintf(savedata, "%s\n", "End of Computation...");
			fprintf(savedata, "\n");

			fclose(savedata);

			// delete the memory address
			// allocated to the images
			// do this by the command 'delete'
			// applied to the object 'ImageReconstruction' which
			// invokes the pointer to the data
			// structure 'data' containing the image
			// 'Signal'
			delete ImageReconstruction.pointer;

			// the object 'ImageReconstruction' invokes the
			// class destructor
			ImageReconstruction.~ImageReconstruction2024();
		} // processing (end)

	} // run the program (end)

	// ANSI C requires the 'main'
	// function returning a value: 
	// zero in this case
	return 0;
} // end of main 


void OnFrequencyDomain(char filterType, char filter_kz, char imageFilename[], int rcxres, int rcyres, double m_Real, double m_Imaginary, int m_angles, double Sinc_SamplingRate)
{

	int NofXpixels = rcxres;
	int NofYpixels = rcyres;

	double pi = 3.141592;

	double* SpaceR = 0;
	double* SpaceI = 0;
	double* SpaceM = 0;

	double* Signal = 0;

	double* BandWidth = 0;

	double* RadonSincFilter = 0;

	int i, j, index, dx, dy, w;

	FILE* logfile;

	char logfilename[128] = "ImageReconstruction2024.log";

	if ((logfile = fopen(logfilename, "w+")) == NULL)
	{

		printf("%s\n %s\n", "Unable to open log File", "Now Exit");

		exit(0);

	}
	else { // allocate memory 

		if ((Signal = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			exit(0);

		}

		if ((SpaceR = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			free(Signal);
			exit(0);

		}

		if ((SpaceI = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			free(Signal);
			free(SpaceR);
			exit(0);

		}

		if ((SpaceM = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			free(Signal);
			free(SpaceR);
			free(SpaceI);
			exit(0);

		}


		if ((RadonSincFilter = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			free(Signal);
			free(SpaceR);
			free(SpaceI);
			free(SpaceM);
			exit(0);

		}


		if ((BandWidth = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			free(Signal);
			free(SpaceR);
			free(SpaceI);
			free(SpaceM);
			free(RadonSincFilter);
			exit(0);

		}

	} // allocate memory 


	FILE* pf;
	char SignalFilename[128];

	sprintf(SignalFilename, "%s", imageFilename);

	if ((pf = fopen(SignalFilename, "rb+")) == NULL)
	{

		fprintf(logfile, "%s\n", "Cannot open file to read Signal");

		// FIFO memory deallocation method
		free(Signal);
		free(SpaceR);
		free(SpaceI);
		free(SpaceM);
		free(RadonSincFilter);
		free(BandWidth);
		exit(0);

	}
	else { // read data


		double readData = 0.0;

		for (i = 0; i < rcyres; i++)
		{ ///read signal data
			for (j = 0; j < rcxres; j++)
			{

				index = ((j * rcyres) + i);

				fread(&readData, sizeof(double), 1, pf);

				*(Signal + index) = (double)readData;

			}
		} ///read signal data

		fprintf(logfile, "%s\n", "Signal Read in DOUBLE (64bits) format");

		fclose(pf);
	} // save data


	//// initialize pointers
	for (i = 0; i < NofYpixels; i++)
	{
		for (j = 0; j < NofXpixels; j++)
		{

			int index = ((j * NofYpixels) + i);

			*(SpaceR + index) = (double)0.0;

			*(SpaceI + index) = (double)0.0;

			*(SpaceM + index) = (double)0.0;

		}

	}

	if (filter_kz == 'k')
	{

		printf("%s\n", "Now Direct Fourier Trasforming the Image...");

		///// Fourier Transform //////
		for (i = 0; i < NofYpixels; i++)
		{ ///calculate k-space data

			for (j = 0; j < NofXpixels; j++)
			{

				dx = ((int)i - NofYpixels / 2);
				dy = ((int)j - NofXpixels / 2);

				int k2 = ((int)(dy * NofYpixels) + dx);

				w = ((j * NofYpixels) + i);

				for (int s = 0; s < NofYpixels; s++)
				{ ///calculate k-space data 
					for (int p = 0; p < NofXpixels; p++)
					{


						int ds = ((int)NofYpixels / 2 - s);
						int dp = ((int)NofXpixels / 2) - p;

						int k3 = ((int)(ds * NofXpixels) + dp);

						int t = ((p * NofYpixels) + s);


						double phase = ((double)(2.0 * pi * k2 * k3) / ((double)NofXpixels * (double)NofYpixels));


						double complexR = (double)cos((double)phase) + (double)sin((double)phase);

						double complexI = (double)-sin((double)phase) + (double)cos((double)phase);


						*(SpaceR + w) += (double)*(Signal + t) * (double)complexR;

						*(SpaceI + w) += (double)*(Signal + t) * (double)complexI;

					}

				}///calculate k-space data 


			}
		} ///calculate k-space data

	}
	else if (filter_kz == 'z')
	{

		printf("%s\n", "Now Direct Z Trasforming the Image...");

		double phase;
		double exponent, r;
		double complexRZ, complexIZ;

		double magnitude = (double)sqrt((double)m_Real * m_Real + (double)m_Imaginary * m_Imaginary);
		double Zphase = (double)2.0 * pi * atan2((double)m_Imaginary, (double)m_Real) / ((double)NofXpixels * NofYpixels);

		///// Z Transform //////
		for (i = 0; i < NofYpixels; i++)
		{ ///calculate Z-Space data

			for (j = 0; j < NofXpixels; j++)
			{


				dx = ((int)i - NofYpixels / 2);
				dy = ((int)j - NofXpixels / 2);

				int k2 = ((int)(dy * NofYpixels) + dx);

				w = ((j * NofYpixels) + i);

				for (int s = 0; s < NofYpixels; s++)
				{ ///calculate Z-Space data 
					for (int p = 0; p < NofXpixels; p++)
					{


						int ds = ((int)s - NofYpixels / 2);
						int dp = ((int)p - NofXpixels / 2);

						int k3 = ((int)(ds * NofXpixels) + dp);

						int t = ((p * NofYpixels) + s);


						phase = ((double)(2.0 * pi * k2 * k3) / (NofXpixels * NofYpixels));

						exponent = (double)2.0 * pi * t * (double)Zphase / ((double)pow((double)NofXpixels * NofYpixels, 2.0));

						exponent = (double)fabs((double)exponent);

						r = (double)pow((double)magnitude, -(double)exponent);


						complexRZ = (double)cos((double)phase) + (double)sin((double)phase);

						complexIZ = -(double)sin((double)phase) + (double)cos((double)phase);


						*(SpaceR + w) += (double)*(Signal + t) * (double)complexRZ * (double)r;

						*(SpaceI + w) += (double)*(Signal + t) * (double)complexIZ * (double)r;
					}

				}///calculate Z-Space data 


			}
		} ///calculate Z-Space data
		///// Z Transform //////

	}

	char filename[300];
	sprintf(filename, "%s", "SpaceR-Image.img");

	fprintf(logfile, "%s\t%s\n", "Now Saving Signal in File: ", filename);

	if ((pf = fopen(filename, "wb+")) == NULL)
	{

		fprintf(logfile, "%s\n", "Cannot open file to save Signal");

		// FIFO memory deallocation method
		free(Signal);
		free(SpaceR);
		free(SpaceI);
		free(SpaceM);
		free(RadonSincFilter);
		free(BandWidth);
		exit(0);

	}
	else { // save data


		for (i = 0; i < NofYpixels; i++)
		{ ///save data
			for (j = 0; j < NofXpixels; j++)
			{

				int index = ((j * NofYpixels) + i);

				double savedata = (double)*(SpaceR + index);

				fwrite(&savedata, sizeof(double), 1, pf);

			}
		} ///save data

		fprintf(logfile, "%s\n", "Signal Saved");

		fclose(pf);
	} // save data


	sprintf(filename, "%s", "SpaceI-Image.img");

	fprintf(logfile, "%s\t%s\n", "Now Saving Signal in File: ", filename);

	if ((pf = fopen(filename, "wb+")) == NULL)
	{

		fprintf(logfile, "%s\n", "Cannot open file to save Signal");

		// FIFO memory deallocation method
		free(Signal);
		free(SpaceR);
		free(SpaceI);
		free(SpaceM);
		free(RadonSincFilter);
		free(BandWidth);
		exit(0);

	}
	else { // save data


		for (i = 0; i < NofYpixels; i++)
		{ ///save data
			for (j = 0; j < NofXpixels; j++)
			{

				int index = ((j * NofYpixels) + i);

				double savedata = (double)*(SpaceI + index);

				fwrite(&savedata, sizeof(double), 1, pf);

			}
		} ///save data

		fprintf(logfile, "%s\n", "Signal Saved");

		fclose(pf);
	} // save data


	// Magnitude (begin)
	for (i = 0; i < NofYpixels; i++)
	{
		for (j = 0; j < NofXpixels; j++)
		{

			int index = ((j * NofYpixels) + i);

			*(SpaceM + index) = (double)sqrt((double)*(SpaceR + index) * (double)*(SpaceR + index) +
		                              		 (double)*(SpaceI + index) * (double)*(SpaceI + index));

		}
	} // Magnitude (end)

	sprintf(filename, "%s", "SpaceM-Image.img");

	fprintf(logfile, "%s\t%s\n", "Now Saving Signal in File: ", filename);

	if ((pf = fopen(filename, "wb+")) == NULL)
	{

		fprintf(logfile, "%s\n", "Cannot open file to save Signal");

		// FIFO memory deallocation method
		free(Signal);
		free(SpaceR);
		free(SpaceI);
		free(SpaceM);
		free(RadonSincFilter);
		free(BandWidth);
		exit(0);

	}
	else { // save data


		for (i = 0; i < NofYpixels; i++)
		{ ///save data
			for (j = 0; j < NofXpixels; j++)
			{

				int index = ((j * NofYpixels) + i);

				double savedata = (double)*(SpaceM + index);

				fwrite(&savedata, sizeof(double), 1, pf);

			}
		} ///save data

		fprintf(logfile, "%s\n", "Signal Saved");

		fclose(pf);
	} // save data


	printf("%s\n", "Frequency Domain Processing Completed");
	fprintf_s(logfile, "%s\n", "Frequency Domain Processing Completed");

	std::cout << "Now Calculating the Pixel-Local k-space Image..." << endl;

	// open the file containing the k-space
	sprintf(filename, "%s", "SpaceM-Image.img");

	if ((pf = fopen(filename, "rb")) == NULL)
	{
		// alert the user and save into the log file 
		// the event that the program cannot open the 
		// file containing the image to process
		std::cout << "Cannot open file: " << filename << endl;
		// FIFO memory deallocation method
		free(Signal);
		free(SpaceR);
		free(SpaceI);
		free(SpaceM);
		free(RadonSincFilter);
		free(BandWidth);
		exit(0);

	}
	else { // else

		double number;

		//// initialize pointer
		for (int i = 0; i < NofYpixels; i++)
		{
			for (int j = 0; j < NofXpixels; j++)
			{

				int index = ((j * NofYpixels) + i);

				// at each iteration of the two for loops
				// the program reads the pixel value from the
				// file containing the image and 
				fread(&number, sizeof(double), 1, pf);

				*(SpaceM + index) = (double)number;

			} // y dim

		}  // x dim 

		// close the file containg the image to process 	
		fclose(pf);

	} // else 
	/// read image file (end)

	// scale the K-space magnitude (begin)
	// Procedure to scale the K-space Magnitude (begin)
	// scale (begin) 
	double max = *(SpaceM);
	double min = *(SpaceM);

	double averageKspace = 0.0;

	/// compute max and min of data (begin)
	for (int i = 0; i < NofYpixels; i++)
	{
		for (int j = 0; j < NofXpixels; j++)
		{

			int index = ((j * NofYpixels) + i);


			if (*(SpaceM + index) > (double)max)

				max = (double)*(SpaceM + index);

			if (*(SpaceM + index) < (double)min)

				min = (double)*(SpaceM + index);

			averageKspace += (double)*(SpaceM + index);

		} // y dim

	}  // x dim
	/// compute max and min of data (end)

	averageKspace /= ((double)NofXpixels * NofYpixels);

	double sDevKspace = (double)0.0;

	for (int i = 0; i < NofYpixels; i++)
	{
		for (int j = 0; j < NofXpixels; j++)
		{

			int index = ((j * NofYpixels) + i);

			sDevKspace += (double)pow(((double)*(SpaceM + index) - (double)averageKspace), 2.0);

		} // y dim

	}  // x dim

	sDevKspace = (double)sqrt((double)sDevKspace / ((double)NofXpixels * NofYpixels));


	// calculate Bandwidth adaptively (begin)
	for (int i = 0; i < NofYpixels; i++)
	{
		for (int j = 0; j < NofXpixels; j++)
		{

			int index = ((j * NofYpixels) + i);

			if (max == min) { *(SpaceM + index) = (double)1.0; }

			else {

				if ((double)sDevKspace != (double)0.0)
				{

					*(SpaceM + index) =

						((*(SpaceM + index) - averageKspace) / sDevKspace) / ((double)max - min);


					*(SpaceM + index) = (double)z_f /

						((double)z_f + exp(-(double)*(SpaceM + index)));

				}
				else { *(SpaceM + index) = (double)1.0; }

			}

		} // y dim

	}  // x dim
	// calculate Bandwidth adaptively (end)

	// assign the name string "Signal.img"
	// to the string 'outputFile'
	char outputFile[128];
	FILE* savedata;

	sprintf(outputFile, "%s", "BandWidth.img");

	// open the image file in write-binary mode
	if ((savedata = fopen(outputFile, "wb")) == NULL)
	{
		// alert the user of possible failure in opening the file
		std::cout << "Cannot open output file, Now Exit..." << endl;
		// FIFO memory deallocation method
		free(Signal);
		free(SpaceR);
		free(SpaceI);
		free(SpaceM);
		free(RadonSincFilter);
		free(BandWidth);
		exit(0);

	}
	else { // (save)


		for (int i = 0; i < NofYpixels; i++)
		{
			for (int j = 0; j < NofXpixels; j++)
			{

				int index = ((j * NofYpixels) + i);

				// at each iteration of the for loop saves the pixel
				// value contained at the memory address: ' &pointer->Signal[v][f] '
				double value = (double)*(SpaceM + index);

				fwrite(&value, sizeof(double), 1, savedata);

			} 

		}
		// close the file afer saving
		fclose(savedata);

	} // (save)


	std::cout << "Now Calculating the Radon SincFilter of the Image..." << endl;

	double s_segment, z_segment;
	double dy_z, x_z, y_z;
	double sincx, sincy;
	double toll = 0.000001;

	int angles = m_angles;

	for (int k = 1; k <= angles; k++)
	{ // angles

		///// Radon Transform //////
		for (i = 0; i < NofYpixels; i++)
		{ ///calculate Radon-space data

			for (j = 0; j < NofXpixels; j++)
			{

				double alpha = ((double)2.0 * pi * i * j * k / ((double)angles * NofXpixels * NofYpixels));

				// Radon
				dx = ((int)i - NofYpixels / 2);
				dy = ((int)j - NofXpixels / 2);

				if (_isnan(((double)alpha)) == 1) { alpha = (double)0.0; }

				else {}


				// calculate the variable s
				if ((double)cos((double)alpha) != (double)0.0)

					s_segment = (double)dx / ((double)cos((double)alpha));

				else s_segment = (double)0.0;


				if (_isnan(((double)s_segment)) == 1) { s_segment = (double)0.0; }

				else {}


				// calculate the orthogonal from the point source
				dy_z = (double)s_segment * ((double)sin((double)alpha));

				if (_isnan(((double)dy_z)) == 1) { dy_z = (double)0.0; }

				else {}


				// calculate the variable z
				if (((double)sin((double)(pi / 2.0) - alpha)) != 0.0)

					z_segment = (double)dy_z / ((double)sin((double)(pi / 2.0) - alpha));

				else z_segment = (double)0.0;

				if (_isnan(((double)z_segment)) == 1) { z_segment = (double)0.0; }

				else {}


				// calculate the two independent variables of the Radon transform
				x_z = ((double)z_segment * (double)sin((double)alpha)) +
  					  ((double)s_segment * (double)cos((double)alpha));

				if (_isnan(((double)x_z)) == 1) { x_z = (double)0.0; }

				else {}


				y_z = (-(double)z_segment * (double)cos((double)alpha)) +
					   ((double)s_segment * (double)sin((double)alpha));

				if (_isnan(((double)y_z)) == 1) { y_z = (double)0.0; }

				else {}

				w = ((j * NofYpixels) + i);

				for (int s = 0; s < NofYpixels - 1; s++)
				{ ///calculate Radon-Space data 
					for (int p = 0; p < NofXpixels - 1; p++)
					{

						int t = ((p * NofYpixels) + s);

						double dty = ((double)((double)s - NofYpixels / 2) * Sinc_SamplingRate *
							          (double)*(SpaceM + t));

						double dtx = ((double)((double)p - NofXpixels / 2) * Sinc_SamplingRate *
							          (double)*(SpaceM + t));


						double dy_y = (double)pi * (2.0 * (double)*(SpaceM + t) * y_z - dty);
						double dx_x = (double)pi * (2.0 * (double)*(SpaceM + t) * x_z - dtx);

						if ((double)fabs((double)dx_x) > toll)

							sincx = (double)sin((double)dx_x) / ((double)dx_x);

						else sincx = (double)1.0;

						if ((double)fabs((double)dy_y) > toll)

							sincy = (double)sin((double)dy_y) / ((double)dy_y);

						else sincy = (double)1.0;


						*(RadonSincFilter + w) += ((double)sincx * sincy);

					}

				}///calculate Radon-space data 

			}
		} ///calculate Radon-space data
		///// Radon Transform //////

	} // angles



	sprintf(filename, "%s", "RadonSincFilter.img");

	fprintf(logfile, "%s\t%s\n", "Now Saving Filtered Signal in File: ", filename);

	if ((pf = fopen(filename, "wb+")) == NULL)
	{

		fprintf(logfile, "%s\n", "Cannot open file to save Signal");

		// FIFO memory deallocation method
		free(Signal);
		free(SpaceR);
		free(SpaceI);
		free(SpaceM);
		free(RadonSincFilter);
		free(BandWidth);

		exit(0);

	}
	else { // save data


		for (i = 0; i < NofYpixels; i++)
		{ ///save data
			for (j = 0; j < NofXpixels; j++)
			{

				int index = ((j * NofYpixels) + i);

				double savedata = (double)*(RadonSincFilter + index);

				fwrite(&savedata, sizeof(double), 1, pf);

			}
		} ///save data

		fprintf(logfile, "%s\n", "Signal Saved");

		fclose(pf);
	} // save data


	 //// initialize pointers
	for (i = 0; i < NofYpixels; i++)
	{
		for (j = 0; j < NofXpixels; j++)
		{

			int index = ((j * NofYpixels) + i);

			*(SpaceR + index) = (double)0.0;

			*(SpaceI + index) = (double)0.0;

			*(SpaceM + index) = (double)0.0;

		}

	}

	//// Procedure to scale the filter (begin)
	 // scale tf (begin) 
	max = *(RadonSincFilter);
	min = *(RadonSincFilter);

	/// compute max and min of data (begin)
	for (i = 0; i < rcyres; i++)
	{
		for (j = 0; j < rcxres; j++)
		{

			index = ((j * rcyres) + i);

			if (*(RadonSincFilter + index) > (double)max)

				max = (double)*(RadonSincFilter + index);

			if (*(RadonSincFilter + index) < (double)min)

				min = (double)*(RadonSincFilter + index);

		} // y dim

	}  // x dim
	/// compute max and min of data (end)

	if (tolower(filterType) == 'l') {

		// scale (begin)
		for (i = 0; i < rcyres; i++)
		{
			for (j = 0; j < rcxres; j++)
			{

				index = ((j * rcyres) + i);

				if (max == min) *(RadonSincFilter + index) = (double)0.0;

				else {

					*(RadonSincFilter + index) = (double)fabs((min - (double)*(RadonSincFilter + index)) / (min - max));

					*(RadonSincFilter + index) *= ft_SCALE;

				}

			} // y dim

		}  // x dim

	}
	else if (tolower(filterType) == 'h') {

		// scale (begin)
		for (i = 0; i < rcyres; i++)
		{
			for (j = 0; j < rcxres; j++)
			{

				index = ((j * rcyres) + i);

				if (max == min) *(RadonSincFilter + index) = (double)0.0;

				else {

					// HP = 1.0 - LP
					*(RadonSincFilter + index) = (double)1.0 - (double)fabs((min - (double)*(RadonSincFilter + index)) / (min - max));

					*(RadonSincFilter + index) *= ft_SCALE;

				}

			} // y dim

		}  // x dim
		// scale (end)
	} //// Procedure to scale the filter (end)
	
	if (filter_kz == 'k')
	{
		printf("%s\n", "Now Direct Fourier Trasforming the SincFilter Image...");
		printf("%s\n", "1D Fourier Transform...");

		///// 1D Fourier Transform in polar coordinates//////
		for (j = 0; j < NofXpixels; j++)
		{ // w for 1D FT

			int dj = ((int)j - NofXpixels / 2);

			for (int s = 0; s < NofYpixels; s++)
			{ ///calculate k-space data 

				w = ((j * NofYpixels) + s);


				for (int p = 0; p < NofXpixels; p++)
				{ // take one line of the 2D image

					int ds = ((int)s - NofYpixels / 2);
					int dp = ((int)p - NofXpixels / 2);

					int k3 = ((int)(ds * NofXpixels) + dp);

					int t = ((p * NofYpixels) + s);

					double alpha = (double)atan2((double)ds, (double)dj);

					double r = sqrt(((double)(double)pow((double)ds, 2.0) + (double)pow((double)dj, 2.0)));

					double argument = ((double)r * cos((double)alpha)) + ((double)r * sin((double)alpha));

					double phase = ((double)2.0 * pi * k3 * argument);

					double complexR = (double)cos((double)phase) + (double)sin((double)phase);

					double complexI = (double)-sin((double)phase) + (double)cos((double)phase);

					// w for 1D FT
					*(SpaceR + w) += (double)*(RadonSincFilter + t) * (double)complexR;

					*(SpaceI + w) += (double)*(RadonSincFilter + t) * (double)complexI;
					// w for 1D FT

				} // take one line of the 2D image

			}///calculate k-space data 

		}// w for 1D FT
		///// 1D Fourier Transform in polar coordinates//////


		///// 1D Fourier Transform in polar coordinates//////
		for (j = 0; j < NofXpixels; j++)
		{ // w for 1D FT

			int dj = ((int)j - NofXpixels / 2);

			for (int s = 0; s < NofYpixels; s++)
			{ ///calculate k-space data 

				w = ((s * NofXpixels) + j);


				for (int p = 0; p < NofXpixels; p++)
				{ // take one line of the 2D image

					int ds = ((int)s - NofYpixels / 2);
					int dp = ((int)p - NofXpixels / 2);

					int k3 = ((int)(ds * NofXpixels) + dp);

					int t = ((p * NofYpixels) + s);

					double alpha = (double)atan2((double)ds, (double)dj);

					double r = sqrt(((double)(double)pow((double)ds, 2.0) + (double)pow((double)dj, 2.0)));

					double argument = ((double)r * cos((double)alpha)) + ((double)r * sin((double)alpha));

					double phase = ((double)2.0 * pi * k3 * argument);

					double complexR = (double)cos((double)phase) + (double)sin((double)phase);

					double complexI = (double)-sin((double)phase) + (double)cos((double)phase);

					// w for 1D FT
					*(SpaceR + w) += (double)*(RadonSincFilter + t) * (double)complexR;

					*(SpaceI + w) += (double)*(RadonSincFilter + t) * (double)complexI;
					// w for 1D FT

				} // take one line of the 2D image

			}///calculate k-space data 

		}// w for 1D FT
		///// 1D Fourier Transform in polar coordinates//////
	}
	else if (filter_kz == 'z')
	{

		printf("%s\n", "Now Direct Z Trasforming the SincFilter Image...");
		printf("%s\n", "1D Z Transform...");

		double phase;
		double exponent;
		double complexRZ, complexIZ;

		double magnitude = (double)sqrt((double)m_Real * m_Real + (double)m_Imaginary * m_Imaginary);
		double Zphase = (double)2.0 * pi * atan2((double)m_Imaginary, (double)m_Real) / ((double)NofXpixels * NofYpixels);

		///// 1D Z Transform in polar coordinates//////
		for (j = 0; j < NofXpixels; j++)
		{ // w for 1D Z

			int dj = ((int)j - NofXpixels / 2);

			for (int s = 0; s < NofYpixels; s++)
			{ ///calculate z-space data 

				w = ((j * NofYpixels) + s);


				for (int p = 0; p < NofXpixels; p++)
				{ // take one line of the 2D image

					int ds = ((int)s - NofYpixels / 2);
					int dp = ((int)p - NofXpixels / 2);

					int k3 = ((int)(ds * NofXpixels) + dp);

					int t = ((p * NofYpixels) + s);

					double alpha = (double)atan2((double)ds, (double)dj);

					double r = sqrt(((double)(double)pow((double)ds, 2.0) + (double)pow((double)dj, 2.0)));

					double argument = ((double)r * cos((double)alpha)) + ((double)r * sin((double)alpha));

					phase = ((double)2.0 * pi * k3 * argument);

					exponent = (double)2.0 * pi * t * (double)Zphase / ((double)pow((double)NofXpixels * NofYpixels, 2.0));

					exponent = (double)fabs((double)exponent);

					double rr = (double)pow((double)magnitude, -(double)exponent);


					complexRZ = (double)cos((double)phase) + (double)sin((double)phase);

					complexIZ = (double)-sin((double)phase) + (double)cos((double)phase);

					// w for 1D ZT
					*(SpaceR + w) += (double)*(RadonSincFilter + t) * (double)complexRZ * (double)rr;

					*(SpaceI + w) += (double)*(RadonSincFilter + t) * (double)complexIZ * (double)rr;
					// w for 1D ZT

				} // take one line of the 2D image

			}///calculate z-space data 

		}// w for 1D ZT
		///// 1D Z Transform in polar coordinates//////


		///// 1D Z Transform in polar coordinates//////
		for (j = 0; j < NofXpixels; j++)
		{ // w for 1D ZT

			int dj = ((int)j - NofXpixels / 2);

			for (int s = 0; s < NofYpixels; s++)
			{ ///calculate z-space data 

				w = ((s * NofXpixels) + j);


				for (int p = 0; p < NofXpixels; p++)
				{ // take one line of the 2D image

					int ds = ((int)s - NofYpixels / 2);
					int dp = ((int)p - NofXpixels / 2);

					int k3 = ((int)(ds * NofXpixels) + dp);

					int t = ((p * NofYpixels) + s);

					double alpha = (double)atan2((double)ds, (double)dj);

					double r = sqrt(((double)(double)pow((double)ds, 2.0) + (double)pow((double)dj, 2.0)));

					double argument = ((double)r * cos((double)alpha)) + ((double)r * sin((double)alpha));

					double phase = ((double)2.0 * pi * k3 * argument);

					exponent = (double)2.0 * pi * t * (double)Zphase / ((double)pow((double)NofXpixels * NofYpixels, 2.0));

					exponent = (double)fabs((double)exponent);

					double rr = (double)pow((double)magnitude, -(double)exponent);

					double complexR = (double)cos((double)phase) + (double)sin((double)phase);

					double complexI = (double)-sin((double)phase) + (double)cos((double)phase);

					// w for 1D ZT
					*(SpaceR + w) += (double)*(RadonSincFilter + t) * (double)complexR * (double)rr;

					*(SpaceI + w) += (double)*(RadonSincFilter + t) * (double)complexI * (double)rr;
					// w for 1D ZT

				} // take one line of the 2D image

			}///calculate z-space data 

		}// w for 1D ZT
		///// 1D Z Transform in polar coordinates//////

	}

	sprintf(filename, "%s", "SpaceR-Filter.img");

	fprintf(logfile, "%s\t%s\n", "Now Saving Signal in File: ", filename);

	if ((pf = fopen(filename, "wb+")) == NULL)
	{

		fprintf(logfile, "%s\n", "Cannot open file to save Signal");

		// FIFO memory deallocation method
		free(Signal);
		free(SpaceR);
		free(SpaceI);
		free(SpaceM);
		free(RadonSincFilter);
		free(BandWidth);
		exit(0);

	}
	else { // save data


		for (i = 0; i < NofYpixels; i++)
		{ ///save data
			for (j = 0; j < NofXpixels; j++)
			{

				int index = ((j * NofYpixels) + i);

				double savedata = (double)*(SpaceR + index);

				fwrite(&savedata, sizeof(double), 1, pf);

			}
		} ///save data

		fprintf(logfile, "%s\n", "Signal Saved");

		fclose(pf);
	} // save data


	sprintf(filename, "%s", "SpaceI-Filter.img");

	fprintf(logfile, "%s\t%s\n", "Now Saving Signal in File: ", filename);

	if ((pf = fopen(filename, "wb+")) == NULL)
	{

		fprintf(logfile, "%s\n", "Cannot open file to save Signal");

		// FIFO memory deallocation method
		free(Signal);
		free(SpaceR);
		free(SpaceI);
		free(SpaceM);
		free(RadonSincFilter);
		free(BandWidth);

		exit(0);

	}
	else { // save data


		for (i = 0; i < NofYpixels; i++)
		{ ///save data
			for (j = 0; j < NofXpixels; j++)
			{

				int index = ((j * NofYpixels) + i);

				double savedata = (double)*(SpaceI + index);

				fwrite(&savedata, sizeof(double), 1, pf);

			}
		} ///save data

		fprintf(logfile, "%s\n", "Signal Saved");

		fclose(pf);
	} // save data


	// Magnitude (begin)
	for (i = 0; i < NofYpixels; i++)
	{
		for (j = 0; j < NofXpixels; j++)
		{

			int index = ((j * NofYpixels) + i);

			*(SpaceM + index) = (double)sqrt((double)*(SpaceR + index) * (double)*(SpaceR + index) +
				                             (double)*(SpaceI + index) * (double)*(SpaceI + index));

		}
	} // Magnitude (end)

	sprintf(filename, "%s", "SpaceM-Filter.img");

	fprintf(logfile, "%s\t%s\n", "Now Saving Signal in File: ", filename);

	if ((pf = fopen(filename, "wb+")) == NULL)
	{

		fprintf(logfile, "%s\n", "Cannot open file to save Signal");

		// FIFO memory deallocation method
		free(Signal);
		free(SpaceR);
		free(SpaceI);
		free(SpaceM);
		free(RadonSincFilter);
		free(BandWidth);

		exit(0);

	}
	else { // save data


		for (i = 0; i < NofYpixels; i++)
		{ ///save data
			for (j = 0; j < NofXpixels; j++)
			{

				int index = ((j * NofYpixels) + i);

				double savedata = (double)*(SpaceM + index);

				fwrite(&savedata, sizeof(double), 1, pf);

			}
		} ///save data

		fprintf(logfile, "%s\n", "Signal Saved");

		fclose(pf);
	} // save data


	printf("%s\n", "Frequency Domain Processing Completed");
	fprintf_s(logfile, "%s\n", "Frequency Domain Processing Completed");

	fclose(logfile);

	// FIFO memory deallocation method
	free(Signal);
	free(SpaceR);
	free(SpaceI);
	free(SpaceM);
	free(RadonSincFilter);
	free(BandWidth);

}

void OnTransferFunctionButterworthKspace(char filterType, int rcxres, int rcyres, int n_poles, double m_cof, double Gain, double m_Bandwidth, double m_TheSamplingRate)
{

	int Order = n_poles;
	int NofXpixels = rcxres;
	int NofYpixels = rcyres;

	int i, j, index, t;

	double pi = 3.141592;

	double* tf = 0;

	FILE* logfile;

	char logfilename[128] = "TF.log";

	if ((logfile = fopen(logfilename, "w+")) == NULL)
	{

		printf("%s\n %s\n", "Unable to open log File", "Now Exit");

		exit(0);

	}
	else { // allocate memory 


		if ((tf = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			exit(0);

		}

	} // allocate memory 


	struct ButterworthPolynomial {

		double** Bw_Odd; // pointer to the matrix entry

		double** Bw_Even; // pointer to the matrix entry

		double** BW; // pointer to the matrix entry

	}*Butterworth_Polynomial_pointer; // pointer to the matrices


	// (1) allocate struct (begin)

	Butterworth_Polynomial_pointer = new ButterworthPolynomial;


	Butterworth_Polynomial_pointer->Bw_Odd = new double* [Order + 1];

	Butterworth_Polynomial_pointer->Bw_Even = new double* [Order + 1];

	Butterworth_Polynomial_pointer->BW = new double* [Order + 1];


	for (int v = 0; v <= Order; v++) { // (1)

		Butterworth_Polynomial_pointer->Bw_Odd[v] = new double[1];

		Butterworth_Polynomial_pointer->Bw_Even[v] = new double[1];

		Butterworth_Polynomial_pointer->BW[v] = new double[1];

	} // (1) allocate struct 'data' (end)


	  // (2) initialize (begin)
	for (int v = 0; v <= Order; v++) { // (a)

		for (int f = 0; f < 1; f++) { // (b)

			Butterworth_Polynomial_pointer->Bw_Odd[v][f] = (double)0.0;

			Butterworth_Polynomial_pointer->Bw_Even[v][f] = (double)0.0;

			Butterworth_Polynomial_pointer->BW[v][f] = (double)0.0;

		} //(b)

	} //(a)
   // (2) initialize (end)

   //// initialize pointer
	for (i = 0; i < NofYpixels; i++)
	{
		for (j = 0; j < NofXpixels; j++)
		{

			index = ((j * NofYpixels) + i);

			*(tf + index) = (double)0.0;

		}

	}


	double Bw_term = 1.0;
	double complex; // complex variable of the filter TF 
	int k;

	int di, dj;
	double dx, dy;
	double dtx, dty;
	double sincx, sincy;
	double toll = 0.000001;


	for (i = 0; i < NofYpixels; i++)
	{ ///calculate TF-Space data

		for (j = 0; j < NofXpixels; j++)
		{

			di = ((int)i - NofYpixels / 2);
			dj = ((int)j - NofXpixels / 2);

			dty = ((double)((double)i - NofYpixels / 2) * m_TheSamplingRate); //space (pixel) * frequency (1/sec)
			dtx = ((double)((double)j - NofXpixels / 2) * m_TheSamplingRate); //space (pixel) * frequency (1/sec)


			dy = (double)pi * (2.0 * m_Bandwidth * di - dty);
			// Where 2.0 * bandwidth * di is: 
			// frequency (1/sec) * space (pixel)
			dx = (double)pi * (2.0 * m_Bandwidth * dj - dtx);
			//		 2.0 * bandwidth * dj is: 
			// frequency (1/sec) * space (pixel)


			t = ((j * NofYpixels) + i);

			// STEP 1: Use the variable ' complex ' and assign to it the following multiplication:
			// 2.0 * pi * the square root of the sum between: 1. the second power of ' dx '; 
			// and 2. the second power of ' dy '. Then divide all by the factor: 
			// (double) pow(((double)NofXpixels*NofYpixels), ((double)1.0/m_cof));
			complex = (double)2.0 * pi * ((double)sqrt((double)(dx * dx + dy * dy))) /
				(double)pow(((double)NofXpixels * NofYpixels), 1.0 / m_cof);

			// Wikipedia Notes
			complex /= (double)m_cof;

			k = 0;

			if (Order % 2 == 0)
			{ // Butterworth (even)

				for (int o = 1; o <= ((int)Order / 2); o++)
				{

					k++;

					// STEP 2: Calculate ' Bw_term ' as per Wikipedia notes.
					// Hint: ' Bw_term ' is the normalized Butterworth polynomial
					// in its general form, when ' n ' (Order) is even
					Bw_term = (double)complex * complex - ((double)2.0 * complex *

						cos(pi * ((double)(2.0 * k + Order - 1.0) / (2.0 * Order))) + 1.0);

					Butterworth_Polynomial_pointer->Bw_Even[o][0] = ((double)Bw_term);

				}

			}// Butterworth (even)


			k = 0;


			if (Order % 2 != 0)
			{ // Butterworth (odd)


				for (int o = 1; o <= ((int)(Order - 1) / 2); o++)
				{

					k++;

					// STEP 3: Calculate ' Bw_term ' as per Wikipedia notes.
					// Hint: ' Bw_term ' is the normalized Butterworth polynomial
					// in its general form, when ' n ' (Order) is odd
					double Bw_term = ((double)complex + 1.0) * ((double)complex * complex - 2.0 * complex *

						cos(pi * ((double)(2.0 * k + Order - 1.0) / (2.0 * Order))) + 1.0);

					Butterworth_Polynomial_pointer->Bw_Odd[o][0] = ((double)Bw_term);

				}

			} // Butterworth Odd


			// calculate Butterworth polynomial (begin)
			double Butterworth_Polynomial = 1.0;

			if (Order % 2 == 0)
			{ // Butterworth (even)

				for (int p = 1; p <= ((int)Order / 2); p++)

					// STEP 4: Calculate ' Butterworth_Polynomial ' when ' n ' is even
					// Hint: see Wikipedia notes. One line of code that builds the 
					// multiplication between all of ' Butterworth_Polynomial_pointer->Bw_Even[p][0]); '
					Butterworth_Polynomial *= ((double)Butterworth_Polynomial_pointer->Bw_Even[p][0]);

			}
			else if (Order % 2 != 0) // Butterworth (odd)
			{

				for (int p = 1; p <= ((int)(Order - 1) / 2); p++)

					// STEP 5: Calculate ' Butterworth_Polynomial ' when ' n ' is odd
					// Hint: see Wikipedia notes. One line of code that builds the 
					// multiplication between all of ' Butterworth_Polynomial_pointer->Bw_Odd[p][0]); '
					Butterworth_Polynomial *= ((double)Butterworth_Polynomial_pointer->Bw_Odd[p][0]);

			}
			// calculate Butterworth polynomial (end)	


			// STEP 6: Check if ' fabs((double)dx) ' is greater than the tolerance: ' toll '
			// If so, then calculate the sinc function of ' dx ' and assign the value to
			// the variable ' sincx '. If, not set ' sincx = (double)1.0; ' Note that
			// ' sincx ' is the sinc function in ' x '
			if ((double)fabs((double)dx) > toll)

				sincx = (double)sin((double)dx) / ((double)dx);

			else sincx = (double)1.0;


			// STEP 7: Check if ' fabs((double)dy) ' is greater than the tolerance: ' toll '
			// If so, then calculate the sinc function of ' dy ' and assign the value to
			// the variable ' sincy '. If, not set ' sincy = (double)1.0; ' Note that
			// ' sincy ' is the sinc function in ' y '
			if ((double)fabs((double)dy) > toll)

				sincy = (double)sin((double)dy) / ((double)dy);

			else sincy = (double)1.0;



			if (((double)Butterworth_Polynomial) != 0.0) {

				if ((_isnan((double)Gain / Butterworth_Polynomial)) == 0) {

					// STEP 8: Calculate the transfer function: *(tf + t) as the ratio
					// between ' Gain ' and ' Butterworth_Polynomial '
					*(tf + t) = ((double)Gain / Butterworth_Polynomial);

					// STEP 9: build two-dimensional Sinc function and assign the value to the pointer ' *(tf + t) '
					// Hint: Multiply the Transfer Function ' *(tf + t) 'by the sinc functions in x and y: ((double)sincx * sincy).
					// Then assign the resulting multiplication to: ' *(tf + t) '
					*(tf + t) *= ((double)sincx * sincy);

				}
				else { *(tf + t) = (double)0.0; }

			}
			else { *(tf + t) = (double)0.0; }

		}

	}///calculate TF-Space data 
	///// TF //////


	 //// Procedure to scale the filter (begin)
	 // scale tf (begin) 
	double max = *(tf);
	double min = *(tf);

	/// compute max and min of data (begin)
	for (i = 0; i < rcyres; i++)
	{
		for (j = 0; j < rcxres; j++)
		{

			index = ((j * rcyres) + i);

			if (*(tf + index) > (double)max)

				max = (double)*(tf + index);

			if (*(tf + index) < (double)min)

				min = (double)*(tf + index);

		} // y dim

	}  // x dim
	/// compute max and min of data (end)

	if (tolower(filterType) == 'l') {

		// scale (begin)
		for (i = 0; i < rcyres; i++)
		{
			for (j = 0; j < rcxres; j++)
			{

				index = ((j * rcyres) + i);

				if (max == min) *(tf + index) = (double)0.0;

				else {

					*(tf + index) = (double)fabs((min - (double)*(tf + index)) / (min - max));

					*(tf + index) *= ft_SCALE;

				}

			} // y dim

		}  // x dim

	}
	else if (tolower(filterType) == 'h') {

		// scale (begin)
		for (i = 0; i < rcyres; i++)
		{
			for (j = 0; j < rcxres; j++)
			{

				index = ((j * rcyres) + i);

				if (max == min) *(tf + index) = (double)0.0;

				else {

					// HP = 1.0 - LP
					*(tf + index) = (double)1.0 - (double)fabs((min - (double)*(tf + index)) / (min - max));

					*(tf + index) *= ft_SCALE;

				}

			} // y dim

		}  // x dim
		// scale (end)
	} //// Procedure to scale the filter (end)

	char filename[328];
	FILE* pf;

	sprintf(filename, "%s", "TF.img");

	fprintf(logfile, "%s\t%s\n", "Now Saving TF in File: ", filename);

	if ((pf = fopen(filename, "wb+")) == NULL)
	{

		fprintf(logfile, "%s\n", "Cannot open file to save TF");

		// FIFO memory deallocation method
		free(tf);
		delete Butterworth_Polynomial_pointer;

		exit(0);

	}
	else { // save data

		double savedata = 0.0;

		for (i = 0; i < NofYpixels; i++)
		{ ///save TF data
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				savedata = (double)*(tf + index);

				fwrite(&savedata, sizeof(double), 1, pf);

			}
		} ///save Z-Space data

		fprintf(logfile, "%s\n", "TF Saved");

		fclose(pf);
	} // save data


	printf("%s\n", "TF Processing Completed");
	fprintf_s(logfile, "%s\n", "TF Processing Completed");

	fclose(logfile);


	// FIFO memory deallocation method
	free(tf);
	delete Butterworth_Polynomial_pointer;

}
void OnTransferFunctionChebyshevKspace(char filterType, int rcxres, int rcyres, int n_poles, double m_cof, double m_eps, double m_Bandwidth, double m_TheSamplingRate)
{

	int NofXpixels = rcxres;
	int NofYpixels = rcyres;

	int i, j, index;

	double pi = 3.141592;

	double* tf = 0;

	FILE* logfile;

	char filenamelog[128] = "Chebyshev.log";

	if ((logfile = fopen(filenamelog, "w+")) == NULL)
	{

		fprintf(logfile, "%s\n", "Unable to open log file, now exit...");
		exit(0);

	}
	else { // run the Filtering Process (begin) 


		if ((tf = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			exit(0);

		}

		struct ChebyshevPoles {

			double* spmR; // pointer to the matrix entry

			double* spmI; // pointer to the matrix entry

			double* spm; // pointer to the matrix entry

		}*Chebyshev_pointer; // pointer to the matrices


		// (1) allocate struct 'data' (begin)
		Chebyshev_pointer = new ChebyshevPoles;

		Chebyshev_pointer->spmR = new double[n_poles + 1];

		Chebyshev_pointer->spmI = new double[n_poles + 1];

		Chebyshev_pointer->spm = new double[n_poles + 1];


		// (2) initialize (begin)
		for (int v = 1; v <= n_poles; v++) { // (a)

			Chebyshev_pointer->spmR[v] = (double)0.0;

			Chebyshev_pointer->spmI[v] = (double)0.0;

			Chebyshev_pointer->spm[v] = (double)0.0;

		} //(a)
	   // (2) initialize (end)

		double eps = 1.0 / m_eps; // ripple factor is m_eps
		// cannot be zero as per data input to the program

		// calculate Chebyshev Space
		for (i = 1; i <= n_poles; i++)
		{

			// STEP 1: Calculate '  thetam ' as per Wikipedia notes
			double thetam = (pi / 2.0) * (2.0 * i - 1.0) / (n_poles);

			// calculate negative pole only to use in the transfer function
			double argument = ((double)1.0 / (n_poles)) *

				log((double)eps + sqrt(((double)eps * eps) + 1.0));

			// STEP 2: Calculate ' sinh ' of ' argument '
			double sinh = ((double)exp((double)argument) - (double)exp((double)-argument)) / 2.0;

			// STEP 3: Calculate ' cosh ' of ' argument '
			double cosh = ((double)exp((double)argument) + (double)exp((double)-argument)) / 2.0;

			// STEP 4: Calculate ' Chebyshev_pointer->spmR[i][0] ' as the multiplication
			// between ' -(double)sinh ' and ' (double)sin((double)thetam) '; See Wikipedia notes
			Chebyshev_pointer->spmR[i] = -(double)sinh * (double)sin((double)thetam);

			// STEP 5: Calculate ' Chebyshev_pointer->spmI[i][0] ' as the multiplication
			// between ' -(double)cosh ' and ' (double)cos((double)thetam) '; See Wikipedia notes
			Chebyshev_pointer->spmI[i] = (double)cosh * (double)cos((double)thetam);

		}

		FILE* opf;
		char ofilename[328];
		double savedata;

		sprintf(ofilename, "%s", "ChebyshevR.img");

		fprintf(logfile, "%s\t%s\n", "Now Saving Chebyshev Real Space in File: ", ofilename);

		if ((opf = fopen(ofilename, "wb+")) == NULL)
		{

			fprintf(logfile, "%s\n", "Cannot open file to save Chebyshev Real Space");

			free(tf);
			delete Chebyshev_pointer;

			exit(0);

		}
		else { // save data


			for (i = 1; i <= n_poles; i++)
			{ ///save Chebyshev-space data

				savedata = (double)Chebyshev_pointer->spmR[i];

				fwrite(&savedata, sizeof(double), 1, opf);


			} ///save Chebyshev-space data

			fprintf(logfile, "%s\n", "Chebyshev Real Space Saved");

			fclose(opf);
		} // save data


		sprintf(ofilename, "%s", "ChebyshevI.img");

		fprintf(logfile, "%s\t%s\n", "Now Saving Chebyshev Imaginary Space in File: ", ofilename);

		if ((opf = fopen(ofilename, "wb+")) == NULL)
		{

			fprintf(logfile, "%s\n", "Cannot open file to save Chebyshev Imaginary Space");

			free(tf);
			delete Chebyshev_pointer;

			exit(0);

		}
		else { // save data


			for (i = 1; i <= n_poles; i++)
			{ ///save Chebyshev-space data

				savedata = (double)Chebyshev_pointer->spmI[i];

				fwrite(&savedata, sizeof(double), 1, opf);


			} ///save Chebyshev-space data

			fprintf(logfile, "%s\n", "Chebyshev Imaginary Space Saved");

			fclose(opf);
		} // save data

		/// calculate magnitude of the Poles
		for (int s = 1; s <= n_poles; s++)
		{

			// magnitude of Pole
			Chebyshev_pointer->spm[s] = (double)sqrt(((double)Chebyshev_pointer->spmR[s] * Chebyshev_pointer->spmR[s]) +
				((double)Chebyshev_pointer->spmI[s] * Chebyshev_pointer->spmI[s]));
		} /// calculate magnitude of the Poles

		//// initialize pointer
		for (i = 0; i < NofYpixels; i++)
		{
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				*(tf + index) = (double)0.0;

			}

		}


		double complex;
		double tf_denominator;

		int t;
		int di, dj;
		double dx, dy;
		double dtx, dty;
		double sincx, sincy;
		double toll = 0.000001;


		for (i = 0; i < NofYpixels; i++)
		{ ///calculate TF-Space data

			for (j = 0; j < NofXpixels; j++)
			{

				di = ((int)i - NofYpixels / 2);
				dj = ((int)j - NofXpixels / 2);

				dty = ((double)((double)i - NofYpixels / 2) * m_TheSamplingRate); //space (pixel) * frequency (1/sec)
				dtx = ((double)((double)j - NofXpixels / 2) * m_TheSamplingRate); //space (pixel) * frequency (1/sec)


				dy = (double)pi * (2.0 * m_Bandwidth * di - dty);
				// Where 2.0 * bandwidth * di is: 
				// frequency (1/sec) * space (pixel)
				dx = (double)pi * (2.0 * m_Bandwidth * dj - dtx);
				//		 2.0 * bandwidth * dj is: 
				// frequency (1/sec) * space (pixel)


				t = ((j * NofYpixels) + i);

				// STEP 6: Use the variable ' complex ' and assign to it the following multiplication:
				// 2.0 * pi * the square root of the sum between: 1. the second power of ' dx '; 
				// and 2. the second power of ' dy '. Then divide all by the factor: 
				// (double) pow(((double)NofXpixels*NofYpixels), ((double)1.0/m_cof));
				complex = (double)2.0 * pi * ((double)sqrt((double)(dx * dx + dy * dy))) /
					(double)pow(((double)NofXpixels * NofYpixels), 1.0 / m_cof);

				double product = 1.0;

				for (int u = 1; u <= n_poles; u++)
				{

					if ((double)Chebyshev_pointer->spmR[u] < 0)


						product *= (((double)complex -

							((double)Chebyshev_pointer->spmR[u] +

								(double)Chebyshev_pointer->spmI[u])));


					else product *= (double)1.0;

				}


				// STEP 7: Check if ' fabs((double)dx) ' is greater than the tolerance: ' toll '
				// If so, then calculate the sinc function of ' dx ' and assign the value to
				// the variable ' sincx '. If, not set ' sincx = (double)1.0; ' Note that
				// ' sincx ' is the sinc function in ' x '
				if ((double)fabs((double)dx) > toll)

					sincx = (double)sin((double)dx) / ((double)dx);

				else sincx = (double)1.0;

				// STEP 8: Check if ' fabs((double)dy) ' is greater than the tolerance: ' toll '
				// If so, then calculate the sinc function of ' dy ' and assign the value to
				// the variable ' sincy '. If, not set ' sincy = (double)1.0; ' Note that
				// ' sincy ' is the sinc function in ' y '
				if ((double)fabs((double)dy) > toll)

					sincy = (double)sin((double)dy) / ((double)dy);

				else sincy = (double)1.0;



				if ((double)((pow(2.0, (n_poles - 1))) * m_eps) != 0.0 && product != (double)0.0) {


					tf_denominator = (((((double)pow(2.0, (n_poles - 1))) * m_eps)) *

						((double)(product)));

					if ((_isnan((double)tf_denominator)) == 0) {

						// STEP 9: Calculate the transfer function: *(tf + t) as the ratio
						// between ' 1.0 ' and ' tf_denominator '
						*(tf + t) = (double)1.0 / (((((double)pow(2.0, (n_poles - 1))) * m_eps)) *

							((double)(product)));

						// STEP 10: build two-dimensional Sinc function and assign the value to the pointer ' *(tf + t) '
						// Hint: Multiply the Transfer Function ' *(tf + t) 'by the sinc functions in x and y: ((double)sincx * sincy).
						// Then assign the resulting multiplication to: ' *(tf + t) '
						*(tf + t) *= ((double)sincx * sincy);

					}
					else { *(tf + t) = (double)0.0; }

				}
				else { *(tf + t) = (double)0.0; }


			}

		}///calculate TF-Space data 
		///// TF //////

		 //// Procedure to scale the filter (begin)
		 // scale tf (begin) 
		double max = *(tf);
		double min = *(tf);

		/// compute max and min of data (begin)
		for (i = 0; i < rcyres; i++)
		{
			for (j = 0; j < rcxres; j++)
			{

				index = ((j * rcyres) + i);

				if (*(tf + index) > (double)max)

					max = (double)*(tf + index);

				if (*(tf + index) < (double)min)

					min = (double)*(tf + index);

			} // y dim

		}  // x dim
		/// compute max and min of data (end)

		if (tolower(filterType) == 'l') {

			// scale (begin)
			for (i = 0; i < rcyres; i++)
			{
				for (j = 0; j < rcxres; j++)
				{

					index = ((j * rcyres) + i);

					if (max == min) *(tf + index) = (double)0.0;

					else {

						*(tf + index) = (double)fabs((min - (double)*(tf + index)) / (min - max));

						*(tf + index) *= ft_SCALE;

					}

				} // y dim

			}  // x dim

		}
		else if (tolower(filterType) == 'h') {

			// scale (begin)
			for (i = 0; i < rcyres; i++)
			{
				for (j = 0; j < rcxres; j++)
				{

					index = ((j * rcyres) + i);

					if (max == min) *(tf + index) = (double)0.0;

					else {

						// HP = 1.0 - LP
						*(tf + index) = (double)1.0 - (double)fabs((min - (double)*(tf + index)) / (min - max));

						*(tf + index) *= ft_SCALE;

					}

				} // y dim

			}  // x dim
			// scale (end)
		} //// Procedure to scale the filter (end)


		char filename[128];
		FILE* pf;

		sprintf(filename, "%s", "TF.img");

		fprintf(logfile, "%s\t%s\n", "Now Saving TF in File: ", filename);

		if ((pf = fopen(filename, "wb+")) == NULL)
		{

			fprintf(logfile, "%s\n", "Cannot open file to save TF");

			// FIFO memory deallocation method
			free(tf);
			delete Chebyshev_pointer;

			exit(0);

		}
		else { // save data


			for (i = 0; i < NofYpixels; i++)
			{ ///save TF data
				for (j = 0; j < NofXpixels; j++)
				{

					index = ((j * NofYpixels) + i);

					savedata = (double)*(tf + index);

					fwrite(&savedata, sizeof(double), 1, pf);

				}
			} ///save Z-Space data

			fprintf(logfile, "%s\n", "TF Saved");

			fclose(pf);
		} // save data


		printf("%s\n", "TF Processing Completed");
		fprintf_s(logfile, "%s\n", "TF Processing Completed");

		fclose(logfile);


		// FIFO memory deallocation method
		free(tf);
		delete Chebyshev_pointer;

	} // run the Filtering Process (ends) 

}


void OnFourierTransform_scale(char imageFilename[], int rcxres, int rcyres)
{

	int NofXpixels = rcxres;
	int NofYpixels = rcyres;

	int i, j, index;
	int dx, dy;
	int ds, dp;
	int k2, k3, w, t;

	double pi = 3.141592;

	double* kSpaceR = 0;
	double* kSpaceI = 0;
	double* Signal = 0;

	FILE* logfile;

	char logfilename[128] = "Fourier-T.log";

	if ((logfile = fopen(logfilename, "w+")) == NULL)
	{

		printf("%s\n %s\n", "Unable to open log File", "Now Exit");

		exit(0);

	}
	else { // allocate memory 


		if ((kSpaceR = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			exit(0);

		}

		if ((kSpaceI = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			free(kSpaceR);
			exit(0);

		}

		if ((Signal = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			free(kSpaceR);
			free(kSpaceI);
			exit(0);

		}

	} // allocate memory 

	//// read image data and initialize pointers
	double number = 0.0;

	for (i = 0; i < NofYpixels; i++)
	{
		for (j = 0; j < NofXpixels; j++)
		{

			index = ((j * NofYpixels) + i);

			*(kSpaceR + index) = (double)0.0;

			*(kSpaceI + index) = (double)0.0;

		}

	}

	FILE* pf;
	char SignalFilename[128];
	double readData;

	sprintf(SignalFilename, "%s", imageFilename);

	if ((pf = fopen(SignalFilename, "rb+")) == NULL)
	{

		fprintf(logfile, "%s\n", "Cannot open file to read Signal");

		// FIFO memory deallocation method
		free(kSpaceR);
		free(kSpaceI);
		free(Signal);

		exit(0);

	}
	else { // read data


		for (i = 0; i < rcyres; i++)
		{ ///read signal data
			for (j = 0; j < rcxres; j++)
			{

				index = ((j * rcyres) + i);

				fread(&readData, sizeof(double), 1, pf);

				*(Signal + index) = (double)readData;

			}
		} ///read signal data

		fprintf(logfile, "%s\n", "Signal Read in DOUBLE (64bits) format");

		fclose(pf);
	} // save data

	double phase, complexR, complexI;

	///// Fourier Transform //////
	for (i = 0; i < NofYpixels; i++)
	{ ///calculate k-space data

		for (j = 0; j < NofXpixels; j++)
		{


			dx = ((int)i - NofYpixels / 2);
			dy = ((int)j - NofXpixels / 2);

			k2 = ((int)(dy * NofYpixels) + dx);

			w = ((j * NofYpixels) + i);

			for (int s = 0; s < NofYpixels; s++)
			{ ///calculate k-space data 
				for (int p = 0; p < NofXpixels; p++)
				{


					ds = ((int)s - NofYpixels / 2);
					dp = ((int)p - NofXpixels / 2);

					k3 = ((int)(ds * NofXpixels) + dp);

					t = ((p * NofYpixels) + s);

					phase = ((double)(2.0 * pi * k2 * k3) / ((double)NofXpixels * (double)NofYpixels));

					//** nayuki.eigenstate.org/page/how-to-implement-the-discrete-fourier-transform (begin)**/
					complexR = (double)cos((double)phase) + (double)sin((double)phase);

					complexI = -(double)sin((double)phase) + (double)cos((double)phase);
					//** nayuki.eigenstate.org/page/how-to-implement-the-discrete-fourier-transform (end)**/

					*(kSpaceR + w) += (double)*(Signal + t) * (double)complexR;

					*(kSpaceI + w) += (double)*(Signal + t) * (double)complexI;

				}

			}///calculate k-space data 


		}
	} ///calculate k-space data
	///// Fourier Transform //////

	//// Procedure to scale (begin)
	double max = *(kSpaceR);
	double min = *(kSpaceR);

	/// compute max and min of data (begin)
	for (i = 0; i < rcyres; i++)
	{
		for (j = 0; j < rcxres; j++)
		{

			w = ((j * NofYpixels) + i);

			if (*(kSpaceR + w) > (double)max)

				max = (double)*(kSpaceR + w);

			if (*(kSpaceR + w) < (double)min)

				min = (double)*(kSpaceR + w);

		} // y dim

	}  // x dim
	/// compute max and min of data (end)

	for (i = 0; i < rcyres; i++)
	{
		for (j = 0; j < rcxres; j++)
		{

			w = ((j * NofYpixels) + i);

			if (max == min) *(kSpaceR + w) = (double)0.0;

			else {

				*(kSpaceR + w) = (double)fabs((min - (double)*(kSpaceR + w)) / (min - max));

				*(kSpaceR + w) *= ft_SCALE_z;

			}

		} // y dim

	}  // x dim
	//// Procedure to scale (end)

	//// Procedure to scale (begin)
	max = *(kSpaceI);
	min = *(kSpaceI);

	/// compute max and min of data (begin)
	for (i = 0; i < rcyres; i++)
	{
		for (j = 0; j < rcxres; j++)
		{

			w = ((j * NofYpixels) + i);

			if (*(kSpaceI + w) > (double)max)

				max = (double)*(kSpaceI + w);

			if (*(kSpaceI + w) < (double)min)

				min = (double)*(kSpaceI + w);

		} // y dim

	}  // x dim
	/// compute max and min of data (end)

	for (i = 0; i < rcyres; i++)
	{
		for (j = 0; j < rcxres; j++)
		{

			w = ((j * NofYpixels) + i);

			if (max == min) *(kSpaceI + w) = (double)0.0;

			else {

				*(kSpaceI + w) = (double)fabs((min - (double)*(kSpaceI + w)) / (min - max));

				*(kSpaceI + w) *= ft_SCALE_z;

			}

		} // y dim

	}  // x dim
	//// Procedure to scale (end)

	double savedata = 0.0;
	char FTfilename[128];

	sprintf(FTfilename, "%s%s", "K-SpaceR-", imageFilename);

	fprintf(logfile, "%s\t%s\n", "Now Saving K-Space Signal (Real) in File: ", FTfilename);

	if ((pf = fopen(FTfilename, "wb+")) == NULL)
	{

		fprintf(logfile, "%s\n", "Cannot open file to save K-Space Signal");


		// FIFO memory deallocation method
		free(kSpaceR);
		free(kSpaceI);
		free(Signal);

		exit(0);

	}
	else { // save data


		for (i = 0; i < NofYpixels; i++)
		{ ///save k-space data
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				savedata = (double)*(kSpaceR + index);

				fwrite(&savedata, sizeof(double), 1, pf);

			}
		} ///save k-space data

		fprintf(logfile, "%s\n", "K-Space Signal (Real) Saved");

		fclose(pf);
	} // save data



	sprintf(FTfilename, "%s%s", "K-SpaceI-", imageFilename);

	fprintf(logfile, "%s\t%s\n", "Now Saving K-Space Signal (Imaginary) in File: ", FTfilename);

	if ((pf = fopen(FTfilename, "wb+")) == NULL)
	{

		fprintf(logfile, "%s\n", "Cannot open file to save K-Space Signal");

		// FIFO memory deallocation method
		free(kSpaceR);
		free(kSpaceI);
		free(Signal);

		exit(0);

	}
	else { // save data


		for (i = 0; i < NofYpixels; i++)
		{ ///save k-space data
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				savedata = (double)*(kSpaceI + index);

				fwrite(&savedata, sizeof(double), 1, pf);

			}
		} ///save k-space data

		fprintf(logfile, "%s\n", "K-Space Signal (Imaginary) Saved");

		fclose(pf);

	} // save data

	sprintf(FTfilename, "%s%s", "K-SpaceM-", imageFilename);

	fprintf_s(logfile, "%s\t%s\n", "Now Saving K-Space Magnitude of the Signal in File: ", FTfilename);

	if ((pf = fopen(FTfilename, "wb+")) == NULL)
	{

		fprintf_s(logfile, "%s\n", "Cannot open file to save K-Space Magnitude of the Signal");

		// FIFO memory deallocation method
		free(kSpaceR);
		free(kSpaceI);
		free(Signal);

		exit(0);

	}
	else { // save data

		// K-Space Magnitude (begin)
		for (int s = 0; s < (int)NofYpixels; s++)
		{
			for (int p = 0; p < (int)NofXpixels; p++)
			{


				index = ((p * NofYpixels) + s);

				savedata = (double)sqrt((double)*(kSpaceR + index) * (double)*(kSpaceR + index) +
					(double)*(kSpaceI + index) * (double)*(kSpaceI + index));

				fwrite(&savedata, sizeof(double), 1, pf);

			}
		} // K-Space Magnitude (end)

		fprintf_s(logfile, "%s\n", "K-Space Magnitude of the Signal Saved");

		fclose(pf);
	} // save data

	printf("%s\n", "FT Processing Completed");
	fprintf_s(logfile, "%s\n", "FT Processing Completed");

	fclose(logfile);


	// FIFO memory deallocation method
	free(kSpaceR);
	free(kSpaceI);
	free(Signal);

}

void OnZTransform_scale(char imageFilename[], int rcxres, int rcyres, double m_Real, double m_Imaginary)
{

	int NofXpixels = rcxres;
	int NofYpixels = rcyres;

	int i, j, index;
	int dx, dy;
	int ds, dp;
	int k2, k3, w, t;

	double pi = 3.141592;

	double* ZSpaceR = 0;
	double* ZSpaceI = 0;
	double* Signal = 0;

	FILE* logfile;

	char logfilename[128] = "Z-T.log";

	if ((logfile = fopen(logfilename, "w+")) == NULL)
	{

		printf("%s\n %s\n", "Unable to open log File", "Now Exit");

		exit(0);

	}
	else { // allocate memory 


		if ((ZSpaceR = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			exit(0);

		}

		if ((ZSpaceI = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			free(ZSpaceR);
			exit(0);

		}

		if ((Signal = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			free(ZSpaceR);
			free(ZSpaceI);
			exit(0);

		}

	} // allocate memory 

	//// read image data and initialize pointers
	double number = 0.0;

	for (i = 0; i < NofYpixels; i++)
	{
		for (j = 0; j < NofXpixels; j++)
		{

			index = ((j * NofYpixels) + i);

			*(ZSpaceR + index) = (double)0.0;

			*(ZSpaceI + index) = (double)0.0;

		}

	}

	FILE* pf;
	char SignalFilename[128];
	double readData;

	sprintf(SignalFilename, "%s", imageFilename);

	if ((pf = fopen(SignalFilename, "rb+")) == NULL)
	{

		fprintf(logfile, "%s\n", "Cannot open file to read Signal");

		// FIFO memory deallocation method
		free(ZSpaceR);
		free(ZSpaceI);
		free(Signal);

		exit(0);

	}
	else { // read data


		for (i = 0; i < rcyres; i++)
		{ ///read signal data
			for (j = 0; j < rcxres; j++)
			{

				index = ((j * rcyres) + i);

				fread(&readData, sizeof(double), 1, pf);

				*(Signal + index) = (double)readData;

			}
		} ///read signal data

		fprintf(logfile, "%s\n", "Signal Read in DOUBLE (64bits) format");

		fclose(pf);
	} // save data


	double phase;
	double exponent, r;
	double complexRZ, complexIZ;

	double magnitude = (double)sqrt((double)m_Real * m_Real + (double)m_Imaginary * m_Imaginary);
	double Zphase = (double)2.0 * pi * atan2((double)m_Imaginary, (double)m_Real) / ((double)NofXpixels * NofYpixels);

	///// Z Transform //////
	for (i = 0; i < NofYpixels; i++)
	{ ///calculate Z-Space data

		for (j = 0; j < NofXpixels; j++)
		{


			dx = ((int)i - NofYpixels / 2);
			dy = ((int)j - NofXpixels / 2);

			k2 = ((int)(dy * NofYpixels) + dx);

			w = ((j * NofYpixels) + i);

			for (int s = 0; s < NofYpixels; s++)
			{ ///calculate Z-Space data 
				for (int p = 0; p < NofXpixels; p++)
				{


					ds = ((int)s - NofYpixels / 2);
					dp = ((int)p - NofXpixels / 2);

					k3 = ((int)(ds * NofXpixels) + dp);

					t = ((p * NofYpixels) + s);


					phase = ((double)(2.0 * pi * k2 * k3) / (NofXpixels * NofYpixels));

					exponent = (double)2.0 * pi * t * (double)Zphase / ((double)pow((double)NofXpixels * NofYpixels, 2.0));

					exponent = (double)fabs((double)exponent);

					r = (double)pow((double)magnitude, -(double)exponent);


					complexRZ = (double)cos((double)phase) + (double)sin((double)phase);

					complexIZ = -(double)sin((double)phase) + (double)cos((double)phase);


					*(ZSpaceR + w) += (double)*(Signal + t) * (double)complexRZ * (double)r;

					*(ZSpaceI + w) += (double)*(Signal + t) * (double)complexIZ * (double)r;
				}

			}///calculate Z-Space data 


		}
	} ///calculate Z-Space data
	///// Z Transform //////

	//// Procedure to scale (begin)
	double max = *(ZSpaceR);
	double min = *(ZSpaceR);

	/// compute max and min of data (begin)
	for (i = 0; i < rcyres; i++)
	{
		for (j = 0; j < rcxres; j++)
		{

			w = ((j * NofYpixels) + i);

			if (*(ZSpaceR + w) > (double)max)

				max = (double)*(ZSpaceR + w);

			if (*(ZSpaceR + w) < (double)min)

				min = (double)*(ZSpaceR + w);

		} // y dim

	}  // x dim
	/// compute max and min of data (end)

	for (i = 0; i < rcyres; i++)
	{
		for (j = 0; j < rcxres; j++)
		{

			w = ((j * NofYpixels) + i);

			if (max == min) *(ZSpaceR + w) = (double)0.0;

			else {

				*(ZSpaceR + w) = (double)fabs((min - (double)*(ZSpaceR + w)) / (min - max));

				*(ZSpaceR + w) *= ft_SCALE_z;

			}

		} // y dim

	}  // x dim
	//// Procedure to scale (end)

	//// Procedure to scale (begin)
	max = *(ZSpaceI);
	min = *(ZSpaceI);

	/// compute max and min of data (begin)
	for (i = 0; i < rcyres; i++)
	{
		for (j = 0; j < rcxres; j++)
		{

			w = ((j * NofYpixels) + i);

			if (*(ZSpaceI + w) > (double)max)

				max = (double)*(ZSpaceI + w);

			if (*(ZSpaceI + w) < (double)min)

				min = (double)*(ZSpaceI + w);

		} // y dim

	}  // x dim
	/// compute max and min of data (end)

	for (i = 0; i < rcyres; i++)
	{
		for (j = 0; j < rcxres; j++)
		{

			w = ((j * NofYpixels) + i);

			if (max == min) *(ZSpaceI + w) = (double)0.0;

			else {

				*(ZSpaceI + w) = (double)fabs((min - (double)*(ZSpaceI + w)) / (min - max));

				*(ZSpaceI + w) *= ft_SCALE_z;

			}

		} // y dim

	}  // x dim
	//// Procedure to scale (end)


	double savedata = 0.0;
	char Zfilename[128];

	sprintf(Zfilename, "%s%s", "Z-SpaceR-", imageFilename);

	fprintf(logfile, "%s\t%s\n", "Now Saving Z-Space Signal (Real) in File: ", Zfilename);

	if ((pf = fopen(Zfilename, "wb+")) == NULL)
	{

		fprintf(logfile, "%s\n", "Cannot open file to save Z-Space Signal");


		// FIFO memory deallocation method
		free(ZSpaceR);
		free(ZSpaceI);
		free(Signal);

		exit(0);

	}
	else { // save data


		for (i = 0; i < NofYpixels; i++)
		{ ///save Z-Space data
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				savedata = (double)*(ZSpaceR + index);

				fwrite(&savedata, sizeof(double), 1, pf);

			}
		} ///save Z-Space data

		fprintf(logfile, "%s\n", "Z-Space Signal (Real) Saved");

		fclose(pf);
	} // save data



	sprintf(Zfilename, "%s%s", "Z-SpaceI-", imageFilename);

	fprintf(logfile, "%s\t%s\n", "Now Saving Z-Space Signal (Imaginary) in File: ", Zfilename);

	if ((pf = fopen(Zfilename, "wb+")) == NULL)
	{

		fprintf(logfile, "%s\n", "Cannot open file to save Z-Space Signal");

		// FIFO memory deallocation method
		free(ZSpaceR);
		free(ZSpaceI);
		free(Signal);

		exit(0);

	}
	else { // save data


		for (i = 0; i < NofYpixels; i++)
		{ ///save Z-Space data
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				savedata = (double)*(ZSpaceI + index);

				fwrite(&savedata, sizeof(double), 1, pf);

			}
		} ///save Z-Space data

		fprintf(logfile, "%s\n", "Z-Space Signal (Imaginary) Saved");

		fclose(pf);

	} // save data

	sprintf(Zfilename, "%s%s", "Z-SpaceM-", imageFilename);

	fprintf_s(logfile, "%s\t%s\n", "Now Saving Z-Space Magnitude of the Signal in File: ", Zfilename);

	if ((pf = fopen(Zfilename, "wb+")) == NULL)
	{

		fprintf_s(logfile, "%s\n", "Cannot open file to save Z-Space Magnitude of the Signal");

		// FIFO memory deallocation method
		free(ZSpaceR);
		free(ZSpaceI);
		free(Signal);

		exit(0);

	}
	else { // save data	

		// save a zero image (begin)
		for (int s = 0; s < NofYpixels; s++)
		{
			for (int p = 0; p < NofXpixels; p++)
			{

				savedata = (double)0.0;

				fwrite(&savedata, sizeof(double), 1, pf);

			}
		} // save a zero image (end)

		fclose(pf);

	}

	if ((pf = fopen(Zfilename, "wb+")) == NULL)
	{

		fprintf_s(logfile, "%s\n", "Cannot open file to save Z-Space Magnitude of the Signal");

		// FIFO memory deallocation method
		free(ZSpaceR);
		free(ZSpaceI);
		free(Signal);

		exit(0);

	}
	else { // save data

		// Z-Space Magnitude (begin)
		for (int s = 0; s < (int)NofYpixels; s++)
		{
			for (int p = 0; p < (int)NofXpixels; p++)
			{


				index = ((p * NofYpixels) + s);

				savedata = (double)sqrt((double)*(ZSpaceR + index) * (double)*(ZSpaceR + index) +
					(double)*(ZSpaceI + index) * (double)*(ZSpaceI + index));

				fwrite(&savedata, sizeof(double), 1, pf);

			}
		} // Z-Space Magnitude (end)

		fprintf_s(logfile, "%s\n", "Z-Space Magnitude of the Signal Saved");

		fclose(pf);
	} // save data

	printf("%s\n", "Z Processing Completed");
	fprintf_s(logfile, "%s\n", "Z Processing Completed");

	fclose(logfile);


	// FIFO memory deallocation method
	free(ZSpaceR);
	free(ZSpaceI);
	free(Signal);

}


void OnTransferFunctionBesselKspace(char filterType, int rcxres, int rcyres, int n_poles, double m_cof, double m_Bandwidth, double m_TheSamplingRate)
{

	int NofXpixels = rcxres;
	int NofYpixels = rcyres;

	int i, j, index;
	double pi = 3.141592;

	double* tf = 0;

	FILE* logfile;

	char logfilename[128] = "TF.log";

	if ((logfile = fopen(logfilename, "w+")) == NULL)
	{

		printf("%s\n %s\n", "Unable to open log File", "Now Exit");

		exit(0);

	}
	else { // allocate memory 


		if ((tf = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			exit(0);

		}

	} // allocate memory 

	struct BesselPolynomial {

		double** ak; // pointer to the matrix entry

	}*BesselPolynomial_pointer; // pointer to the matrices


	BesselPolynomial_pointer = new BesselPolynomial;


	BesselPolynomial_pointer->ak = new double* [n_poles + 1];


	for (int v = 0; v <= n_poles; v++) { // (1)

		BesselPolynomial_pointer->ak[v] = new double[1];

	} // (1) allocate struct 'data' (end)


	  // (2) initialize (begin)
	for (int v = 0; v <= n_poles; v++) { // (a)

		for (int f = 0; f < 1; f++) { // (b)

			BesselPolynomial_pointer->ak[v][f] = (double)1.0;

		} //(b)

	} //(a)
	// (2) initialize (end)


	double product_b = 1.0;
	double product_c = 1.0;
	double product_d = 1.0;
	double product_e = 1.0;

	for (int k = 0; k <= n_poles; k++)
	{

		// STEP 1: Calculate ' ((double)2.0 * n_poles - (double)k) ' factorial; 
		// and assign the result to ' product_b '
		// Hint: if ((double)2.0 * n_poles - (double)k) == 0 then
		// ' product_b *= (double)1.0; '
		if (((double)2.0 * n_poles - (double)k) == 0)

			product_b *= (double)1.0;

		else  product_b *= ((double)2.0 * n_poles - (double)k);

		// STEP 2: Calculate ' (double)pow((double)2.0, ((double)n_poles - k)) '
		// and assign the value to ' product_c '
		product_c = (double)pow((double)2.0, ((double)n_poles - k));

		// STEP 3: Calculate ' k ' factorial
		// and assign the value to ' product_d '
		// Hint: ' if (k == 0) product_d *= (double)1.0; '
		if (k == 0) product_d *= (double)1.0;
		else          product_d *= (double)k;


		// STEP 4: Calculate ' ((double)n_poles - k) ' factorial 
		// and assign the value to ' product_e ' 
		// Hint: this factor ' product_e ' can be zero as last multiplier of the
		// factorial series, and as such is 0! = 1.
		if (n_poles == k)

			product_e *= (double)1.0;

		else	product_e *= ((double)n_poles - k);
		// this factor ' product_e ' can be zero as last multiplier of the
		// factorial series, and as such is 0! = 1.

		if (((double)product_c * product_d * product_e) != 0.0)

			// STEP 5: Calculate ' BesselPolynomial_pointer->ak[k][0] ' as per Wikipedia notes
			// Use the terms: product_b, product_c, product_d, product_e; to compose the ratio
			// ' BesselPolynomial_pointer->ak[k][0] '
			BesselPolynomial_pointer->ak[k][0] = (double)product_b / ((double)product_c * product_d * product_e);

		else BesselPolynomial_pointer->ak[k][0] = (double)1.0;

	}


	FILE* pf;
	char filename[328];
	double savedata = 0;
	double savedata2 = 0;

	sprintf(filename, "%s", "Bessel.img");

	fprintf_s(logfile, "%s\t%s\n", "Now Saving Bessel Space in File: ", filename);

	if ((pf = fopen(filename, "wb+")) == NULL)
	{

		fprintf_s(logfile, "%s\n", "Cannot open file to save Bessel Space Signal");

		free(tf);
		delete BesselPolynomial_pointer;

		exit(0);

	}
	else { // save data


		for (i = 0; i <= n_poles; i++)
		{ ///save Bessel-Polynomial coefficients data

			savedata = (double)BesselPolynomial_pointer->ak[i][0];

			fwrite(&savedata, sizeof(double), 1, pf);

		}///save Bessel-Polynomial coefficients data

		fprintf_s(logfile, "%s\n", "Bessel-Polynomial coefficients Saved");

		fclose(pf);
	} // save data


		//// initialize pointer
	for (i = 0; i < NofYpixels; i++)
	{
		for (j = 0; j < NofXpixels; j++)
		{

			index = ((j * NofYpixels) + i);

			*(tf + index) = (double)0.0;

		}

	}


	double Bessel_Polynomial;
	double Bessel_Polynomial_Zero;
	double complex;

	int t;
	int di, dj;
	double dx, dy;
	double dtx, dty;
	double sincx, sincy;
	double toll = 0.000001;


	for (i = 0; i < NofYpixels; i++)
	{ ///calculate TF-Space data

		for (j = 0; j < NofXpixels; j++)
		{

			di = ((int)i - NofYpixels / 2);
			dj = ((int)j - NofXpixels / 2);

			dty = ((double)((double)i - NofYpixels / 2) * m_TheSamplingRate); //space (pixel) * frequency (1/sec)
			dtx = ((double)((double)j - NofXpixels / 2) * m_TheSamplingRate); //space (pixel) * frequency (1/sec)


			dy = (double)pi * (2.0 * m_Bandwidth * di - dty);
			// Where 2.0 * bandwidth * di is: 
			// frequency (1/sec) * space (pixel)
			dx = (double)pi * (2.0 * m_Bandwidth * dj - dtx);
			//		 2.0 * bandwidth * dj is: 
			// frequency (1/sec) * space (pixel)


			t = ((j * NofYpixels) + i);

			// STEP 6: Use the variable ' complex ' and assign to it the following multiplication:
			// 2.0 * pi * the square root of the sum between: 1. the second power of ' dx '; 
			// and 2. the second power of ' dy '. Then divide all by the factor: 
			// (double) pow(((double)NofXpixels*NofYpixels), ((double)1.0/m_cof));
			complex = (double)2.0 * pi * ((double)sqrt((double)(dx * dx + dy * dy))) /
				(double)pow(((double)NofXpixels * NofYpixels), 1.0 / m_cof);


			Bessel_Polynomial = 0.0;

			for (int k = 0; k <= n_poles; k++)
			{

				// m_cof != 0.0 as per data input into the program
				Bessel_Polynomial += ((double)BesselPolynomial_pointer->ak[k][0] *
					(double)pow(((double)complex / m_cof), (double)k));

			}


			Bessel_Polynomial_Zero = (double)BesselPolynomial_pointer->ak[0][0];

			// STEP 7: Check if ' fabs((double)dx) ' is greater than the tolerance: ' toll '
			// If so, then calculate the sinc function of ' dx ' and assign the value to
			// the variable ' sincx '. If, not set ' sincx = (double)1.0; ' Note that
			// ' sincx ' is the sinc function in ' x '
			if ((double)fabs((double)dx) > toll)

				sincx = (double)sin((double)dx) / ((double)dx);

			else sincx = (double)1.0;

			// STEP 8: Check if ' fabs((double)dy) ' is greater than the tolerance: ' toll '
			// If so, then calculate the sinc function of ' dy ' and assign the value to
			// the variable ' sincy '. If, not set ' sincy = (double)1.0; ' Note that
			// ' sincy ' is the sinc function in ' y '
			if ((double)fabs((double)dy) > toll)

				sincy = (double)sin((double)dy) / ((double)dy);

			else sincy = (double)1.0;



			if (((double)Bessel_Polynomial) != 0.0) {

				if ((_isnan(((double)Bessel_Polynomial_Zero / Bessel_Polynomial))) == 0) {

					// STEP 9: Calculate the transfer function: *(tf + t) as the ratio
					// between ' Bessel_Polynomial_Zero ' and ' Bessel_Polynomial '
					*(tf + t) = ((double)Bessel_Polynomial_Zero / Bessel_Polynomial);

					// STEP 10: build two-dimensional Sinc function and assign the value to the pointer ' *(tf + t) '
					// Hint: Multiply the Transfer Function ' *(tf + t) 'by the sinc functions in x and y: ((double)sincx * sincy).
					// Then assign the resulting multiplication to: ' *(tf + t) '
					*(tf + t) *= ((double)sincx * sincy);

				}
				else { *(tf + t) = (double)0.0; }

			}
			else { *(tf + t) = (double)0.0; }


		}

	}///calculate TF-Space data 
	///// TF //////


	//// Procedure to scale the filter (begin)
	// scale tf (begin) 
	double max = *(tf);
	double min = *(tf);

	/// compute max and min of data (begin)
	for (i = 0; i < rcyres; i++)
	{
		for (j = 0; j < rcxres; j++)
		{

			index = ((j * rcyres) + i);

			if (*(tf + index) > (double)max)

				max = (double)*(tf + index);

			if (*(tf + index) < (double)min)

				min = (double)*(tf + index);

		} // y dim

	}  // x dim
	/// compute max and min of data (end)

	if (tolower(filterType) == 'l') {

		// scale (begin)
		for (i = 0; i < rcyres; i++)
		{
			for (j = 0; j < rcxres; j++)
			{

				index = ((j * rcyres) + i);

				if (max == min) *(tf + index) = (double)0.0;

				else {

					*(tf + index) = (double)fabs((min - (double)*(tf + index)) / (min - max));

					*(tf + index) *= ft_SCALE;

				}

			} // y dim

		}  // x dim

	}
	else if (tolower(filterType) == 'h') {

		// scale (begin)
		for (i = 0; i < rcyres; i++)
		{
			for (j = 0; j < rcxres; j++)
			{

				index = ((j * rcyres) + i);

				if (max == min) *(tf + index) = (double)0.0;

				else {

					// HP = 1.0 - LP
					*(tf + index) = (double)1.0 - (double)fabs((min - (double)*(tf + index)) / (min - max));

					*(tf + index) *= ft_SCALE;

				}

			} // y dim

		}  // x dim
		// scale (end)
	} //// Procedure to scale the filter (end)


	sprintf(filename, "%s", "TF.img");

	fprintf(logfile, "%s\t%s\n", "Now Saving TF in File: ", filename);

	if ((pf = fopen(filename, "wb+")) == NULL)
	{

		fprintf(logfile, "%s\n", "Cannot open file to save TF");

		// FIFO memory deallocation method
		free(tf);
		delete BesselPolynomial_pointer;

		exit(0);

	}
	else { // save data


		for (i = 0; i < NofYpixels; i++)
		{ ///save TF data
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				savedata = (double)*(tf + index);

				fwrite(&savedata, sizeof(double), 1, pf);

			}
		} ///save Z-Space data

		fprintf(logfile, "%s\n", "TF Saved");

		fclose(pf);
	} // save data


	printf("%s\n", "TF Processing Completed");
	fprintf_s(logfile, "%s\n", "TF Processing Completed");

	fclose(logfile);


	// FIFO memory deallocation method
	free(tf);
	delete BesselPolynomial_pointer;

}

void OnTransferFunctionBesselZspace(char filterType, int rcxres, int rcyres, int n_poles, double m_cof, double m_Bandwidth, double m_TheSamplingRate, double m_Real, double m_Imaginary)
{

	int NofXpixels = rcxres;
	int NofYpixels = rcyres;

	int i, j, index;
	double pi = 3.141592;

	double* tf = 0;

	FILE* logfile;

	char logfilename[128] = "TF.log";

	if ((logfile = fopen(logfilename, "w+")) == NULL)
	{

		printf("%s\n %s\n", "Unable to open log File", "Now Exit");

		exit(0);

	}
	else { // allocate memory 


		if ((tf = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			exit(0);

		}

	} // allocate memory 

	struct BesselPolynomial {

		double** ak; // pointer to the matrix entry

	}*BesselPolynomial_pointer; // pointer to the matrices


	BesselPolynomial_pointer = new BesselPolynomial;


	BesselPolynomial_pointer->ak = new double* [n_poles + 1];


	for (int v = 0; v <= n_poles; v++) { // (1)

		BesselPolynomial_pointer->ak[v] = new double[1];

	} // (1) allocate struct 'data' (end)


	  // (2) initialize (begin)
	for (int v = 0; v <= n_poles; v++) { // (a)

		for (int f = 0; f < 1; f++) { // (b)

			BesselPolynomial_pointer->ak[v][f] = (double)1.0;

		} //(b)

	} //(a)
	// (2) initialize (end)


	double product_b = 1.0;
	double product_c = 1.0;
	double product_d = 1.0;
	double product_e = 1.0;

	for (int k = 0; k <= n_poles; k++)
	{

		if (((double)2.0 * n_poles - (double)k) == 0)

			product_b *= (double)1.0;

		else  product_b *= ((double)2.0 * n_poles - (double)k);


		product_c = (double)pow((double)2.0, ((double)n_poles - k));


		if (k == 0) product_d *= (double)1.0;
		else          product_d *= (double)k;

		// this factor ' product_e ' can be zero as last multiplier of the
		// factorial series, and as such is 0! = 1.
		if (n_poles == k)

			product_e *= (double)1.0;

		else	product_e *= ((double)n_poles - k);
		// this factor ' product_e ' can be zero as last multiplier of the
		// factorial series, and as such is 0! = 1.

		if (((double)product_c * product_d * product_e) != 0.0)

			BesselPolynomial_pointer->ak[k][0] = (double)product_b / ((double)product_c * product_d * product_e);

		else BesselPolynomial_pointer->ak[k][0] = (double)1.0;

	}



	FILE* pf;
	char filename[328];
	double savedata = 0;
	double savedata2 = 0;

	sprintf(filename, "%s", "Bessel.img");

	fprintf_s(logfile, "%s\t%s\n", "Now Saving Bessel Space in File: ", filename);

	if ((pf = fopen(filename, "wb+")) == NULL)
	{

		fprintf_s(logfile, "%s\n", "Cannot open file to save Bessel Space Signal");

		free(tf);
		delete BesselPolynomial_pointer;

		exit(0);

	}
	else { // save data


		for (i = 0; i <= n_poles; i++)
		{ ///save Bessel-Polynomial coefficients data

			savedata = (double)BesselPolynomial_pointer->ak[i][0];

			fwrite(&savedata, sizeof(double), 1, pf);

		}///save Bessel-Polynomial coefficients data

		fprintf_s(logfile, "%s\n", "Bessel-Polynomial coefficients Saved");

		fclose(pf);
	} // save data


		//// initialize pointer
	for (i = 0; i < NofYpixels; i++)
	{
		for (j = 0; j < NofXpixels; j++)
		{

			index = ((j * NofYpixels) + i);

			*(tf + index) = (double)0.0;

		}

	}


	double Bessel_Polynomial;
	double Bessel_Polynomial_Zero;
	double complex;

	int t;
	int di, dj;
	double dx, dy;
	double dtx, dty;
	double sincx, sincy;
	double toll = 0.000001;


	for (i = 0; i < NofYpixels; i++)
	{ ///calculate TF-Space data

		for (j = 0; j < NofXpixels; j++)
		{

			di = ((int)i - NofYpixels / 2);
			dj = ((int)j - NofXpixels / 2);

			dty = ((double)((int)i - NofYpixels / 2) * m_TheSamplingRate); //space (pixel) * frequency (1/sec)
			dtx = ((double)((int)j - NofXpixels / 2) * m_TheSamplingRate); //space (pixel) * frequency (1/sec)

			/// Whittaker-Shannon interpolation formula 2010
			dy = (double)pi * (2.0 * m_Bandwidth * di - dty);
			// Where 2.0 * bandwidth * di is: 
			// frequency (1/sec) * space (pixel)
			dx = (double)pi * (2.0 * m_Bandwidth * dj - dtx);
			//		 2.0 * bandwidth * dj is: 
			// frequency (1/sec) * space (pixel)
			/// Whittaker-Shannon interpolation formula 2010

			t = ((j * NofYpixels) + i);


			double phase = (double)2.0 * pi * atan2((double)m_Imaginary, (double)m_Real);

			double magnitude = (double)sqrt((double)(((double)dx * dx) + ((double)dy * dy)));

			double complexR = (double)magnitude * (double)cos((double)phase);
			double complexI = (double)magnitude * (double)sin((double)phase);

			double samplingPeriod = 2.0 * pi * (double)pow(((double)NofXpixels * NofYpixels), 1.0 / m_cof);

			double T = ((double)m_cof) / ((double)tan((double)m_cof * samplingPeriod / 2.0));

			// bilinear transform to change the variable from 's' to 'z'
			complex = ((double)2.0 / T) * ((double)complexR + complexI - 1.0) / ((double)complexR + complexI + 1.0);


			Bessel_Polynomial = 0.0;

			for (int k = 0; k <= n_poles; k++)
			{

				// m_cof != 0.0 as per data input into the program
				Bessel_Polynomial += ((double)BesselPolynomial_pointer->ak[k][0] *
					(double)pow(((double)complex / m_cof), (double)k));

			}


			Bessel_Polynomial_Zero = 0.0;

			for (int k = 0; k <= n_poles; k++)
			{

				Bessel_Polynomial_Zero += (double)BesselPolynomial_pointer->ak[k][0] *
					(double)pow(((double)0.0), (double)k);

			}


			///// Wittaker-Shannon sampling 2010//////
			if ((double)fabs((double)dx) > toll)

				sincx = (double)sin((double)dx) / ((double)dx);

			else sincx = (double)1.0;

			if ((double)fabs((double)dy) > toll)

				sincy = (double)sin((double)dy) / ((double)dy);

			else sincy = (double)1.0;
			///// Wittaker-Shannon sampling 2010//////


			if (((double)Bessel_Polynomial) != 0.0) {

				if ((_isnan(((double)Bessel_Polynomial_Zero / Bessel_Polynomial))) == 0) {

					*(tf + t) = ((double)Bessel_Polynomial_Zero / Bessel_Polynomial);

					*(tf + t) *= ((double)sincx * sincy);

				}
				else { *(tf + t) = (double)0.0; }

			}
			else { *(tf + t) = (double)0.0; }


		}

	}///calculate TF-Space data 
	///// TF //////


		 //// Procedure to scale the filter (begin)
		 // scale tf (begin) 
	double max = *(tf);
	double min = *(tf);

	/// compute max and min of data (begin)
	for (i = 0; i < rcyres; i++)
	{
		for (j = 0; j < rcxres; j++)
		{

			index = ((j * rcyres) + i);

			if (*(tf + index) > (double)max)

				max = (double)*(tf + index);

			if (*(tf + index) < (double)min)

				min = (double)*(tf + index);

		} // y dim

	}  // x dim
	/// compute max and min of data (end)

	if (tolower(filterType) == 'l') {

		// scale (begin)
		for (i = 0; i < rcyres; i++)
		{
			for (j = 0; j < rcxres; j++)
			{

				index = ((j * rcyres) + i);

				if (max == min) *(tf + index) = (double)0.0;

				else {

					*(tf + index) = (double)fabs((min - (double)*(tf + index)) / (min - max));

					*(tf + index) *= ft_SCALE;

				}

			} // y dim

		}  // x dim

	}
	else if (tolower(filterType) == 'h') {

		// scale (begin)
		for (i = 0; i < rcyres; i++)
		{
			for (j = 0; j < rcxres; j++)
			{

				index = ((j * rcyres) + i);

				if (max == min) *(tf + index) = (double)0.0;

				else {

					// HP = 1.0 - LP
					*(tf + index) = (double)1.0 - (double)fabs((min - (double)*(tf + index)) / (min - max));

					*(tf + index) *= ft_SCALE;

				}

			} // y dim

		}  // x dim
		// scale (end)
	} //// Procedure to scale the filter (end)



	sprintf(filename, "%s", "TF.img");

	fprintf(logfile, "%s\t%s\n", "Now Saving TF in File: ", filename);

	if ((pf = fopen(filename, "wb+")) == NULL)
	{

		fprintf(logfile, "%s\n", "Cannot open file to save TF");

		// FIFO memory deallocation method
		free(tf);
		delete BesselPolynomial_pointer;

		exit(0);

	}
	else { // save data


		for (i = 0; i < NofYpixels; i++)
		{ ///save TF data
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				savedata = (double)*(tf + index);

				fwrite(&savedata, sizeof(double), 1, pf);

			}
		} ///save Z-Space data

		fprintf(logfile, "%s\n", "TF Saved");

		fclose(pf);
	} // save data


	printf("%s\n", "TF Processing Completed");
	fprintf_s(logfile, "%s\n", "TF Processing Completed");

	fclose(logfile);


	// FIFO memory deallocation method
	free(tf);
	delete BesselPolynomial_pointer;

}

void OnTransferFunctionButterworthZspace(char filterType, int rcxres, int rcyres, int n_poles, double m_cof, double Gain, double m_Bandwidth, double m_TheSamplingRate, double m_Real, double m_Imaginary)
{

	int Order = n_poles;
	int NofXpixels = rcxres;
	int NofYpixels = rcyres;

	int i, j, index, t;

	double pi = 3.141592;

	double* tf = 0;

	FILE* logfile;

	char logfilename[128] = "TF.log";

	if ((logfile = fopen(logfilename, "w+")) == NULL)
	{

		printf("%s\n %s\n", "Unable to open log File", "Now Exit");

		exit(0);

	}
	else { // allocate memory 


		if ((tf = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			exit(0);

		}

	} // allocate memory 


	struct ButterworthPolynomial {

		double** Bw_Odd; // pointer to the matrix entry

		double** Bw_Even; // pointer to the matrix entry

		double** BW; // pointer to the matrix entry

	}*Butterworth_Polynomial_pointer; // pointer to the matrices


	// (1) allocate struct (begin)

	Butterworth_Polynomial_pointer = new ButterworthPolynomial;


	Butterworth_Polynomial_pointer->Bw_Odd = new double* [Order + 1];

	Butterworth_Polynomial_pointer->Bw_Even = new double* [Order + 1];

	Butterworth_Polynomial_pointer->BW = new double* [Order + 1];


	for (int v = 0; v <= Order; v++) { // (1)

		Butterworth_Polynomial_pointer->Bw_Odd[v] = new double[1];

		Butterworth_Polynomial_pointer->Bw_Even[v] = new double[1];

		Butterworth_Polynomial_pointer->BW[v] = new double[1];

	} // (1) allocate struct 'data' (end)


	  // (2) initialize (begin)
	for (int v = 0; v <= Order; v++) { // (a)

		for (int f = 0; f < 1; f++) { // (b)

			Butterworth_Polynomial_pointer->Bw_Odd[v][f] = (double)0.0;

			Butterworth_Polynomial_pointer->Bw_Even[v][f] = (double)0.0;

			Butterworth_Polynomial_pointer->BW[v][f] = (double)0.0;

		} //(b)

	} //(a)
   // (2) initialize (end)

   //// initialize pointer
	for (i = 0; i < NofYpixels; i++)
	{
		for (j = 0; j < NofXpixels; j++)
		{

			index = ((j * NofYpixels) + i);

			*(tf + index) = (double)0.0;

		}

	}


	double Bw_term = 1.0;
	double complex; // complex variable of the filter TF 
	int k;

	int di, dj;
	double dx, dy;
	double dtx, dty;
	double sincx, sincy;
	double toll = 0.000001;


	for (i = 0; i < NofYpixels; i++)
	{ ///calculate TF-Space data

		for (j = 0; j < NofXpixels; j++)
		{

			di = ((int)i - NofYpixels / 2);
			dj = ((int)j - NofXpixels / 2);

			dty = ((double)((int)i - NofYpixels / 2) * m_TheSamplingRate); //space (pixel) * frequency (1/sec)
			dtx = ((double)((int)j - NofXpixels / 2) * m_TheSamplingRate); //space (pixel) * frequency (1/sec)

			/// Whittaker-Shannon interpolation formula 2010
			dy = (double)pi * (2.0 * m_Bandwidth * di - dty);
			// Where 2.0 * bandwidth * di is: 
			// frequency (1/sec) * space (pixel)
			dx = (double)pi * (2.0 * m_Bandwidth * dj - dtx);
			//		 2.0 * bandwidth * dj is: 
			// frequency (1/sec) * space (pixel)
			/// Whittaker-Shannon interpolation formula 2010

			t = ((j * NofYpixels) + i);

			double phase = (double)2.0 * pi * atan2((double)m_Imaginary, (double)m_Real);

			double magnitude = (double)sqrt((double)(((double)dx * dx) + ((double)dy * dy)));

			double complexR = (double)magnitude * (double)cos((double)phase);
			double complexI = (double)magnitude * (double)sin((double)phase);

			double samplingPeriod = 2.0 * pi * (double)pow(((double)NofXpixels * NofYpixels), 1.0 / m_cof);

			double T = ((double)m_cof) / ((double)tan((double)m_cof * samplingPeriod / 2.0));

			// bilinear transform to change the variable from 's' to 'z'
			complex = ((double)2.0 / T) * ((double)complexR + complexI - 1.0) / ((double)complexR + complexI + 1.0);

			// Wikipedia Notes
			complex /= (double)m_cof;

			k = 0;

			if (Order % 2 == 0)
			{ // Butterworth (even)

				for (int o = 1; o <= ((int)Order / 2); o++)
				{

					k++;

					Bw_term = (double)complex * complex - ((double)2.0 * complex *

						cos(pi * ((double)(2.0 * k + Order - 1.0) / (2.0 * Order))) + 1.0);

					Butterworth_Polynomial_pointer->Bw_Even[o][0] = ((double)Bw_term);

				}

			}// Butterworth (even)


			k = 0;


			if (Order % 2 != 0)
			{ // Butterworth (odd)


				for (int o = 1; o <= ((int)(Order - 1) / 2); o++)
				{

					k++;

					double Bw_term = ((double)complex + 1.0) * ((double)complex * complex - 2.0 * complex *

						cos(pi * ((double)(2.0 * k + Order - 1.0) / (2.0 * Order))) + 1.0);

					Butterworth_Polynomial_pointer->Bw_Odd[o][0] = ((double)Bw_term);

				}

			} // Butterworth Odd


			// calculate Butterworth polynomial (begin)
			double Butterworth_Polynomial = 1.0;

			if (Order % 2 == 0)
			{ // Butterworth (even)

				for (int p = 1; p <= ((int)Order / 2); p++)

					Butterworth_Polynomial *= ((double)Butterworth_Polynomial_pointer->Bw_Even[p][0]);

			}
			else if (Order % 2 != 0) // Butterworth (odd)
			{

				for (int p = 1; p <= ((int)(Order - 1) / 2); p++)

					Butterworth_Polynomial *= ((double)Butterworth_Polynomial_pointer->Bw_Odd[p][0]);

			}
			// calculate Butterworth polynomial (end)	


				///// Wittaker-Shannon sampling 2010//////
			if ((double)fabs((double)dx) > toll)

				sincx = (double)sin((double)dx) / ((double)dx);

			else sincx = (double)1.0;

			if ((double)fabs((double)dy) > toll)

				sincy = (double)sin((double)dy) / ((double)dy);

			else sincy = (double)1.0;
			///// Wittaker-Shannon sampling 2010//////


			if (((double)Butterworth_Polynomial) != 0.0) {

				if ((_isnan((double)Gain / Butterworth_Polynomial)) == 0) {

					*(tf + t) = ((double)Gain / Butterworth_Polynomial);

					*(tf + t) *= ((double)sincx * sincy);

				}
				else { *(tf + t) = (double)0.0; }

			}
			else { *(tf + t) = (double)0.0; }

		}

	}///calculate TF-Space data 
	///// TF //////


	 //// Procedure to scale the filter (begin)
	 // scale tf (begin) 
	double max = *(tf);
	double min = *(tf);

	/// compute max and min of data (begin)
	for (i = 0; i < rcyres; i++)
	{
		for (j = 0; j < rcxres; j++)
		{

			index = ((j * rcyres) + i);

			if (*(tf + index) > (double)max)

				max = (double)*(tf + index);

			if (*(tf + index) < (double)min)

				min = (double)*(tf + index);

		} // y dim

	}  // x dim
	/// compute max and min of data (end)

	if (tolower(filterType) == 'l') {

		// scale (begin)
		for (i = 0; i < rcyres; i++)
		{
			for (j = 0; j < rcxres; j++)
			{

				index = ((j * rcyres) + i);

				if (max == min) *(tf + index) = (double)0.0;

				else {

					*(tf + index) = (double)fabs((min - (double)*(tf + index)) / (min - max));

					*(tf + index) *= ft_SCALE;

				}

			} // y dim

		}  // x dim

	}
	else if (tolower(filterType) == 'h') {

		// scale (begin)
		for (i = 0; i < rcyres; i++)
		{
			for (j = 0; j < rcxres; j++)
			{

				index = ((j * rcyres) + i);

				if (max == min) *(tf + index) = (double)0.0;

				else {

					// HP = 1.0 - LP
					*(tf + index) = (double)1.0 - (double)fabs((min - (double)*(tf + index)) / (min - max));

					*(tf + index) *= ft_SCALE;

				}

			} // y dim

		}  // x dim
		// scale (end)
	} //// Procedure to scale the filter (end)

	char filename[328];
	FILE* pf;

	sprintf(filename, "%s", "TF.img");

	fprintf(logfile, "%s\t%s\n", "Now Saving TF in File: ", filename);

	if ((pf = fopen(filename, "wb+")) == NULL)
	{

		fprintf(logfile, "%s\n", "Cannot open file to save TF");

		// FIFO memory deallocation method
		free(tf);
		delete Butterworth_Polynomial_pointer;

		exit(0);

	}
	else { // save data

		double savedata = 0.0;

		for (i = 0; i < NofYpixels; i++)
		{ ///save TF data
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				savedata = (double)*(tf + index);

				fwrite(&savedata, sizeof(double), 1, pf);

			}
		} ///save Z-Space data

		fprintf(logfile, "%s\n", "TF Saved");

		fclose(pf);
	} // save data


	printf("%s\n", "TF Processing Completed");
	fprintf_s(logfile, "%s\n", "TF Processing Completed");

	fclose(logfile);


	// FIFO memory deallocation method
	free(tf);
	delete Butterworth_Polynomial_pointer;

}

void OnTransferFunctionChebyshevZspace(char filterType, int rcxres, int rcyres, int n_poles, double m_cof, double m_eps, double m_Bandwidth, double m_TheSamplingRate, double m_Real, double m_Imaginary)
{

	int NofXpixels = rcxres;
	int NofYpixels = rcyres;

	int i, j, index;

	double pi = 3.141592;

	double* tf = 0;

	FILE* logfile;

	char filenamelog[128] = "Chebyshev.log";

	if ((logfile = fopen(filenamelog, "w+")) == NULL)
	{

		fprintf(logfile, "%s\n", "Unable to open log file, now exit...");
		exit(0);

	}
	else { // run the Filtering Process (begin) 


		if ((tf = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			exit(0);

		}

		struct ChebyshevPoles {

			double* spmR; // pointer to the matrix entry

			double* spmI; // pointer to the matrix entry

			double* spm; // pointer to the matrix entry

		}*Chebyshev_pointer; // pointer to the matrices


		// (1) allocate struct 'data' (begin)
		Chebyshev_pointer = new ChebyshevPoles;

		Chebyshev_pointer->spmR = new double[n_poles + 1];

		Chebyshev_pointer->spmI = new double[n_poles + 1];

		Chebyshev_pointer->spm = new double[n_poles + 1];


		// (2) initialize (begin)
		for (int v = 1; v <= n_poles; v++) { // (a)

			Chebyshev_pointer->spmR[v] = (double)0.0;

			Chebyshev_pointer->spmI[v] = (double)0.0;

			Chebyshev_pointer->spm[v] = (double)0.0;

		} //(a)
	   // (2) initialize (end)

		double eps = 1.0 / m_eps; // ripple factor is m_eps
		// cannot be zero as per data input to the program

		// calculate Chebyshev Space
		for (i = 1; i <= n_poles; i++)
		{

			double thetam = (pi / 2.0) * (2 * i - 1.0) / (n_poles);

			// calculate negative pole only to use in the transfer function
			double argument = ((double)1.0 / (n_poles)) *

				log((double)eps + sqrt(((double)eps * eps) + 1.0));


			double sinh = ((double)exp((double)argument) - (double)exp((double)-argument)) / 2.0;

			double cosh = ((double)exp((double)argument) + (double)exp((double)-argument)) / 2.0;

			Chebyshev_pointer->spmR[i] = -(double)sinh * (double)sin((double)thetam);

			Chebyshev_pointer->spmI[i] = (double)cosh * (double)cos((double)thetam);

		}

		FILE* opf;
		char ofilename[328];
		double savedata;

		sprintf(ofilename, "%s", "ChebyshevR.img");

		fprintf(logfile, "%s\t%s\n", "Now Saving Chebyshev Real Space in File: ", ofilename);

		if ((opf = fopen(ofilename, "wb+")) == NULL)
		{

			fprintf(logfile, "%s\n", "Cannot open file to save Chebyshev Real Space");

			free(tf);
			delete Chebyshev_pointer;

			exit(0);

		}
		else { // save data


			for (i = 1; i <= n_poles; i++)
			{ ///save Chebyshev-space data

				savedata = (double)Chebyshev_pointer->spmR[i];

				fwrite(&savedata, sizeof(double), 1, opf);


			} ///save Chebyshev-space data

			fprintf(logfile, "%s\n", "Chebyshev Real Space Saved");

			fclose(opf);
		} // save data


		sprintf(ofilename, "%s", "ChebyshevI.img");

		fprintf(logfile, "%s\t%s\n", "Now Saving Chebyshev Imaginary Space in File: ", ofilename);

		if ((opf = fopen(ofilename, "wb+")) == NULL)
		{

			fprintf(logfile, "%s\n", "Cannot open file to save Chebyshev Imaginary Space");

			free(tf);
			delete Chebyshev_pointer;

			exit(0);

		}
		else { // save data


			for (i = 1; i <= n_poles; i++)
			{ ///save Chebyshev-space data

				savedata = (double)Chebyshev_pointer->spmI[i];

				fwrite(&savedata, sizeof(double), 1, opf);


			} ///save Chebyshev-space data

			fprintf(logfile, "%s\n", "Chebyshev Imaginary Space Saved");

			fclose(opf);
		} // save data

		/// calculate magnitude of the Poles
		for (int s = 1; s <= n_poles; s++)
		{

			// magnitude of Pole
			Chebyshev_pointer->spm[s] = (double)sqrt(((double)Chebyshev_pointer->spmR[s] *
				Chebyshev_pointer->spmR[s]) +
				((double)Chebyshev_pointer->spmI[s] *
					Chebyshev_pointer->spmI[s]));
		} /// calculate magnitude of the Poles

		//// initialize pointer
		for (i = 0; i < NofYpixels; i++)
		{
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				*(tf + index) = (double)0.0;

			}

		}


		double complex;
		double tf_denominator;

		int t;
		int di, dj;
		double dx, dy;
		double dtx, dty;
		double sincx, sincy;
		double toll = 0.000001;


		for (i = 0; i < NofYpixels; i++)
		{ ///calculate TF-Space data

			for (j = 0; j < NofXpixels; j++)
			{

				di = ((int)i - NofYpixels / 2);
				dj = ((int)j - NofXpixels / 2);

				dty = ((double)((int)i - NofYpixels / 2) * m_TheSamplingRate); //space (pixel) * frequency (1/sec)
				dtx = ((double)((int)j - NofXpixels / 2) * m_TheSamplingRate); //space (pixel) * frequency (1/sec)

				/// Whittaker-Shannon interpolation formula 2010
				dy = (double)pi * (2.0 * m_Bandwidth * di - dty);
				// Where 2.0 * bandwidth * di is: 
				// frequency (1/sec) * space (pixel)
				dx = (double)pi * (2.0 * m_Bandwidth * dj - dtx);
				//		 2.0 * bandwidth * dj is: 
				// frequency (1/sec) * space (pixel)
				/// Whittaker-Shannon interpolation formula 2010

				t = ((j * NofYpixels) + i);

				double phase = (double)2.0 * pi * atan2((double)m_Imaginary, (double)m_Real);

				double magnitude = (double)sqrt((double)(((double)dx * dx) + ((double)dy * dy)));

				double complexR = (double)magnitude * (double)cos((double)phase);
				double complexI = (double)magnitude * (double)sin((double)phase);

				double samplingPeriod = 2.0 * pi * (double)pow(((double)NofXpixels * NofYpixels), 1.0 / m_cof);

				double T = ((double)m_cof) / ((double)tan((double)m_cof * samplingPeriod / 2.0));

				// bilinear transform to change the variable from 's' to 'z'
				complex = ((double)2.0 / T) * ((double)complexR + complexI - 1.0) / ((double)complexR + complexI + 1.0);


				double product = 1.0;

				for (int u = 1; u <= n_poles; u++)
				{

					if ((double)Chebyshev_pointer->spmR[u] < 0)


						product *= (((double)complex -

							((double)Chebyshev_pointer->spmR[u] +

								(double)Chebyshev_pointer->spmI[u])));


					else product *= (double)1.0;

				}

				///// Wittaker-Shannon sampling 2010//////
				if ((double)fabs((double)dx) > toll)

					sincx = (double)sin((double)dx) / ((double)dx);

				else sincx = (double)1.0;

				if ((double)fabs((double)dy) > toll)

					sincy = (double)sin((double)dy) / ((double)dy);

				else sincy = (double)1.0;
				///// Wittaker-Shannon sampling 2010//////


				if ((double)((pow(2.0, (n_poles - 1))) * m_eps) != 0.0 && product != (double)0.0) {


					tf_denominator = (((((double)pow(2.0, (n_poles - 1))) * m_eps)) *

						((double)(product)));

					if ((_isnan((double)tf_denominator)) == 0) {

						*(tf + t) = (double)1.0 / (((((double)pow(2.0, (n_poles - 1))) * m_eps)) *

							((double)(product)));

						*(tf + t) *= ((double)sincx * sincy);

					}
					else { *(tf + t) = (double)0.0; }

				}
				else { *(tf + t) = (double)0.0; }


			}

		}///calculate TF-Space data 
		///// TF //////

		 //// Procedure to scale the filter (begin)
		 // scale tf (begin) 
		double max = *(tf);
		double min = *(tf);

		/// compute max and min of data (begin)
		for (i = 0; i < rcyres; i++)
		{
			for (j = 0; j < rcxres; j++)
			{

				index = ((j * rcyres) + i);

				if (*(tf + index) > (double)max)

					max = (double)*(tf + index);

				if (*(tf + index) < (double)min)

					min = (double)*(tf + index);

			} // y dim

		}  // x dim
		/// compute max and min of data (end)

		if (tolower(filterType) == 'l') {

			// scale (begin)
			for (i = 0; i < rcyres; i++)
			{
				for (j = 0; j < rcxres; j++)
				{

					index = ((j * rcyres) + i);

					if (max == min) *(tf + index) = (double)0.0;

					else {

						*(tf + index) = (double)fabs((min - (double)*(tf + index)) / (min - max));

						*(tf + index) *= ft_SCALE;

					}

				} // y dim

			}  // x dim

		}
		else if (tolower(filterType) == 'h') {

			// scale (begin)
			for (i = 0; i < rcyres; i++)
			{
				for (j = 0; j < rcxres; j++)
				{

					index = ((j * rcyres) + i);

					if (max == min) *(tf + index) = (double)0.0;

					else {

						// HP = 1.0 - LP
						*(tf + index) = (double)1.0 - (double)fabs((min - (double)*(tf + index)) / (min - max));

						*(tf + index) *= ft_SCALE;

					}

				} // y dim

			}  // x dim
			// scale (end)
		} //// Procedure to scale the filter (end)


		char filename[128];
		FILE* pf;

		sprintf(filename, "%s", "TF.img");

		fprintf(logfile, "%s\t%s\n", "Now Saving TF in File: ", filename);

		if ((pf = fopen(filename, "wb+")) == NULL)
		{

			fprintf(logfile, "%s\n", "Cannot open file to save TF");

			// FIFO memory deallocation method
			free(tf);
			delete Chebyshev_pointer;

			exit(0);

		}
		else { // save data


			for (i = 0; i < NofYpixels; i++)
			{ ///save TF data
				for (j = 0; j < NofXpixels; j++)
				{

					index = ((j * NofYpixels) + i);

					savedata = (double)*(tf + index);

					fwrite(&savedata, sizeof(double), 1, pf);

				}
			} ///save Z-Space data

			fprintf(logfile, "%s\n", "TF Saved");

			fclose(pf);
		} // save data


		printf("%s\n", "TF Processing Completed");
		fprintf_s(logfile, "%s\n", "TF Processing Completed");

		fclose(logfile);


		// FIFO memory deallocation method
		free(tf);
		delete Chebyshev_pointer;

	} // run the Filtering Process (ends) 

}

void OnInverseFourierTransformTransferFunctionKspace(int rcxres, int rcyres)
{

	int NofXpixels = rcxres;
	int NofYpixels = rcyres;

	int i, j, index;
	int dx, dy;
	int ds, dp;
	int k2, k3, w, t;

	double pi = 3.141592;

	double phase;

	FILE* logfile;
	char logfilename[128] = "INV-FT.log";

	FILE* image;
	char imageFilename[256];

	double* kSpaceR = 0;
	double* kSpaceI = 0;
	double* reconSignal = 0;
	double* transferFR = 0;
	double* transferFI = 0;
	double* Signal = 0;
	double* sincFilterR = 0;
	double* sincFilterI = 0;


	if ((logfile = fopen(logfilename, "w+")) == NULL)
	{

		exit(0);

	}
	else { // allocate memory


		printf("%s\n", "Now INV Fourier Processing...");
		fprintf(logfile, "%s\n", "Now INV Fourier Processing...");

		if ((kSpaceR = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			exit(0);

		}

		if ((kSpaceI = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			free(kSpaceR);
			exit(0);

		}


		if ((reconSignal = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Imaginary Image data: Exit");

			// FIFO memory deallocation method
			free(kSpaceR);
			free(kSpaceI);

			exit(0);

		}

		if ((transferFR = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			free(kSpaceR);
			free(kSpaceI);
			free(reconSignal);

			exit(0);

		}

		if ((transferFI = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			free(kSpaceR);
			free(kSpaceI);
			free(reconSignal);
			free(transferFR);

			exit(0);

		}

		if ((Signal = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			free(kSpaceR);
			free(kSpaceI);
			free(reconSignal);
			free(transferFR);
			free(transferFI);

			exit(0);

		}

		if ((sincFilterR = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			free(kSpaceR);
			free(kSpaceI);
			free(reconSignal);
			free(transferFR);
			free(transferFI);
			free(Signal);

			exit(0);

		}

		if ((sincFilterI = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			free(kSpaceR);
			free(kSpaceI);
			free(reconSignal);
			free(transferFR);
			free(transferFI);
			free(Signal);
			free(sincFilterR);

			exit(0);

		}


	} // allocate memory


	//// read image data and initialize pointers
	sprintf(imageFilename, "%s", "SpaceR-Image.img");

	if ((image = fopen(imageFilename, "rb+")) == NULL)
	{

		fprintf(logfile, "%s%s\n", "Cannot open Image File: ", imageFilename);

		// FIFO memory deallocation method
		free(kSpaceR);
		free(kSpaceI);
		free(reconSignal);
		free(transferFR);
		free(transferFI);
		free(Signal);
		free(sincFilterR);
		free(sincFilterI);

		exit(0);

	}
	else { // read data and initialize pointers

		double number = 0.0;

		for (i = 0; i < NofYpixels; i++)
		{
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				fread(&number, sizeof(double), 1, image);

				*(kSpaceR + index) = (double)number;


			}

		}

		fclose(image);

	}// read data and initialize pointers


	char imageFilename2[128];

	sprintf(imageFilename2, "%s", "SpaceI-Image.img");


	if ((image = fopen(imageFilename2, "rb+")) == NULL)
	{

		fprintf(logfile, "%s%s\n", "Cannot open Image File: ", imageFilename2);

		// FIFO memory deallocation method
		free(kSpaceR);
		free(kSpaceI);
		free(reconSignal);
		free(transferFR);
		free(transferFI);
		free(Signal);
		free(sincFilterR);
		free(sincFilterI);

		exit(0);

	}
	else { // read data and initialize pointers

		double number = 0.0;

		for (i = 0; i < NofYpixels; i++)
		{
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				fread(&number, sizeof(double), 1, image);

				*(kSpaceI + index) = (double)number;

			}

		}

		fclose(image);


		for (i = 0; i < NofYpixels; i++)
		{
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				*(reconSignal + index) = (double)0.0;

				*(Signal + index) = (double)0.0;

			}

		}


	}// read data and initialize pointers


	//// read image data and initialize pointers
	sprintf(imageFilename, "%s", "K-SpaceR-TF.img");

	if ((image = fopen(imageFilename, "rb+")) == NULL)
	{

		fprintf(logfile, "%s%s\n", "Cannot open Image File: ", imageFilename);

		// FIFO memory deallocation method
		free(kSpaceR);
		free(kSpaceI);
		free(reconSignal);
		free(transferFR);
		free(transferFI);
		free(Signal);
		free(sincFilterR);
		free(sincFilterI);

		exit(0);

	}
	else { // read data and initialize pointers

		double number = 0.0;

		for (i = 0; i < NofYpixels; i++)
		{
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				fread(&number, sizeof(double), 1, image);

				*(transferFR + index) = (double)number;


			}

		}

		fclose(image);

	}// read data and initialize pointers

	//// read image data and initialize pointers
	sprintf(imageFilename, "%s", "K-SpaceI-TF.img");

	if ((image = fopen(imageFilename, "rb+")) == NULL)
	{

		fprintf(logfile, "%s%s\n", "Cannot open Image File: ", imageFilename);

		// FIFO memory deallocation method
		free(kSpaceR);
		free(kSpaceI);
		free(reconSignal);
		free(transferFR);
		free(transferFI);
		free(Signal);
		free(sincFilterR);
		free(sincFilterI);

		exit(0);

	}
	else { // read data and initialize pointers

		double number = 0.0;

		for (i = 0; i < NofYpixels; i++)
		{
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				fread(&number, sizeof(double), 1, image);

				*(transferFI + index) = (double)number;


			}

		}

		fclose(image);

	}// read data and initialize pointers


	//// read image data and initialize pointers
	sprintf(imageFilename, "%s", "SpaceR-Filter.img");

	if ((image = fopen(imageFilename, "rb+")) == NULL)
	{

		fprintf(logfile, "%s%s\n", "Cannot open Image File: ", imageFilename);

		// FIFO memory deallocation method
		free(kSpaceR);
		free(kSpaceI);
		free(reconSignal);
		free(transferFR);
		free(transferFI);
		free(Signal);
		free(sincFilterR);
		free(sincFilterI);


		exit(0);

	}
	else { // read data and initialize pointers

		double number = 0.0;

		for (i = 0; i < NofYpixels; i++)
		{
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				fread(&number, sizeof(double), 1, image);

				*(sincFilterR + index) = (double)number;


			}

		}

		fclose(image);

	}// read data and initialize pointers



	sprintf(imageFilename2, "%s", "SpaceI-Filter.img");

	if ((image = fopen(imageFilename2, "rb+")) == NULL)
	{

		fprintf(logfile, "%s%s\n", "Cannot open Image File: ", imageFilename2);

		// FIFO memory deallocation method
		free(kSpaceR);
		free(kSpaceI);
		free(reconSignal);
		free(transferFR);
		free(transferFI);
		free(Signal);
		free(sincFilterR);
		free(sincFilterI);

		exit(0);

	}
	else { // read data and initialize pointers

		double number = 0.0;

		for (i = 0; i < NofYpixels; i++)
		{
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				fread(&number, sizeof(double), 1, image);

				*(sincFilterI + index) = (double)number;

			}

		}

		fclose(image);
	}// read data and initialize pointers

	double real = 0.0, imaginary = 0.0;
	double realComponent = 0.0, imaginaryComponent = 0.0;

	///// INV F Transform //////
	for (i = 0; i < NofYpixels; i++)
	{ ///process k-Space data

		for (j = 0; j < NofXpixels; j++)
		{

			dx = ((int)i - NofYpixels / 2);
			dy = ((int)j - NofXpixels / 2);

			k2 = ((int)(dy * NofYpixels) + dx);

			w = ((j * NofYpixels) + i);

			real = (double)0.0;
			imaginary = (double)0.0;

			double r = sqrt(((double)(double)pow((double)dx, 2.0) + (double)pow((double)dy, 2.0)));

			for (int s = 0; s < NofYpixels; s++)
			{ ///process k-Space data

				for (int p = 0; p < NofXpixels; p++)
				{

					ds = ((int)s - NofYpixels / 2);
					dp = ((int)p - NofXpixels / 2);

					k3 = ((int)(ds * NofXpixels) + dp);

					t = ((p * NofYpixels) + s);

					// polar coordinates clear 
					phase = ((double)(2.0 * pi * k2 * k3) / ((double)NofXpixels * (double)NofYpixels));
					// polar coordinates clear 

					// STEP 11: Calculate ' real ' as a running sum of the pointwise multiplication
					// between: 1. The k-space of the departing image: ' *(kspaceR+t) & *(kspaceI+t) '
					// and 2. The k-space of the Transfer Function: ' *(transferFR + t) & *(transferFI + t) '.
					// Hint: Multiply the real components of the two k-spaces times  ' (double) cos((double)phase) '
					// Hint: ' ((double)*(kSpaceR + t) * (double)*(transferFR + t) * (double)cos((double)phase)) '
					// Then subtract ' - ' the multiplication between the imaginary components of the two k-spaces and
					// (double) sin((double)phase)
					real += ((double)*(kSpaceR + t) * (double)*(sincFilterR + t) * (double)*(transferFR + t) * (double)cos((double)phase)) -
						((double)*(kSpaceI + t) * (double)*(sincFilterI + t) * (double)*(transferFI + t) * (double)sin((double)phase));


					// STEP 12: Calculate ' imaginary ' as a running sum of the pointwise multiplication
					// between: 1. The k-space of the departing image: ' *(kspaceR+t) & *(kspaceI+t) '
					// and 2. The k-space of the Transfer Function: ' *(transferFR + t) & *(transferFI + t) '.
					// Hint: Multiply the real components of the two k-spaces times  ' (double) sin((double)phase) '
					// Hint: ' ((double)*(kSpaceR + t) * (double)*(transferFR + t) * (double)sin((double)phase))) '
					// Then add ' + ' the multiplication between the imaginary components of the two k-spaces and
					// (double) cos((double)phase)	
					imaginary += ((double)*(kSpaceR + t) * (double)*(sincFilterR + t) * (double)*(transferFR + t) * (double)sin((double)phase)) +
						((double)*(kSpaceI + t) * (double)*(sincFilterI + t) * (double)*(transferFI + t) * (double)cos((double)phase));


				}

			}///process k-Space data 


			// STEP 13: Calculate ' realComponent ' as the second power of ' real '
			// Check if ' realComponent ' is a number and in such case do nothing. 
			// If ' realComponent ' is NOT a number then assign to it the value ' 0.0 '
			// Hint: the library function is: ' _isnan() '
			realComponent = (double)real * real;

			if ((_isnan((double)realComponent)) == 0) {}

			else { realComponent = (double)0.0; }


			// STEP 14: Calculate ' imaginaryComponent ' as the second power of ' imaginary '
			// Check if ' imaginaryComponent ' is a number and in such case do nothing. 
			// If ' imaginaryComponent ' is NOT a number then assign to it the value ' 0.0 '
			// Hint: the library function is: ' _isnan() '
			imaginaryComponent = (double)imaginary * imaginary;

			if ((_isnan((double)imaginaryComponent)) == 0) {}

			else { imaginaryComponent = (double)0.0; }


			// STEP 15: Calculate the magnitude of the reconstructed (filtered) image
			// Hint: the magnitude of the image is defined in image space and can be
			// calculated remembering the formula of the magnitude of a complex number:
			// square root of the sum of: 1. the second power of ' real ', and 
			// 2. the second power of ' imaginary '.
			*(reconSignal + w) = (double)sqrt(((double)realComponent) + ((double)imaginaryComponent));

		}
	} ///process k-Space data

	// Flip the image Up Down (begins)
	for (j = 0; j < rcyres; j++) {

		for (i = 0; i < rcxres; i++) {

			int k3 = (j * rcxres + i);

			*(Signal + k3) = (double)*(reconSignal + k3);

		}
	}


	double scalePixelBrightness = 24425077.0; // 2024 calibration with xxns003i00001-1-stack09-1

	for (j = 0; j < rcyres; j++) {

		for (i = 0; i < rcxres; i++) {

			int k3 = (j * rcxres + i);

			int k4 = ((int)((rcyres - j - 1) * rcxres) + (rcxres - i - 1));

			*(reconSignal + k3) = (double)*(Signal + k4) * scalePixelBrightness;

		}
	}
	// Flip the image Up Down (ends)

	double savedata = 0.0;
	FILE* pf;
	char reconFilename[128];

	sprintf(reconFilename, "%s", "ReconstructedSignal.img");


	fprintf(logfile, "%s\t%s\n", "Now Saving Filtered Signal in File: ", reconFilename);

	if ((pf = fopen(reconFilename, "wb+")) == NULL)
	{

		fprintf(logfile, "%s\n", "Cannot open file to save Filtered Signal");

		// FIFO memory deallocation method
		free(kSpaceR);
		free(kSpaceI);
		free(reconSignal);
		free(transferFR);
		free(transferFI);
		free(Signal);
		free(sincFilterR);
		free(sincFilterI);

		exit(0);

	}
	else { // save data


		for (i = 0; i < NofYpixels; i++)
		{ ///save Z-Space data
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				savedata = (double)*(reconSignal + index);

				fwrite(&savedata, sizeof(double), 1, pf);

			}
		} ///save Z-Space data

		fprintf(logfile, "%s\n", "Filtered Signal Saved");

		fclose(pf);
	} // save data

	printf("%s\n", "Inverse Fourier Processing Completed");
	fprintf(logfile, "%s\n", "Inverse Fourier Processing Completed");

	// FIFO memory deallocation method
	free(kSpaceR);
	free(kSpaceI);
	free(reconSignal);
	free(transferFR);
	free(transferFI);
	free(Signal);
	free(sincFilterR);
	free(sincFilterI);

}

void OnInverseZTransformTransferFunction(int rcxres, int rcyres, double m_Real, double m_Imaginary)
{

	int NofXpixels = rcxres;
	int NofYpixels = rcyres;

	int i, j, index;
	int dx, dy;
	int ds, dp;
	int k2, k3, w, t;

	double pi = 3.141592;

	double phase;

	FILE* logfile;
	char logfilename[128] = "INV-ZT.log";

	FILE* image;
	char imageFilename[256];

	double* ZSpaceR = 0;
	double* ZSpaceI = 0;
	double* reconSignal = 0;
	double* transferFR = 0;
	double* transferFI = 0;
	double* Signal = 0;
	double* sincFilterR = 0;
	double* sincFilterI = 0;

	if ((logfile = fopen(logfilename, "w+")) == NULL)
	{

		exit(0);

	}
	else { // allocate memory


		printf("%s\n", "Now INV Z Processing...");
		fprintf(logfile, "%s\n", "Now INV Z Processing...");

		if ((ZSpaceR = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			exit(0);

		}

		if ((ZSpaceI = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			free(ZSpaceR);
			exit(0);

		}


		if ((reconSignal = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Imaginary Image data: Exit");

			// FIFO memory deallocation method
			free(ZSpaceR);
			free(ZSpaceI);

			exit(0);

		}

		if ((transferFR = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			free(ZSpaceR);
			free(ZSpaceI);
			free(reconSignal);

			exit(0);

		}

		if ((transferFI = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			free(ZSpaceR);
			free(ZSpaceI);
			free(reconSignal);
			free(transferFR);

			exit(0);

		}

		if ((Signal = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			free(ZSpaceR);
			free(ZSpaceI);
			free(reconSignal);
			free(transferFR);
			free(transferFI);

			exit(0);

		}

		if ((sincFilterR = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			free(ZSpaceR);
			free(ZSpaceI);
			free(reconSignal);
			free(transferFR);
			free(transferFI);
			free(Signal);

			exit(0);

		}

		if ((sincFilterI = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			free(ZSpaceR);
			free(ZSpaceI);
			free(reconSignal);
			free(transferFR);
			free(transferFI);
			free(Signal);
			free(sincFilterR);

			exit(0);

		}

	} // allocate memory


	//// read image data and initialize pointers
	sprintf(imageFilename, "%s", "SpaceR-Image.img");

	if ((image = fopen(imageFilename, "rb+")) == NULL)
	{

		fprintf(logfile, "%s%s\n", "Cannot open Image File: ", imageFilename);

		// FIFO memory deallocation method
		free(ZSpaceR);
		free(ZSpaceI);
		free(reconSignal);
		free(transferFR);
		free(transferFI);
		free(Signal);
		free(sincFilterR);
		free(sincFilterI);

		exit(0);

	}
	else { // read data and initialize pointers

		double number = 0.0;

		for (i = 0; i < NofYpixels; i++)
		{
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				fread(&number, sizeof(double), 1, image);

				*(ZSpaceR + index) = (double)number;


			}

		}

		fclose(image);

	}// read data and initialize pointers


	char imageFilename2[128];

	sprintf(imageFilename2, "%s", "SpaceI-Image.img");


	if ((image = fopen(imageFilename2, "rb+")) == NULL)
	{

		fprintf(logfile, "%s%s\n", "Cannot open Image File: ", imageFilename2);

		// FIFO memory deallocation method
		free(ZSpaceR);
		free(ZSpaceI);
		free(reconSignal);
		free(transferFR);
		free(transferFI);
		free(Signal);
		free(sincFilterR);
		free(sincFilterI);

		exit(0);

	}
	else { // read data and initialize pointers

		double number = 0.0;

		for (i = 0; i < NofYpixels; i++)
		{
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				fread(&number, sizeof(double), 1, image);

				*(ZSpaceI + index) = (double)number;

			}

		}

		fclose(image);


		for (i = 0; i < NofYpixels; i++)
		{
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				*(reconSignal + index) = (double)0.0;

			}

		}


	}// read data and initialize pointers


	//// read image data and initialize pointers
	sprintf(imageFilename, "%s", "Z-SpaceR-TF.img");

	if ((image = fopen(imageFilename, "rb+")) == NULL)
	{

		fprintf(logfile, "%s%s\n", "Cannot open Image File: ", imageFilename);

		// FIFO memory deallocation method
		free(ZSpaceR);
		free(ZSpaceI);
		free(reconSignal);
		free(transferFR);
		free(transferFI);
		free(Signal);
		free(sincFilterR);
		free(sincFilterI);

		exit(0);

	}
	else { // read data and initialize pointers

		double number = 0.0;

		for (i = 0; i < NofYpixels; i++)
		{
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				fread(&number, sizeof(double), 1, image);

				*(transferFR + index) = (double)number;


			}

		}

		fclose(image);

	}// read data and initialize pointers

	//// read image data and initialize pointers
	sprintf(imageFilename, "%s", "Z-SpaceI-TF.img");

	if ((image = fopen(imageFilename, "rb+")) == NULL)
	{

		fprintf(logfile, "%s%s\n", "Cannot open Image File: ", imageFilename);

		// FIFO memory deallocation method
		free(ZSpaceR);
		free(ZSpaceI);
		free(reconSignal);
		free(transferFR);
		free(transferFI);
		free(Signal);
		free(sincFilterR);
		free(sincFilterI);

		exit(0);

	}
	else { // read data and initialize pointers

		double number = 0.0;

		for (i = 0; i < NofYpixels; i++)
		{
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				fread(&number, sizeof(double), 1, image);

				*(transferFI + index) = (double)number;


			}

		}

		fclose(image);

	}// read data and initialize pointers


	//// read image data and initialize pointers
	sprintf(imageFilename, "%s", "SpaceR-Filter.img");

	if ((image = fopen(imageFilename, "rb+")) == NULL)
	{

		fprintf(logfile, "%s%s\n", "Cannot open Image File: ", imageFilename);

		// FIFO memory deallocation method
		free(ZSpaceR);
		free(ZSpaceI);
		free(reconSignal);
		free(transferFR);
		free(transferFI);
		free(Signal);
		free(sincFilterR);
		free(sincFilterI);


		exit(0);

	}
	else { // read data and initialize pointers

		double number = 0.0;

		for (i = 0; i < NofYpixels; i++)
		{
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				fread(&number, sizeof(double), 1, image);

				*(sincFilterR + index) = (double)number;


			}

		}

		fclose(image);

	}// read data and initialize pointers



	sprintf(imageFilename2, "%s", "SpaceI-Filter.img");

	if ((image = fopen(imageFilename2, "rb+")) == NULL)
	{

		fprintf(logfile, "%s%s\n", "Cannot open Image File: ", imageFilename2);

		// FIFO memory deallocation method
		free(ZSpaceR);
		free(ZSpaceI);
		free(reconSignal);
		free(transferFR);
		free(transferFI);
		free(Signal);
		free(sincFilterR);
		free(sincFilterI);

		exit(0);

	}
	else { // read data and initialize pointers

		double number = 0.0;

		for (i = 0; i < NofYpixels; i++)
		{
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				fread(&number, sizeof(double), 1, image);

				*(sincFilterI + index) = (double)number;

			}

		}

		fclose(image);
	}

	/// reconstruct the image (begin)
	double real = 0.0, imaginary = 0.0;
	double realComponent = 0.0, imaginaryComponent = 0.0;
	double exponent, r;

	double magnitude = (double)sqrt((double)m_Real * m_Real + (double)m_Imaginary * m_Imaginary);
	double Zphase = (double)2.0 * pi * atan2((double)m_Imaginary, (double)m_Real) / ((double)NofXpixels * NofYpixels);

	///// INV Z Transform //////
	for (i = 0; i < NofYpixels; i++)
	{ ///process Z-Space data

		for (j = 0; j < NofXpixels; j++)
		{

			dx = ((int)i - NofYpixels / 2);
			dy = ((int)j - NofXpixels / 2);

			k2 = ((int)(dy * NofYpixels) + dx);

			w = ((j * NofYpixels) + i);


			real = (double)0.0;
			imaginary = (double)0.0;

			for (int s = 0; s < NofYpixels; s++)
			{ ///process Z-Space data

				for (int p = 0; p < NofXpixels; p++)
				{

					ds = ((int)s - NofYpixels / 2);
					dp = ((int)p - NofXpixels / 2);

					k3 = ((int)(ds * NofXpixels) + dp);

					t = ((p * NofYpixels) + s);


					phase = ((double)(2.0 * pi * k2 * k3) / (NofXpixels * NofYpixels));

					exponent = (double)2.0 * pi * t * (double)Zphase / ((double)pow((double)NofXpixels * NofYpixels, 2.0));

					exponent = (double)fabs((double)exponent);

					r = (double)pow((double)magnitude, (double)exponent);

					real += ((double)*(ZSpaceR + t) * (double)*(sincFilterR + t) * (double)*(transferFR + t) * (double)cos((double)phase)) -
						((double)*(ZSpaceI + t) * (double)*(sincFilterI + t) * (double)*(transferFI + t) * (double)sin((double)phase));

					real *= (double)r;

					imaginary += ((double)*(ZSpaceR + t) * (double)*(sincFilterR + t) * (double)*(transferFR + t) * (double)sin((double)phase)) +
						((double)*(ZSpaceI + t) * (double)*(sincFilterI + t) * (double)*(transferFI + t) * (double)cos((double)phase));

					imaginary *= (double)r;

				}

			}///process Z-Space data 

			realComponent = (double)real * real;

			if ((_isnan((double)realComponent)) == 0) {}

			else { realComponent = (double)0.0; }


			imaginaryComponent = (double)imaginary * imaginary;

			if ((_isnan((double)imaginaryComponent)) == 0) {}

			else { imaginaryComponent = (double)0.0; }



			*(reconSignal + w) = (double)sqrt(((double)realComponent) + ((double)imaginaryComponent));

			double scalePixelBrightness = 24425077.0; // 2024 calibration with xxns003i00001-1-stack09-1

			*(reconSignal + w) *= (double)scalePixelBrightness;
		}
	} ///process Z-Space data
	/// reconstruct the image (end)

	double savedata = 0.0;
	FILE* pf;
	char reconFilename[128];

	sprintf(reconFilename, "%s", "ReconstructedSignal.img");


	fprintf(logfile, "%s\t%s\n", "Now Saving Filtered Signal in File: ", reconFilename);

	if ((pf = fopen(reconFilename, "wb+")) == NULL)
	{

		fprintf(logfile, "%s\n", "Cannot open file to save Filtered Signal");

		// FIFO memory deallocation method
		free(ZSpaceR);
		free(ZSpaceI);
		free(reconSignal);
		free(transferFR);
		free(transferFI);
		free(Signal);

		exit(0);

	}
	else { // save data


		for (i = 0; i < NofYpixels; i++)
		{ ///save Z-Space data
			for (j = 0; j < NofXpixels; j++)
			{

				index = ((j * NofYpixels) + i);

				savedata = (double)*(reconSignal + index);

				fwrite(&savedata, sizeof(double), 1, pf);

			}
		} ///save Z-Space data

		fprintf(logfile, "%s\n", "Filtered Signal Saved");

		fclose(pf);
	} // save data
	printf("%s\n", "Inverse Z Processing Completed");
	fprintf(logfile, "%s\n", "Inverse Z Processing Completed");

	fclose(logfile);


	// FIFO memory deallocation method
	free(ZSpaceR);
	free(ZSpaceI);
	free(reconSignal);
	free(transferFR);
	free(transferFI);
	free(Signal);
	free(sincFilterR);
	free(sincFilterI);
}
