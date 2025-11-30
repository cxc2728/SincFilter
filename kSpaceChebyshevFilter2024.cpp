#define _CRT_SECURE_NO_WARNINGS

#include < iostream >
#include < fstream >
#include < string >
#include < io.h >
#include < dos.h >
#include < conio.h >
#include < stdlib.h >
#include < sstream >
#include < stdio.h >
#include < iomanip >
#include < istream >
#include < math.h >

using namespace std;

#define ft_SCALE 1;
const double ft_SCALE_z = 1.0;

void OnFourierTransform_scale(char imageFilename[], int rcxres, int rcyres);
void OnTransferFunction(unsigned char filterType, int rcxres, int rcyres, int n_poles, double m_cof, double m_eps, double m_Bandwidth, double m_TheSamplingRate);
void OnInverseFourierTransformTransferFunction(char filename[], int rcyres, int rcxres);
void OnFourierTransform(char imageFilename[], int rcxres, int rcyres);

// declare a class by the name 'kSpaceChebyshevFilter2021'
class kSpaceChebyshevFilter2021 {

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

	// constructor of the class 'kSpaceChebyshevFilter2021'
	kSpaceChebyshevFilter2021(int x, int y) : n1(x), n2(y) { pointer = 0;  };

	// declare the prototype of the 
	// function 'allocateData'
	// the function belongs to the 
	// sets of methods of the class 'kSpaceChebyshevFilter2021'
	void allocateData();

	// declare the prototype of the 
	// function 'save'
	// the function belongs to the 
	// sets of methods of the class 'kSpaceChebyshevFilter2021'
	void save();

	// destructor of the class 'kSpaceChebyshevFilter2021'
	~kSpaceChebyshevFilter2021() { }

};

void kSpaceChebyshevFilter2021::allocateData() { // allocate data


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


void kSpaceChebyshevFilter2021::save() { // saveImages

	// declare a pointer to file
	// to be used to read the image
	FILE* savedata;
	// declare a string which contains
	// the file name of the image
	char outputFile[128];

	// assign the name string "Signal.img"
	// to the string 'outputFile'
	sprintf(outputFile, "%s", "Signal.img");

	// open the image file in write-binary mode
	if ((savedata = fopen(outputFile, "wb")) == NULL)
	{
		// alert the user of possible failure in opening the file
		std::cout << "Cannot open output file, Now Exit..." << endl;
		exit(0);

	}
	else { // (save)


		for (int v = 0; v < this->n2; v++) { // (a)

			for (int f = 0; f < this->n1; f++)

				// at each iteration of the for loop saves the pixel
				// value contained at the memory address: ' &pointer->Signal[v][f] '
				fwrite(&pointer->Signal[v][f], sizeof(double), 1, savedata);

		} // (a)

		// close the file afer saving
		fclose(savedata);

	} // (save)

} // saveImages

int main(int argc, char* argv[]) {

	char outputFile[128] = "Chebyshev2018.log";

	if (argc < 9) {
		std::cout << endl;
		std::cout << "Please type the image file name" << endl;
		std::cout << "Please make sure that the image format is Analyze 'double': 64 bits real" << endl;
		std::cout << "Please enter the following values: " << endl;
		std::cout << "The number of pixels along the X direction (integer)" << endl;
		std::cout << "The number of pixels along the Y direction (integer)" << endl;
		std::cout << "Enter 'l' to calculate LPF or 'h' to calculate HPF" << endl;
		std::cout << "The cut-off frequency as double precision number in the range: [2.45, 2.55]" << endl;
		std::cout << "The number of poles of the filter, (int): 4, 5, or 6" << endl;
		std::cout << "The Bandwidth (double)" << endl;
		std::cout << "The Sampling Rate (double)" << endl;
		std::cout << "Suggested parameters for LPF: l 2.55 6 0.0017 0.17" << endl;
		std::cout << "Suggested parameters for HPF: h 2.55 6 0.0017 0.17" << endl;
		std::cout << endl;
		exit(0);
	}

	else { // run the program (begin)

		char imageFileName[128];
		sprintf(imageFileName, "%s", argv[1]);

		int n1 = atoi(argv[2]);
		int n2 = atoi(argv[3]);

		int m_rcxres = (int)n1;
		int m_rcyres = (int)n2;

		unsigned char filterType = *argv[4];
		

		if (tolower(filterType) != 'l' && tolower(filterType) != 'h')
		{

			std::cout << "Enter 'l' to calculate LPF or 'h' to calculate HPF" << endl;
			exit(0);
		}

		double m_cof = atof(argv[5]);

		if (m_cof < (double)2.45 || m_cof >(double) 2.55)
		{

			std::cout << "To use the filter please enter" << endl;
			std::cout << "the cut-off frequency as double precision number in the range: [2.45, 2.55]" << endl;
			exit(0);

		}

		int n_poles = atoi(argv[6]);

		if (n_poles != (int)4 && n_poles != (int)5 && n_poles != (int)6 && n_poles)
		{

			std::cout << "To use the filter please enter" << endl;
			std::cout << "the number of poles as an integer: 4, or 6" << endl;
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
			if ( (double)m_cof == (double)2.5   ||
				 (double)m_cof == (double)2.45  ||
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

		double m_eps = (double)0.5; // fixed for better performance

		double m_Bandwidth = atof(argv[7]);
		double m_TheSamplingRate = atof(argv[8]);

		if ((m_TheSamplingRate > 2.0 * m_Bandwidth))
		{ // if m_TheSamplingRate > 2.0*m_Bandwidth

		} // if m_TheSamplingRate > 2.0*m_Bandwidth
		else {

			std::cout << "The Sampling Rate must be >= 2.0 * Bandwidth" << endl;
			exit(0);

		}


		char DataType[128];
		sprintf(DataType, "%s", "DOUBLE");
		FILE* savedata;
		char outputFile[128] = "DATA.log";


		if ((savedata = fopen(outputFile, "w")) == NULL)
		{

			printf("Cannot open output file, Now Exit...");

		}
		else { // processing (begin)

			printf("\n");
			printf("The image file name is: %s \n", imageFileName);
			printf("The DataType is: %s \n", DataType);
			printf("The number of pixels along the X direction is: %d \n", n1);
			printf("The number of pixels along the Y direction is: %d \n", n2);
			std::cout << "The number of poles of the Chebyshev filter is: " << n_poles << endl;
			std::cout << "The value of the ripple factor is: " << m_eps << endl;
			std::cout << "The Bandwidth is: " << m_Bandwidth << endl;
			std::cout << "The Sampling Rate is: " << m_TheSamplingRate << endl;
			std::cout << "The cut-off frequency is: " << m_cof << endl;

			fprintf(savedata, "%s%s\n", "The image file name is: \n", imageFileName);
			fprintf(savedata, "%s%s\n", "The DataType is: \n", DataType);
			fprintf(savedata, "%s%d\n", "The number of pixels along the X direction is: \n", n1);
			fprintf(savedata, "%s%d\n", "The number of pixels along the Y direction is: \n", n2);
			fprintf(savedata, "%s%lf\n", "The cut-off frequency is: ", m_cof);
			fprintf(savedata, "%s%d\n", "The number of poles of the Chebyshev filter is: ", n_poles);
			fprintf(savedata, "%s%lf\n", "The value of the ripple factor is: ", m_eps);
			fprintf(savedata, "%s%lf\n", "The Bandwidth is: ", m_Bandwidth);
			fprintf(savedata, "%s%lf\n", "The Sampling Rate is: ", m_TheSamplingRate);
			fprintf(savedata, "%s%c\n", "The filter is: ", filterType);

			fprintf(savedata, "\n");
			fclose(savedata);

			if (tolower(filterType) == 'l')
				std::cout << "The program applies a low pass filter to the Image Data " << filterType << endl;
			else if (tolower(filterType) == 'h')
				std::cout << "The program applies a high pass filter to the Image Data " << filterType << endl;


			// call to the constructor 'Z' so to create
			// an object of type 'Z'. The data type of 'Z'
			// is 'kSpaceChebyshevFilter2021'
			kSpaceChebyshevFilter2021 Z(n1, n2);

			// the object of type 'Z' 
			// sends a message (invokes)
			// to the method 'allocateData()'
			Z.allocateData();

			/// read image file (begin)
			// declare a file pointer
			FILE* pf;

			// open the file containing the image to
			// process. The image to process is by the
			// name of the string contained by 'imageFileName'
			if ((pf = fopen(imageFileName, "rb")) == NULL)
			{
				// alert the user and save into the log file 
				// the event that the program cannot open the 
				// file containing the image to process
				std::cout << "Cannot open file: " << imageFileName << endl;
				exit(0);

			}
			else { // else

				double number;

				for (int i1 = 0; i1 < m_rcyres; i1++) {// x dim

					for (int i2 = 0; i2 < m_rcxres; i2++) { // y dim

					// at each iteration of the two for loops
					// the program reads the pixel value from the
					// file containing the image and 
						fread(&number, sizeof(double), 1, pf);

						// assigns the pixel value 'number' to the
						// pixel value 'Z.pointer->Signal[i1][i2]'
						// where 'Z' invokes the pointer to the 
						// image 'Signal' at the matrix location '[i1][i2]'
						Z.pointer->Signal[i1][i2] = (double)number;

					} // y dim

				}  // x dim 

				// close the file containg the image to process 	
				fclose(pf);


			} // else 
			/// read image file (end)

			std::cout << "Image data loaded" << endl;

			// save all of the images
			// the object 'Z' invokes (sends a message)
			// the method by the name of 'save()'
			Z.save();

			std::cout << "Now calculating the Direct Fourier Transform..." << endl;
			OnFourierTransform(imageFileName, m_rcxres, m_rcyres);

			std::cout << "Now calculating the Transfer Function (TF)..." << endl;
			OnTransferFunction(filterType, m_rcxres, m_rcyres, n_poles, m_cof, m_eps, m_Bandwidth, m_TheSamplingRate);

			char TFfilename[300];
			sprintf(TFfilename, "%s", "TF.img");
			std::cout << "Now calculating the Direct Fourier Transform of TF..." << endl;
			OnFourierTransform_scale(TFfilename, m_rcxres, m_rcyres);

			std::cout << "Now calculating the Inverse Fourier Transform..." << endl;
			OnInverseFourierTransformTransferFunction(imageFileName, m_rcxres, m_rcyres);

			cout << "The Chebyshev filter was calculated" << endl;

			char reconFilename[300];
			sprintf(reconFilename, "%s%s", "FilteredSignal-", imageFileName);
			std::cout << "Now calculating the Direct Fourier Transform of the Filtered Image..." << endl;
			OnFourierTransform(reconFilename, n1, n2);

			// alert the user of the end of the program
			std::cout << "End of Computation..." << endl;
			std::cout << endl;

			// delete the memory address
			// allocated to the images
			// do this by the command 'delete'
			// applied to the object 'Z' which
			// invokes the pointer to the data
			// structure 'data' containing the image
			// 'Signal'
			delete Z.pointer;

			// the object 'Z' invokes the
			// class destructor
			Z.~kSpaceChebyshevFilter2021();

		} // processing (end)

		return 0;
	} // run the program (end)

}// main


void OnTransferFunction(unsigned char filterType, int rcxres, int rcyres, int n_poles, double m_cof, double m_eps, double m_Bandwidth, double m_TheSamplingRate)
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

void OnInverseFourierTransformTransferFunction(char filename[], int rcxres, int rcyres)
{

	int NofXpixels = rcxres;
	int NofYpixels = rcyres;

	int i, j, index;
	int dx, dy;
	int ds, dp;
	int k2, k3, w, t;

	double pi = 3.141592;

	double phase;

	//2010
	double emittingSource = 1.4145; // 2021
	double scale = ((double)rcxres * rcyres * emittingSource);
	//2010

	FILE* logfile;
	char logfilename[128] = "INV-ZT.log";

	FILE* image;
	char imageFilename[256];

	double* kSpaceR = 0;
	double* kSpaceI = 0;
	double* reconSignal = 0;
	double* transferFR = 0;
	double* transferFI = 0;

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

	} // allocate memory


	//// read image data and initialize pointers
	sprintf(imageFilename, "%s%s", "K-SpaceR-", filename);

	if ((image = fopen(imageFilename, "rb+")) == NULL)
	{

		fprintf(logfile, "%s%s\n", "Cannot open Image File: ", imageFilename);

		// FIFO memory deallocation method
		free(kSpaceR);
		free(kSpaceI);
		free(reconSignal);
		free(transferFR);
		free(transferFI);

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

	sprintf(imageFilename2, "%s%s", "K-SpaceI-", filename);


	if ((image = fopen(imageFilename2, "rb+")) == NULL)
	{

		fprintf(logfile, "%s%s\n", "Cannot open Image File: ", imageFilename2);

		// FIFO memory deallocation method
		free(kSpaceR);
		free(kSpaceI);
		free(reconSignal);
		free(transferFR);
		free(transferFI);

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

			for (int s = 0; s < NofYpixels; s++)
			{ ///process k-Space data

				for (int p = 0; p < NofXpixels; p++)
				{

					ds = ((int)s - NofYpixels / 2);
					dp = ((int)p - NofXpixels / 2);

					k3 = ((int)(ds * NofXpixels) + dp);

					t = ((p * NofYpixels) + s);


					phase = ((double)(2.0 * pi * k2 * k3) / ((double)NofXpixels * (double)NofYpixels));

					// STEP 11: Calculate ' real ' as a running sum of the pointwise multiplication
					// between: 1. The k-space of the departing image: ' *(kspaceR+t) & *(kspaceI+t) '
					// and 2. The k-space of the Transfer Function: ' *(transferFR + t) & *(transferFI + t) '.
					// Hint: Multiply the real components of the two k-spaces times  ' (double) cos((double)phase) '
					// Hint: ' ((double)*(kSpaceR + t) * (double)*(transferFR + t) * (double)cos((double)phase)) '
					// Then subtract ' - ' the multiplication between the imaginary components of the two k-spaces and
					// (double) sin((double)phase)
					real += ((double)*(kSpaceR + t) * (double)*(transferFR + t) * (double)cos((double)phase)) -
						    ((double)*(kSpaceI + t) * (double)*(transferFI + t) * (double)sin((double)phase));

					// STEP 12: Calculate ' imaginary ' as a running sum of the pointwise multiplication
					// between: 1. The k-space of the departing image: ' *(kspaceR+t) & *(kspaceI+t) '
					// and 2. The k-space of the Transfer Function: ' *(transferFR + t) & *(transferFI + t) '.
					// Hint: Multiply the real components of the two k-spaces times  ' (double) sin((double)phase) '
					// Hint: ' ((double)*(kSpaceR + t) * (double)*(transferFR + t) * (double)sin((double)phase))) '
					// Then add ' + ' the multiplication between the imaginary components of the two k-spaces and
					// (double) cos((double)phase)	
					imaginary += ((double)*(kSpaceR + t) * (double)*(transferFR + t) * (double)sin((double)phase)) +
					          	 ((double)*(kSpaceI + t) * (double)*(transferFI + t) * (double)cos((double)phase));

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

			*(reconSignal + w) /= (double)scale;

		}
	} ///process k-Space data


	double savedata = 0.0;
	FILE* pf;
	char reconFilename[128];

	sprintf(reconFilename, "%s%s", "FilteredSignal-", filename);


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

	fclose(logfile);


	// FIFO memory deallocation method
	free(kSpaceR);
	free(kSpaceI);
	free(reconSignal);
	free(transferFR);
	free(transferFI);

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

void OnFourierTransform(char imageFilename[], int rcxres, int rcyres)
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