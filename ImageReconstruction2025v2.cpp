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

unsigned char filterType = 'l';
const double z_f = 0.1;
#define ft_SCALE 1;
unsigned char kz = 'k';
const double ft_SCALE_z = 1.0;

void OnInverseFourierTransformTransferFunctionKspace(int rcyres, int rcxres);
void OnInverseZTransformTransferFunction(int rcyres, int rcxres, double m_Real, double m_Imaginary);

void OnFrequencyDomain(char filterType, char kz, char imageFilename[], int rcxres, int rcyres, double m_Real, double m_Imaginary, int m_angles, double Sinc_SamplingRate);

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
	if (argc < 10) {

		std::cout << endl;
		std::cout << "Please type the image file name" << endl;
		std::cout << "Please make sure that the image format is Analyze 'double': 64 bits real" << endl;
		std::cout << "Please enter the number of pixels along the X direction (integer)" << endl;
		std::cout << "Please enter the number of pixels along the Y direction (integer)" << endl;

		std::cout << "Enter 'k' to k-space filter or 'z' to Z-space filter" << endl;
		std::cout << "The value of the real part of the complex number, (double) in: [0.01, 1]" << endl;
		std::cout << "The value of the imaginary part of the complex number, (double) in: [0, 1]" << endl;

		std::cout << "Enter the number of angles used to calculate the SincFilter (int) [1, 10]" << endl;

		std::cout << "The Sampling Rate the Sinc function (double) in: [1.0, 3.0]" << endl;

		std::cout << "Enter 'l' to calculate LPF or 'h' to calculate HPF" << endl;

		std::cout << "Example parameters for LPF: k 0.5 0.5 5 0.8 l" << endl;
		std::cout << "Example parameters for HPF: k 0.5 0.5 5 0.8 h" << endl;
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

			char kz = *argv[4];
			double m_Real = atof(argv[5]);
			double m_Imaginary = atof(argv[6]);

			int m_angles = atoi(argv[7]);
			double Sinc_SamplingRate = atof(argv[8]);

			char filterType = *argv[9];

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

			// inform the user of the image size 
			// (number of rows and number of columns
			// of the matrix containing the image)
			std::cout << endl;
			std::cout << "The number of pixels along the X direction is: " << atoi(argv[2]) << endl;
			std::cout << "The number of pixels along the Y direction is: " << atoi(argv[3]) << endl;
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
			fprintf(savedata, "%s%lf\n", "The value of m_Real is: ", m_Real);
			fprintf(savedata, "%s%lf\n", "The value of m_Imaginary is: ", m_Imaginary);
			fprintf(savedata, "%s%lf\n", "The magnitude of the Complex number is: ", magnitude);
			fprintf(savedata, "%s%lf\n", "The phase of the Complex number is: ", Zphase);
			fprintf(savedata, "%s%d\n", "The value of m_angles is set to: ", m_angles);
			fprintf(savedata, "%s%lf\n", "The Sampling Rate of the Sinc function is: ", Sinc_SamplingRate);
			
			fprintf(savedata, "%s%c\n", "The filter is: ", filterType);

			if (tolower(filterType) == 'l')
				std::cout << "The program applies a low pass filter to the Image Data " << filterType << endl;
			else if (tolower(filterType) == 'h')
				std::cout << "The program applies a high pass filter to the Image Data " << filterType << endl;

			if (tolower(filterType) == 'l')
				fprintf(savedata, "%s\n", "The program applies the low pass Filter to the Image Data");
			else if (tolower(filterType) == 'h')
				fprintf(savedata, "%s\n", "The program applies the high pass Filter to the Image Data");


			// call to the constructor 'ImageReconstruction' so to create
			// an object of type 'ImageReconstruction'. The data type of 'ImageReconstruction'
			// is 'ImageReconstruction2024'
			ImageReconstruction2024 ImageReconstruction(n1, n2);

			// the object of type 'ImageReconstruction' 
			// sends a message (invokes)
			// to the method 'allocateData()'
			ImageReconstruction.allocateData();

			OnFrequencyDomain(filterType, kz, imageFileName, n1, n2, m_Real, m_Imaginary, m_angles, Sinc_SamplingRate);

			// invT (begin)
			if (tolower(kz) == 'k')
			{
				std::cout << "Now Calculating the Inverse Fourier Transform..." << endl;
				OnInverseFourierTransformTransferFunctionKspace(n1, n2);

			}
			else if (tolower(kz) == 'z')
			{
				std::cout << "Now Calculating the Inverse Z Transform..." << endl;
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


void OnFrequencyDomain(char filterType, char kz, char imageFilename[], int rcxres, int rcyres, double m_Real, double m_Imaginary, int m_angles, double Sinc_SamplingRate)
{

	int NofXpixels = rcxres;
	int NofYpixels = rcyres;

	double pi = 3.141592;

	double* SpaceR = 0;
	double* SpaceI = 0;
	double* SpaceM = 0;

	double* Signal = 0;

	double* RadonSincFilter = 0;

	double* BandWidth = 0;

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

	if (kz == 'k')
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
	else if (kz == 'z')
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
	

	if (kz == 'k')
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
	else if (kz == 'z')
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

		if ((Signal = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			free(kSpaceR);
			free(kSpaceI);
			free(reconSignal);

			exit(0);

		}

		if ((sincFilterR = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			free(kSpaceR);
			free(kSpaceI);
			free(reconSignal);
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
	sprintf(imageFilename, "%s", "SpaceR-Filter.img");

	if ((image = fopen(imageFilename, "rb+")) == NULL)
	{

		fprintf(logfile, "%s%s\n", "Cannot open Image File: ", imageFilename);

		// FIFO memory deallocation method
		free(kSpaceR);
		free(kSpaceI);
		free(reconSignal);
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
					real += ((double)*(kSpaceR + t) * (double)*(sincFilterR + t) * (double)cos((double)phase)) -
						    ((double)*(kSpaceI + t) * (double)*(sincFilterI + t) * (double)sin((double)phase));


					// STEP 12: Calculate ' imaginary ' as a running sum of the pointwise multiplication
					// between: 1. The k-space of the departing image: ' *(kspaceR+t) & *(kspaceI+t) '
					// and 2. The k-space of the Transfer Function: ' *(transferFR + t) & *(transferFI + t) '.
					// Hint: Multiply the real components of the two k-spaces times  ' (double) sin((double)phase) '
					// Hint: ' ((double)*(kSpaceR + t) * (double)*(transferFR + t) * (double)sin((double)phase))) '
					// Then add ' + ' the multiplication between the imaginary components of the two k-spaces and
					// (double) cos((double)phase)	
					imaginary += ((double)*(kSpaceR + t) * (double)*(sincFilterR + t) * (double)sin((double)phase)) +
						         ((double)*(kSpaceI + t) * (double)*(sincFilterI + t) * (double)cos((double)phase));


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

		if ((Signal = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			free(ZSpaceR);
			free(ZSpaceI);
			free(reconSignal);

			exit(0);

		}

		if ((sincFilterR = (double*)calloc(NofXpixels * NofYpixels, sizeof(double))) == NULL)
		{

			fprintf(logfile, "%s\n", "Not enough memory to allocate Real Image data: Exit");

			// FIFO memory deallocation method
			free(ZSpaceR);
			free(ZSpaceI);
			free(reconSignal);
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
	sprintf(imageFilename, "%s", "SpaceR-Filter.img");

	if ((image = fopen(imageFilename, "rb+")) == NULL)
	{

		fprintf(logfile, "%s%s\n", "Cannot open Image File: ", imageFilename);

		// FIFO memory deallocation method
		free(ZSpaceR);
		free(ZSpaceI);
		free(reconSignal);
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

					real += ((double)*(ZSpaceR + t) * (double)*(sincFilterR + t) * (double)cos((double)phase)) -
						((double)*(ZSpaceI + t) * (double)*(sincFilterI + t) * (double)sin((double)phase));

					real *= (double)r;

					imaginary += ((double)*(ZSpaceR + t) * (double)*(sincFilterR + t) * (double)sin((double)phase)) +
						((double)*(ZSpaceI + t) * (double)*(sincFilterI + t) * (double)cos((double)phase));

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
	free(Signal);
	free(sincFilterR);
	free(sincFilterI);
}