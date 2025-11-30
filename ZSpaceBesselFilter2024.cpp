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

void OnZTransform_scale(char imageFilename[], int rcxres, int rcyres, double m_Real, double m_Imaginary);
void OnZTransform(char imageFilename[], int rcxres, int rcyres, double m_Real, double m_Imaginary);
void OnTransferFunction(unsigned char filterType, int rcxres, int rcyres, int n_poles, double m_cof, double m_Bandwidth, double m_TheSamplingRate, double m_Real, double m_Imaginary);
void OnInverseZTransformTransferFunction(char filename[], int rcyres, int rcxres, double m_Real, double m_Imaginary);
void OnFourierTransform(char imageFilename[], int rcxres, int rcyres);

// declare a class by the name 'ZSpaceBesselFilter2021'
class ZSpaceBesselFilter2021 {

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

		double **Signal; // declare the pointer to pointer to the matrix (image) 

		}*pointer; // pointer to the element of the structure 'data'
	               // the pointer points to the memory address of the
	               // pointers to pointers 

public:

	// constructor of the class 'ZSpaceBesselFilter2021'
	ZSpaceBesselFilter2021(int x, int y) : n1(x), n2(y) { }; 
		
	// declare the prototype of the 
	// function 'allocateData'
	// the function belongs to the 
	// sets of methods of the class 'ZSpaceBesselFilter2021'
	void allocateData();

	// declare the prototype of the 
	// function 'save'
	// the function belongs to the 
	// sets of methods of the class 'ZSpaceBesselFilter2021'
	void save();

	// destructor of the class 'ZSpaceBesselFilter2021'
	~ZSpaceBesselFilter2021() { } 

};

void ZSpaceBesselFilter2021::allocateData() { // allocate data


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
	 pointer->Signal = new double*[this->n2];

	 
	 for( int v=0; v < this->n2; v++ ) { // (1)
	 // at each iteration of the for loop
	 // assign to the pointer 'Signal[v]' the instance 
	 // of the memory address of the pointer [this-n1].
	 // Practically define the memory address of the 
	 // columns of the matrices containing the image:
	 // 'Signal.

		 pointer->Signal[v] = new double[this->n1];
		

		  } // (1) allocate struct 'data' (end)


		// (2) initialize (begin)
		for(int v=0; v < this->n2; v++ ) { // (a)

			for( int f=0; f < this->n1 ; f++ ) { // (b)
			
			// at each iteration of the two for loops
			// initializes the value of the pixel of
			// the image to zero. This is done for the
			// image 'Signal'.
			pointer->Signal[v][f] = (double)0.0;


			} //(b)

		 } //(a)
		// (2) initialize (end)

} // allocate data


void ZSpaceBesselFilter2021::save() { // saveImages

	// declare a pointer to file
	// to be used to read the image
	FILE * savedata;
	// declare a string which contains
	// the file name of the image
	char outputFile[128];
	
	// assign the name string "Signal.img"
	// to the string 'outputFile'
	sprintf(outputFile, "%s","Signal.img");

	// open the image file in write-binary mode
	if ((savedata = fopen(outputFile,"wb"))==NULL)
	{
		// alert the user of possible failure in opening the file
		std::cout << "Cannot open output file, Now Exit..." << endl;
		exit(0);

	} else  { // (save)


	for( int v=0; v < this->n2; v++ ) { // (a)

		for( int f=0; f < this->n1; f++ ) 
	
		// at each iteration of the for loop saves the pixel
		// value contained at the memory address: ' &pointer->Signal[v][f] '
		fwrite(&pointer->Signal[v][f],sizeof(double),1,savedata);

	} // (a)

	// close the file afer saving
	fclose(savedata);

	} // (save)

	} // saveImages

int main ( int argc, char * argv[] ) {

if (argc < 11) {  std::cout << endl;
				 std::cout << "Please type the image file name" << endl;
				 std::cout << "Please make sure that the image format is Analyze 'double': 64 bits real" << endl;
				 std::cout << "Please enter the following values: " << endl;
				 std::cout << "The number of pixels along the X direction (integer)" << endl;
				 std::cout << "The number of pixels along the Y direction (integer)" << endl;
				 std::cout << "Enter 'l' to calculate LPF or 'h' to calculate HPF" << endl;
				 std::cout << "The cut-off frequency as double precision number in the range: [2.5, 2.8]" << endl;
				 std::cout << "The value of the real part of the complex number, (double) in: [0.01, 1]" << endl;
				 std::cout << "The value of the imaginary part of the complex number, (double) in: [0, 1]" << endl;
				 std::cout << "The number of poles of the filter, (int) in: [2, 5]" << endl;
				 std::cout << "The Bandwidth (double)" << endl;
				 std::cout << "The Sampling Rate (double)" << endl;
				 std::cout << "Suggested parameters for LPF: l 2.5 0.5 0.5 4 0.054 0.154" << endl;
				 std::cout << "Suggested parameters for HPF: h 2.6 0.5 0.5 2 0.054 0.154" << endl;
				 std::cout << endl;
				 exit(0); }

else { // run the program (begin)
	
	char imageFileName[128];
	sprintf(imageFileName, "%s", argv[1]);

	int m_rcxres = atoi(argv[2]);
	int m_rcyres = atoi(argv[3]);

	unsigned char filterType = *argv[4];

	if ( tolower(filterType) != 'l' && tolower(filterType) != 'h' )
	{
	
		std::cout << "Enter 'l' to calculate LPF or 'h' to calculate HPF" << endl;
		exit(0);
	}

	double m_cof = atof(argv[5]);

	if ( m_cof < (double) 2.5 || m_cof > (double) 2.8 )
	{
	
		std::cout << "To use the filter please enter" << endl;
		std::cout << "the cut-off frequency as double precision number in the range: [2.5, 2.8]" << endl;
		exit(0);

	}
	
	if ( tolower(filterType) == 'l' )
	std::cout << "The program applies a low pass filter to the Image Data " << filterType << endl;
	else if ( tolower(filterType) == 'h' ) 
	std::cout << "The program applies a high pass filter to the Image Data " << filterType << endl;


	// reads from the command console the
	// value of the complex number
	double m_Real = atof(argv[6]);
	double m_Imaginary = atof(argv[7]);	

	if ( m_Real < (double) 0.01 || m_Real > (double) 1.0 )
	{
	
		std::cout << "To use the filter please enter" << endl;
		std::cout << "the real part of the complex number, (double) in: [0.01, 1]" << endl;
		std::cout << "The suggested value is 0.5" << endl;
		exit(0);

	}

	if ( m_Imaginary < (double) 0.0 || m_Imaginary > (double) 1.0 )
	{
	
		std::cout << "To use the filter please enter" << endl;
		std::cout << "the imaginary part of the complex number, (double) in: [0, 1]" << endl;
		std::cout << "The suggested value is 0.5" << endl;
		exit(0);

	}


	int n_poles = atoi(argv[8]);

	if ( n_poles < (int) 2 || n_poles > (int) 5 )
	{
	
		std::cout << "To use the filter please enter" << endl;
		std::cout << "the number of poles as an integer in the range: [2, 5]" << endl;
		exit(0);

	}

	if ( n_poles == (int) 2 )
	{
		if ( m_cof == (double) 2.5  ||
			 m_cof == (double) 2.6  ||
			 m_cof == (double) 2.7  ||
			 m_cof == (double) 2.8
			 ){ }

		else {
		
		std::cout << "Please reset the Cut-Off frequency to:" << endl;
		std::cout << "2.5, 2.6, 2.7, or 2.8" << endl;
		exit(0);

		}

	} 
	
	else if ( n_poles == (int) 3 )
	{
	
	    if ( m_cof == (double) 2.5  ||
			 m_cof == (double) 2.6  ||
			 m_cof == (double) 2.7  ||
			 m_cof == (double) 2.8
		   ){ }

		else {
		
		std::cout << "Please reset the Cut-Off frequency to:" << endl;
		std::cout << "2.5, 2.6, 2.7, or 2.8" << endl;
		exit(0);

		}
	
	}

	else if ( n_poles == (int) 4 )
	{
	
	    if ( m_cof == (double) 2.5  ||
			 m_cof == (double) 2.6  ||
			 m_cof == (double) 2.7  ||
			 m_cof == (double) 2.8
		   ){ }

		else {
		
		std::cout << "Please reset the Cut-Off frequency to:" << endl;
		std::cout << "2.5, 2.6, 2.7, or 2.8" << endl;
		exit(0);

		}
	
	}

	else if ( n_poles == (int) 5 )
	{
	
	    if ( m_cof == (double) 2.5  ||
			 m_cof == (double) 2.6  ||
			 m_cof == (double) 2.7  ||
			 m_cof == (double) 2.8
		   ){ }

		else {
		
		std::cout << "Please reset the Cut-Off frequency to:" << endl;
		std::cout << "2.5, 2.6, 2.7, or 2.8" << endl;
		exit(0);

		}
	
	}
	

	double m_Bandwidth = atof(argv[9]);
	double m_TheSamplingRate = atof(argv[10]);

	  if ( (m_TheSamplingRate > 2.0*m_Bandwidth) )
	  { // if m_TheSamplingRate > 2.0*m_Bandwidth

      } // if m_TheSamplingRate > 2.0*m_Bandwidth
	  else {
					
			std::cout << "The Sampling Rate must be >= 2.0 * Bandwidth" << endl;
			exit(0);
			
	  }

	
	// inform the user of the image size 
	// (number of rows and number of columns
	// of the matrix containing the image)
	std::cout << "The number of pixels along the X direction is: " << atoi(argv[2]) << endl;
	std::cout << "The number of pixels along the Y direction is: " << atoi(argv[3]) << endl;
	std::cout << "The value of m_Real is: " << m_Real << endl;
	std::cout << "The value of m_Imaginary is: " << m_Imaginary << endl;
	std::cout << "The number of poles of the Bessel filter is: " << n_poles << endl; 
	std::cout << "The Bandwidth is: " << m_Bandwidth << endl;
	std::cout << "The Sampling Rate is: " << m_TheSamplingRate << endl;

	double pi = 3.141592;
	double Zphase = (double)2.0*pi*atan2( (double)m_Imaginary, (double)m_Real ) / (m_rcxres*m_rcyres);
	double magnitude = (double) sqrt ( (double)m_Real*m_Real + (double)m_Imaginary*m_Imaginary );

	std::cout << "The magnitude of the Complex number is: " << magnitude << endl;
    std::cout << "The phase of the Complex number is: " << Zphase << endl;
	std::cout << "The cut-off frequency is: " << m_cof << endl;

	// assign to the char string 'outputFile'
	// the value "Bessel2018.log"
	char outputFile[128] = "Bessel2018.log";

	// declare a pointer to a file by the
	// name 'savedata'
	FILE * savedata;

	if ((savedata = fopen(outputFile,"w"))==NULL)
	{
		// alert the user of possible failure in opening the file
		std::cout << "Cannot open output file, Now Exit..." << endl;
		exit(0);

	} else { // save to log file

	// save into the log file the image size 
	// (number of rows and number of columns
	// of the matrix containing the image)
	fprintf(savedata,"%s%d\n", "The number of pixels along the X direction is: ", m_rcxres);
	fprintf(savedata,"%s%d\n", "The number of pixels along the Y direction is: ", m_rcyres);
	fprintf(savedata,"%s%lf\n", "The cut-off frequency is: ", m_cof);
	fprintf(savedata,"%s%lf\n", "The value of m_Real is: ", m_Real);
	fprintf(savedata,"%s%lf\n", "The value of m_Imaginary is: ", m_Imaginary);
	fprintf(savedata,"%s%lf\n", "The magnitude of the Complex number is: ", magnitude);
	fprintf(savedata,"%s%lf\n", "The phase of the Complex number is: ", Zphase);
	fprintf(savedata,"%s%d\n", "The number of poles of the Bessel filter is: ", n_poles); 
	fprintf(savedata,"%s%lf\n", "The Bandwidth is: ", m_Bandwidth);
	fprintf(savedata,"%s%lf\n", "The Sampling Rate is: ", m_TheSamplingRate);

	if ( tolower(filterType) == 'l' )
	fprintf(savedata, "%s\n", "The program applies a low pass filter to the Image Data");
	else if ( tolower(filterType) == 'h' ) 
	fprintf(savedata, "%s\n", "The program applies a high pass filter to the Image Data");

	fclose(savedata);

	} // save to log file (end)


	// call to the constructor 'Z' so to create
	// an object of type 'Z'. The data type of 'Z'
	// is 'ZSpaceBesselFilter2021'
	ZSpaceBesselFilter2021 Z(m_rcxres, m_rcyres);

	// the object of type 'Z' 
	// sends a message (invokes)
	// to the method 'allocateData()'
	Z.allocateData();

	/// read image file (begin)
	// declare a file pointer
	FILE * pf;

	// open the file containing the image to
	// process. The image to process is by the
	// name of the string contained by 'imageFileName'
	if ((pf = fopen(imageFileName,"rb"))==NULL)
	{
		// alert the user and save into the log file 
		// the event that the program cannot open the 
		// file containing the image to process
		std::cout << "Cannot open file: " << imageFileName << endl;
		exit(0);

	} else { // else

	double number;

	for (int i1=0; i1 < m_rcyres; i1++) {// x dim
       	
		for (int i2=0; i2 < m_rcxres; i2++) { // y dim
		
		// at each iteration of the two for loops
		// the program reads the pixel value from the
		// file containing the image and 
		fread(&number,sizeof(double),1,pf);
		
		// assigns the pixel value 'number' to the
		// pixel value 'Z.pointer->Signal[i1][i2]'
		// where 'Z' invokes the pointer to the 
		// image 'Signal' at the matrix location '[i1][i2]'
		Z.pointer->Signal[i1][i2] = (double)number;
                          
		} // y dim
        
	}  // x dim 

    // close the file containg the image to process 	
    fclose (pf);


	} // else 
	/// read image file (end)

	std::cout << "Image data loaded" << endl; 

	// save all of the images
	// the object 'Z' invokes (sends a message)
	// the method by the name of 'save()'
	Z.save(); 

	std::cout << "Now calculating the Direct Z Transform..." << endl;
    OnZTransform(imageFileName, m_rcxres, m_rcyres, m_Real, m_Imaginary);

    std::cout << "Now calculating the Transfer Function (TF)..." << endl;
    OnTransferFunction(filterType, m_rcxres, m_rcyres, n_poles, m_cof, m_Bandwidth, m_TheSamplingRate, m_Real, m_Imaginary);

	std::cout << "Now calculating the Direct Z Transform of TF..." << endl;
    OnZTransform_scale("TF.img", m_rcxres, m_rcyres, m_Real, m_Imaginary);

	std::cout << "Now calculating the Inverse Z Transform..." << endl;
	OnInverseZTransformTransferFunction(imageFileName, m_rcxres, m_rcyres, m_Real, m_Imaginary);

	// alert the user of the end of the program
	cout << "The Bessel filter was calculated" << endl;

	std::cout << "Now calculating the Direct Fourier Transform of the Image..." << endl;
	OnFourierTransform(imageFileName, m_rcxres, m_rcyres);

	char reconFilename[300];
	sprintf(reconFilename, "%s%s", "FilteredSignal-", imageFileName);

	std::cout << "Now calculating the Direct Fourier Transform of the Filtered Image..." << endl;
	OnFourierTransform(reconFilename, m_rcxres, m_rcyres);

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
	Z.~ZSpaceBesselFilter2021();

	} // run the program (end)

	return 0;
} // end of main 

void OnZTransform_scale(char imageFilename[], int rcxres, int rcyres, double m_Real, double m_Imaginary)
{
	
	int NofXpixels = rcxres;
	int NofYpixels = rcyres;

	int i, j, index;
	int dx, dy;
	int ds, dp; 
	int k2, k3, w, t;
	
	double pi = 3.141592;

	double * ZSpaceR = 0;
	double * ZSpaceI = 0;
	double * Signal = 0;

	FILE * logfile;
	
	char logfilename[128]="Z-T.log";

  	if ((logfile = fopen(logfilename,"w+"))==NULL)
	{

		printf("%s\n %s\n" , "Unable to open log File", "Now Exit");

		exit(0);
	
	} else { // allocate memory 


	if ((ZSpaceR = (double *) calloc( NofXpixels*NofYpixels, sizeof(double)) ) == NULL)
	{
   
		fprintf(logfile,"%s\n", "Not enough memory to allocate Real Image data: Exit");
   
		exit(0);

	}

	if ((ZSpaceI = (double *) calloc( NofXpixels*NofYpixels, sizeof(double)) ) == NULL)
	{
   
		fprintf(logfile,"%s\n", "Not enough memory to allocate Real Image data: Exit");

		// FIFO memory deallocation method
		free(ZSpaceR);
		exit(0);

	}

	if ((Signal = (double *) calloc( NofXpixels*NofYpixels, sizeof(double)) ) == NULL)
	{
   
		fprintf(logfile,"%s\n", "Not enough memory to allocate Real Image data: Exit");

		// FIFO memory deallocation method
		free(ZSpaceR);
		free(ZSpaceI);
		exit(0);

	}

	} // allocate memory 

	//// read image data and initialize pointers
	double number = 0.0;

		for (i=0; i<NofYpixels; i++)
		{ 
			for (j=0; j<NofXpixels; j++)
			{

				index = ((j*NofYpixels)+i);
				
				*(ZSpaceR+index) = (double) 0.0;

				*(ZSpaceI+index) = (double) 0.0;

			}

		}

	FILE * pf;
	char SignalFilename[128];
	double readData;
	
	sprintf(SignalFilename, "%s", imageFilename);

	if ((pf = fopen(SignalFilename,"rb+"))==NULL)
	{

	 fprintf(logfile, "%s\n", "Cannot open file to read Signal");

	 // FIFO memory deallocation method
	 free(ZSpaceR);
	 free(ZSpaceI);
	 free(Signal);

	 exit(0);
	
	} else { // read data


	for (i=0; i<rcyres; i++)
	{ ///read signal data
		for (j=0; j<rcxres; j++)
		{

			index = ((j*rcyres)+i);
          
            fread(&readData,sizeof(double),1,pf);

			*(Signal+index) = (double)readData;

		}
	} ///read signal data

	fprintf(logfile,"%s\n", "Signal Read in DOUBLE (64bits) format");

	fclose (pf);
	} // save data


	double phase;
	double exponent, r;
	double complexRZ, complexIZ;
		
	double magnitude = (double) sqrt ( (double)m_Real*m_Real + (double)m_Imaginary*m_Imaginary );
	double Zphase = (double)2.0*pi*atan2( (double)m_Imaginary, (double)m_Real ) / ((double)NofXpixels*NofYpixels);

	///// Z Transform //////
	for (i=0; i<NofYpixels; i++)
	{ ///calculate Z-Space data

		for (j=0; j<NofXpixels; j++)
		{

	
			dx = ((int) i - NofYpixels/2);
		    dy = ((int) j - NofXpixels/2);

			k2 = ((int)(dy*NofYpixels)+dx); 

			w = ((j*NofYpixels)+i);

			for (int s=0; s<NofYpixels; s++)
			{ ///calculate Z-Space data 
				for (int p=0; p<NofXpixels; p++)
				{ 
					

		     		ds = ((int) s - NofYpixels/2);
		            dp = ((int) p - NofXpixels/2);
 
				    k3 = ((int)(ds*NofXpixels)+dp); 

					t = ((p*NofYpixels)+s);


		 		    phase = ((double) (2.0 * pi * k2 * k3) / (NofXpixels*NofYpixels));

					exponent = (double) 2.0 * pi * t * (double)Zphase / ((double) pow((double)NofXpixels*NofYpixels, 2.0));

					exponent = (double)fabs((double)exponent);

					r = (double) pow( (double)magnitude, -(double)exponent );


					complexRZ = (double) cos( (double)phase ) + (double) sin( (double)phase );
					
					complexIZ = -(double) sin( (double)phase ) + (double) cos( (double)phase ); 

					
					*(ZSpaceR+w) += (double)*(Signal+t) * (double)complexRZ * (double)r; 

					*(ZSpaceI+w) += (double)*(Signal+t) * (double)complexIZ * (double)r;
			}

		}///calculate Z-Space data 

			    
		}
	} ///calculate Z-Space data
	///// Z Transform //////

	//// Procedure to scale (begin)
	double max = *(ZSpaceR);
	double min= *(ZSpaceR);

	/// compute max and min of data (begin)
	for (i=0; i<rcyres; i++)
	{
		for (j=0; j<rcxres; j++)
		{

				w = ((j*NofYpixels)+i);

				if( *(ZSpaceR+w) > (double)max ) 
			
					max = (double) *(ZSpaceR+w);
              
				if( *(ZSpaceR+w) < (double)min ) 
			
					min = (double) *(ZSpaceR+w);
		
		} // y dim
        
	}  // x dim
	/// compute max and min of data (end)

	for (i=0; i<rcyres; i++)
	{
		for (j=0; j<rcxres; j++)
		{

				w = ((j*NofYpixels)+i);

				if ( max == min ) *(ZSpaceR+w) = (double)0.0;

				else { 
				
						*(ZSpaceR+w) = (double) fabs( (min - (double)*(ZSpaceR+w) ) / (min - max) );

						*(ZSpaceR+w) *= ft_SCALE_z;

				}

		} // y dim
        
	}  // x dim
	//// Procedure to scale (end)

	//// Procedure to scale (begin)
	max = *(ZSpaceI);
	min= *(ZSpaceI);

	/// compute max and min of data (begin)
	for (i=0; i<rcyres; i++)
	{
		for (j=0; j<rcxres; j++)
		{

				w = ((j*NofYpixels)+i);

				if( *(ZSpaceI+w) > (double)max ) 
			
					max = (double) *(ZSpaceI+w);
              
				if( *(ZSpaceI+w) < (double)min ) 
			
					min = (double) *(ZSpaceI+w);
		
		} // y dim
        
	}  // x dim
	/// compute max and min of data (end)

	for (i=0; i<rcyres; i++)
	{
		for (j=0; j<rcxres; j++)
		{

				w = ((j*NofYpixels)+i);

				if ( max == min ) *(ZSpaceI+w) = (double)0.0;

				else { 
				
						*(ZSpaceI+w) = (double) fabs( (min - (double)*(ZSpaceI+w) ) / (min - max) );

						*(ZSpaceI+w) *= ft_SCALE_z;

				}

		} // y dim
        
	}  // x dim
	//// Procedure to scale (end)


	double savedata = 0.0;
	char Zfilename[128];

	sprintf(Zfilename, "%s%s", "Z-SpaceR-", imageFilename);

    fprintf(logfile, "%s\t%s\n", "Now Saving Z-Space Signal (Real) in File: ", Zfilename);

    if ((pf = fopen(Zfilename,"wb+"))==NULL)
	{

	 fprintf(logfile, "%s\n", "Cannot open file to save Z-Space Signal");


	 // FIFO memory deallocation method
 	 free(ZSpaceR);
 	 free(ZSpaceI);
	 free(Signal);

	 exit(0);
	
	} else { // save data


	for (i=0; i<NofYpixels; i++)
	{ ///save Z-Space data
		for (j=0; j<NofXpixels; j++)
		{

			index = ((j*NofYpixels)+i);

			savedata = (double)*(ZSpaceR+index);
          
            fwrite(&savedata,sizeof(double),1,pf);

		}
	} ///save Z-Space data

	fprintf(logfile,"%s\n", "Z-Space Signal (Real) Saved");

	fclose (pf);
	} // save data



	sprintf(Zfilename, "%s%s", "Z-SpaceI-", imageFilename);

    fprintf(logfile, "%s\t%s\n", "Now Saving Z-Space Signal (Imaginary) in File: ", Zfilename);

    if ((pf = fopen(Zfilename,"wb+"))==NULL)
	{

	 fprintf(logfile, "%s\n", "Cannot open file to save Z-Space Signal");

	 // FIFO memory deallocation method
	 free(ZSpaceR);
	 free(ZSpaceI);
	 free(Signal);

	 exit(0);
	
	} else { // save data


	for (i=0; i<NofYpixels; i++)
	{ ///save Z-Space data
		for (j=0; j<NofXpixels; j++)
		{

			index = ((j*NofYpixels)+i);

			savedata = (double)*(ZSpaceI+index);
          
            fwrite(&savedata,sizeof(double),1,pf);

		}
	} ///save Z-Space data

	fprintf(logfile,"%s\n", "Z-Space Signal (Imaginary) Saved");

	fclose (pf);
	
	} // save data

	sprintf(Zfilename, "%s%s", "Z-SpaceM-", imageFilename);

    fprintf_s(logfile, "%s\t%s\n", "Now Saving Z-Space Magnitude of the Signal in File: ", Zfilename);

    if ((pf = fopen(Zfilename,"wb+"))==NULL)
	{

	 fprintf_s(logfile, "%s\n", "Cannot open file to save Z-Space Magnitude of the Signal");

	 // FIFO memory deallocation method
	 free(ZSpaceR);
	 free(ZSpaceI);
	 free(Signal);

	 exit(0);
	
	} else { // save data	

		// save a zero image (begin)
		for (int s=0; s<NofYpixels; s++)
		{ 
			for (int p=0; p<NofXpixels; p++)
			{ 

			savedata = (double)0.0;
          
            fwrite(&savedata,sizeof(double),1,pf);

			}
		} // save a zero image (end)

	fclose(pf);
	
	}
		
	if ((pf = fopen(Zfilename,"wb+"))==NULL)
	{

	 fprintf_s(logfile, "%s\n", "Cannot open file to save Z-Space Magnitude of the Signal");

	 // FIFO memory deallocation method
	 free(ZSpaceR);
	 free(ZSpaceI);
	 free(Signal);

	 exit(0);
	
	} else { // save data
		
		// Z-Space Magnitude (begin)
		for (int s=0; s<(int)NofYpixels; s++)
		{ 
			for (int p=0; p<(int)NofXpixels; p++)
			{ 
			
		
			index = ((p*NofYpixels)+s);

			savedata = (double) sqrt( (double)*(ZSpaceR+index)*(double)*(ZSpaceR+index) + 
		   		                      (double)*(ZSpaceI+index)*(double)*(ZSpaceI+index) );
          
            fwrite(&savedata,sizeof(double),1,pf);
			
		}
	} // Z-Space Magnitude (end)

	fprintf_s(logfile,"%s\n", "Z-Space Magnitude of the Signal Saved");

	fclose (pf);
	} // save data

	printf("%s\n", "Z Processing Completed");
    fprintf_s(logfile,"%s\n", "Z Processing Completed");

	fclose(logfile);
	

	// FIFO memory deallocation method
	free(ZSpaceR);
	free(ZSpaceI);
	free(Signal);

}

void OnZTransform(char imageFilename[], int rcxres, int rcyres, double m_Real, double m_Imaginary)
{
	
	int NofXpixels = rcxres;
	int NofYpixels = rcyres;

	int i, j, index;
	int dx, dy;
	int ds, dp; 
	int k2, k3, w, t;
	
	double pi = 3.141592;

	double * ZSpaceR = 0;
	double * ZSpaceI = 0;
	double * Signal = 0;

	FILE * logfile;
	
	char logfilename[128]="Z-T.log";

  	if ((logfile = fopen(logfilename,"w+"))==NULL)
	{

		printf("%s\n %s\n" , "Unable to open log File", "Now Exit");

		exit(0);
	
	} else { // allocate memory 


	if ((ZSpaceR = (double *) calloc( NofXpixels*NofYpixels, sizeof(double)) ) == NULL)
	{
   
		fprintf(logfile,"%s\n", "Not enough memory to allocate Real Image data: Exit");
   
		exit(0);

	}

	if ((ZSpaceI = (double *) calloc( NofXpixels*NofYpixels, sizeof(double)) ) == NULL)
	{
   
		fprintf(logfile,"%s\n", "Not enough memory to allocate Real Image data: Exit");

		// FIFO memory deallocation method
		free(ZSpaceR);
		exit(0);

	}

	if ((Signal = (double *) calloc( NofXpixels*NofYpixels, sizeof(double)) ) == NULL)
	{
   
		fprintf(logfile,"%s\n", "Not enough memory to allocate Real Image data: Exit");

		// FIFO memory deallocation method
		free(ZSpaceR);
		free(ZSpaceI);
		exit(0);

	}

	} // allocate memory 

	//// read image data and initialize pointers
	double number = 0.0;

		for (i=0; i<NofYpixels; i++)
		{ 
			for (j=0; j<NofXpixels; j++)
			{

				index = ((j*NofYpixels)+i);
				
				*(ZSpaceR+index) = (double) 0.0;

				*(ZSpaceI+index) = (double) 0.0;

			}

		}

	FILE * pf;
	char SignalFilename[128];
	double readData;
	
	sprintf(SignalFilename, "%s", imageFilename);

	if ((pf = fopen(SignalFilename,"rb+"))==NULL)
	{

	 fprintf(logfile, "%s\n", "Cannot open file to read Signal");

	 // FIFO memory deallocation method
	 free(ZSpaceR);
	 free(ZSpaceI);
	 free(Signal);

	 exit(0);
	
	} else { // read data


	for (i=0; i<rcyres; i++)
	{ ///read signal data
		for (j=0; j<rcxres; j++)
		{

			index = ((j*rcyres)+i);
          
            fread(&readData,sizeof(double),1,pf);

			*(Signal+index) = (double)readData;

		}
	} ///read signal data

	fprintf(logfile,"%s\n", "Signal Read in DOUBLE (64bits) format");

	fclose (pf);
	} // save data


	double phase;
	double exponent, r;
	double complexRZ, complexIZ;
		
	double magnitude = (double) sqrt ( (double)m_Real*m_Real + (double)m_Imaginary*m_Imaginary );
	double Zphase = (double)2.0*pi*atan2( (double)m_Imaginary, (double)m_Real ) / ((double)NofXpixels*NofYpixels);

	///// Z Transform //////
	for (i=0; i<NofYpixels; i++)
	{ ///calculate Z-Space data

		for (j=0; j<NofXpixels; j++)
		{

	
			dx = ((int) i - NofYpixels/2);
		    dy = ((int) j - NofXpixels/2);

			k2 = ((int)(dy*NofYpixels)+dx); 

			w = ((j*NofYpixels)+i);

			for (int s=0; s<NofYpixels; s++)
			{ ///calculate Z-Space data 
				for (int p=0; p<NofXpixels; p++)
				{ 
					

		     		ds = ((int) s - NofYpixels/2);
		            dp = ((int) p - NofXpixels/2);
 
				    k3 = ((int)(ds*NofXpixels)+dp); 

					t = ((p*NofYpixels)+s);


		 		    phase = ((double) (2.0 * pi * k2 * k3) / (NofXpixels*NofYpixels));

					exponent = (double) 2.0 * pi * t * (double)Zphase / ((double) pow((double)NofXpixels*NofYpixels, 2.0));

					exponent = (double)fabs((double)exponent);

					r = (double) pow( (double)magnitude, -(double)exponent );


					complexRZ = (double) cos( (double)phase ) + (double) sin( (double)phase );
					
					complexIZ = -(double) sin( (double)phase ) + (double) cos( (double)phase ); 

					
					*(ZSpaceR+w) += (double)*(Signal+t) * (double)complexRZ * (double)r; 

					*(ZSpaceI+w) += (double)*(Signal+t) * (double)complexIZ * (double)r;
			}

		}///calculate Z-Space data 

			    
		}
	} ///calculate Z-Space data
	///// Z Transform //////

	double savedata = 0.0;
	char Zfilename[128];

	sprintf(Zfilename, "%s%s", "Z-SpaceR-", imageFilename);

    fprintf(logfile, "%s\t%s\n", "Now Saving Z-Space Signal (Real) in File: ", Zfilename);

    if ((pf = fopen(Zfilename,"wb+"))==NULL)
	{

	 fprintf(logfile, "%s\n", "Cannot open file to save Z-Space Signal");


	 // FIFO memory deallocation method
 	 free(ZSpaceR);
 	 free(ZSpaceI);
	 free(Signal);

	 exit(0);
	
	} else { // save data


	for (i=0; i<NofYpixels; i++)
	{ ///save Z-Space data
		for (j=0; j<NofXpixels; j++)
		{

			index = ((j*NofYpixels)+i);

			savedata = (double)*(ZSpaceR+index);
          
            fwrite(&savedata,sizeof(double),1,pf);

		}
	} ///save Z-Space data

	fprintf(logfile,"%s\n", "Z-Space Signal (Real) Saved");

	fclose (pf);
	} // save data



	sprintf(Zfilename, "%s%s", "Z-SpaceI-", imageFilename);

    fprintf(logfile, "%s\t%s\n", "Now Saving Z-Space Signal (Imaginary) in File: ", Zfilename);

    if ((pf = fopen(Zfilename,"wb+"))==NULL)
	{

	 fprintf(logfile, "%s\n", "Cannot open file to save Z-Space Signal");

	 // FIFO memory deallocation method
	 free(ZSpaceR);
	 free(ZSpaceI);
	 free(Signal);

	 exit(0);
	
	} else { // save data


	for (i=0; i<NofYpixels; i++)
	{ ///save Z-Space data
		for (j=0; j<NofXpixels; j++)
		{

			index = ((j*NofYpixels)+i);

			savedata = (double)*(ZSpaceI+index);
          
            fwrite(&savedata,sizeof(double),1,pf);

		}
	} ///save Z-Space data

	fprintf(logfile,"%s\n", "Z-Space Signal (Imaginary) Saved");

	fclose (pf);
	
	} // save data

	sprintf(Zfilename, "%s%s", "Z-SpaceM-", imageFilename);

    fprintf_s(logfile, "%s\t%s\n", "Now Saving Z-Space Magnitude of the Signal in File: ", Zfilename);

    if ((pf = fopen(Zfilename,"wb+"))==NULL)
	{

	 fprintf_s(logfile, "%s\n", "Cannot open file to save Z-Space Magnitude of the Signal");

	 // FIFO memory deallocation method
	 free(ZSpaceR);
	 free(ZSpaceI);
	 free(Signal);

	 exit(0);
	
	} else { // save data	

		// save a zero image (begin)
		for (int s=0; s<NofYpixels; s++)
		{ 
			for (int p=0; p<NofXpixels; p++)
			{ 

			savedata = (double)0.0;
          
            fwrite(&savedata,sizeof(double),1,pf);

			}
		} // save a zero image (end)

	fclose(pf);
	
	}
		
	if ((pf = fopen(Zfilename,"wb+"))==NULL)
	{

	 fprintf_s(logfile, "%s\n", "Cannot open file to save Z-Space Magnitude of the Signal");

	 // FIFO memory deallocation method
	 free(ZSpaceR);
	 free(ZSpaceI);
	 free(Signal);

	 exit(0);
	
	} else { // save data
		
		// Z-Space Magnitude (begin)
		for (int s=0; s<(int)NofYpixels; s++)
		{ 
			for (int p=0; p<(int)NofXpixels; p++)
			{ 
			
		
			index = ((p*NofYpixels)+s);

			savedata = (double) sqrt( (double)*(ZSpaceR+index)*(double)*(ZSpaceR+index) + 
		   		                      (double)*(ZSpaceI+index)*(double)*(ZSpaceI+index) );
          
            fwrite(&savedata,sizeof(double),1,pf);
			
		}
	} // Z-Space Magnitude (end)

	fprintf_s(logfile,"%s\n", "Z-Space Magnitude of the Signal Saved");

	fclose (pf);
	} // save data

	printf("%s\n", "Z Processing Completed");
    fprintf_s(logfile,"%s\n", "Z Processing Completed");

	fclose(logfile);
	

	// FIFO memory deallocation method
	free(ZSpaceR);
	free(ZSpaceI);
	free(Signal);

}

void OnTransferFunction(unsigned char filterType, int rcxres, int rcyres, int n_poles, double m_cof, double m_Bandwidth, double m_TheSamplingRate, double m_Real, double m_Imaginary)
{
	
	int NofXpixels = rcxres;
	int NofYpixels = rcyres;

	int i, j, index;
	double pi = 3.141592;
	
	double * tf = 0;

	FILE * logfile;
	
	char logfilename[128]="TF.log";

  	if ((logfile = fopen(logfilename,"w+"))==NULL)
	{

		printf("%s\n %s\n" , "Unable to open log File", "Now Exit");

		exit(0);
	
	} else { // allocate memory 


	if ((tf = (double *) calloc( NofXpixels*NofYpixels, sizeof(double)) ) == NULL)
	{
   
		fprintf(logfile,"%s\n", "Not enough memory to allocate Real Image data: Exit");

		exit(0);

	}

	} // allocate memory 

	struct BesselPolynomial {

		double **ak; // pointer to the matrix entry

	}*BesselPolynomial_pointer; // pointer to the matrices

	 
	 BesselPolynomial_pointer = new BesselPolynomial;

	 
	 BesselPolynomial_pointer->ak = new double*[n_poles+1];


	 for( int v=0; v <= n_poles; v++ ) { // (1)

		 BesselPolynomial_pointer->ak[v] = new double[1];

	  } // (1) allocate struct 'data' (end)
	

	    // (2) initialize (begin)
		for( int v=0; v <= n_poles; v++ ) { // (a)

			for( int f=0; f < 1 ; f++ ) { // (b)

			BesselPolynomial_pointer->ak[v][f] = (double)1.0;

			} //(b)

		} //(a)
		// (2) initialize (end)


		double product_b = 1.0;
		double product_c = 1.0;
		double product_d = 1.0;
		double product_e = 1.0;
				
		for (int k=0; k <= n_poles; k++)
		{ 
	       
		if ( ((double) 2.0 * n_poles - (double) k) == 0 )

			  product_b *= (double) 1.0;

		else  product_b *= ((double) 2.0 * n_poles - (double) k);
		
		
		product_c = (double) pow ( (double) 2.0 , ((double)n_poles - k) );
				
		
		if (k == 0) product_d *= (double) 1.0;
		else          product_d *= (double)k;

		// this factor ' product_e ' can be zero as last multiplier of the
		// factorial series, and as such is 0! = 1.
		if (n_poles == k)

			product_e *= (double)1.0;

		else	product_e *= ((double)n_poles - k);
		// this factor ' product_e ' can be zero as last multiplier of the
		// factorial series, and as such is 0! = 1.

		if ( ((double) product_c * product_d * product_e) != 0.0 )

		BesselPolynomial_pointer->ak[k][0] = (double) product_b / ((double) product_c * product_d * product_e);

		else BesselPolynomial_pointer->ak[k][0] = (double) 1.0;

		}



	FILE * pf;
	char filename[328];
	double savedata = 0;
	double savedata2 = 0;

	sprintf(filename, "%s", "Bessel.img");

    fprintf_s(logfile, "%s\t%s\n", "Now Saving Bessel Space in File: ", filename);

    if ((pf = fopen(filename,"wb+"))==NULL)
	{

	 fprintf_s(logfile, "%s\n", "Cannot open file to save Bessel Space Signal");
	
	 free(tf);
	 delete BesselPolynomial_pointer;
	 
	 exit(0);
	
	} else { // save data


	for (i=0; i<=n_poles; i++)
	{ ///save Bessel-Polynomial coefficients data
		
			savedata = (double)BesselPolynomial_pointer->ak[i][0];
          
            fwrite(&savedata,sizeof(double),1,pf);

	}///save Bessel-Polynomial coefficients data

	fprintf_s(logfile,"%s\n", "Bessel-Polynomial coefficients Saved");

	fclose (pf);
	} // save data


	    //// initialize pointer
		for (i=0; i<NofYpixels; i++)
		{ 
			for (j=0; j<NofXpixels; j++)
			{

				index = ((j*NofYpixels)+i);
								
				*(tf+index) = (double) 0.0;

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
					
					
				for (i=0; i<NofYpixels; i++)
				{ ///calculate TF-Space data
		
					for (j=0; j<NofXpixels; j++)
					{

					di = ((int) i - NofYpixels/2);
					dj = ((int) j - NofXpixels/2);

					dty = ((double)((int) i - NofYpixels/2) * m_TheSamplingRate); //space (pixel) * frequency (1/sec)
					dtx = ((double)((int) j - NofXpixels/2) * m_TheSamplingRate); //space (pixel) * frequency (1/sec)
					
					/// Whittaker-Shannon interpolation formula 2010
					dy = (double) pi * (2.0 * m_Bandwidth * di - dty); 
					// Where 2.0 * bandwidth * di is: 
					// frequency (1/sec) * space (pixel)
					dx = (double) pi * (2.0 * m_Bandwidth * dj - dtx);
					//		 2.0 * bandwidth * dj is: 
					// frequency (1/sec) * space (pixel)
					/// Whittaker-Shannon interpolation formula 2010
					
					t = ((j*NofYpixels)+i);


					double phase = (double)2.0 * pi * atan2((double)m_Imaginary, (double)m_Real);

					double magnitude = (double)sqrt((double)(((double)dx * dx) + ((double)dy * dy)));

					double complexR = (double)magnitude * (double)cos((double)phase);
					double complexI = (double)magnitude * (double)sin((double)phase);
			
					double samplingPeriod = 2.0 * pi * (double)pow(((double)NofXpixels * NofYpixels), 1.0/m_cof);

					double T = ((double)m_cof) / ((double)tan((double)m_cof*samplingPeriod/2.0));

					// bilinear transform to change the variable from 's' to 'z'
					complex = ((double)2.0 / T) * ((double) complexR + complexI - 1.0) / ((double) complexR + complexI + 1.0);
		     				
				
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
					if ( (double)fabs( (double) dx ) > toll )
							
					sincx = (double) sin( (double) dx ) /  ((double) dx);
	
					else sincx = (double)1.0;

					if ( (double)fabs( (double) dy ) > toll )
							
					sincy = (double) sin( (double) dy ) /  ((double) dy);
			
					else sincy = (double)1.0;
					///// Wittaker-Shannon sampling 2010//////
					

					if ( ( (double) Bessel_Polynomial ) != 0.0 ) {
					
					if ( (_isnan(((double) Bessel_Polynomial_Zero / Bessel_Polynomial))) == 0 ) { 					
						
						*(tf + t) = ((double)Bessel_Polynomial_Zero / Bessel_Polynomial);
						
						*(tf + t) *= ((double) sincx * sincy);

					} else { *(tf+t) = (double) 0.0; }

				    } else { *(tf+t) = (double) 0.0; }
					
			
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

						index = ((j*rcyres) + i);

						if (*(tf + index) > (double)max)

							max = (double) *(tf + index);

						if (*(tf + index) < (double)min)

							min = (double) *(tf + index);

					} // y dim

				}  // x dim
				/// compute max and min of data (end)

				if (tolower(filterType) == 'l') {

					// scale (begin)
					for (i = 0; i < rcyres; i++)
					{
						for (j = 0; j < rcxres; j++)
						{

							index = ((j*rcyres) + i);

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

							index = ((j*rcyres) + i);

							if (max == min) *(tf + index) = (double)0.0;

							else {

								// HP = 1.0 - LP
								*(tf + index) = (double) 1.0 - (double)fabs((min - (double)*(tf + index)) / (min - max));

								*(tf + index) *= ft_SCALE;

							}

						} // y dim

					}  // x dim
					// scale (end)
				} //// Procedure to scale the filter (end)




	sprintf(filename, "%s", "TF.img");

    fprintf(logfile, "%s\t%s\n", "Now Saving TF in File: ", filename);

    if ((pf = fopen(filename,"wb+"))==NULL)
	{

	 fprintf(logfile, "%s\n", "Cannot open file to save TF");

	 // FIFO memory deallocation method
	 free(tf);
	 delete BesselPolynomial_pointer;

	 exit(0);
	
	} else { // save data


	for (i=0; i<NofYpixels; i++)
	{ ///save TF data
		for (j=0; j<NofXpixels; j++)
		{

			index = ((j*NofYpixels)+i);

			savedata = (double)*(tf+index);
          
            fwrite(&savedata,sizeof(double),1,pf);

		}
	} ///save Z-Space data

	fprintf(logfile,"%s\n", "TF Saved");

	fclose (pf);
	} // save data


	printf("%s\n", "TF Processing Completed");
    fprintf_s(logfile,"%s\n", "TF Processing Completed");

	fclose(logfile);
	

	 // FIFO memory deallocation method
	 free(tf);
	 delete BesselPolynomial_pointer;

}

void OnInverseZTransformTransferFunction(char filename[], int rcxres, int rcyres, double m_Real, double m_Imaginary)
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
	double scale = ((double)rcxres*rcyres*emittingSource); 
	//2010

	FILE * logfile;
	char logfilename[128]="INV-ZT.log";

	FILE *image;
	char imageFilename[256];

	double * ZSpaceR = 0;
	double * ZSpaceI = 0;
	double * reconSignal = 0;
	double * transferFR = 0;
	double * transferFI = 0;

	if ((logfile = fopen(logfilename,"w+"))==NULL)
	{

	 exit(0);
	
	} else { // allocate memory

		
	printf("%s\n", "Now INV Z Processing...");
    fprintf(logfile,"%s\n", "Now INV Z Processing...");

	if ((ZSpaceR = (double *) calloc( NofXpixels*NofYpixels, sizeof(double)) ) == NULL)
	{
   
		fprintf(logfile,"%s\n", "Not enough memory to allocate Real Image data: Exit");
   
		exit(0);

	}

	if ((ZSpaceI = (double *) calloc( NofXpixels*NofYpixels, sizeof(double)) ) == NULL)
	{
   
		fprintf(logfile,"%s\n", "Not enough memory to allocate Real Image data: Exit");
   
		// FIFO memory deallocation method
		free(ZSpaceR);
		exit(0);

	}


	if ((reconSignal = (double *) calloc( NofXpixels*NofYpixels, sizeof(double)) ) == NULL)
	{
	
		fprintf(logfile,"%s\n", "Not enough memory to allocate Imaginary Image data: Exit");
	
		// FIFO memory deallocation method
		free(ZSpaceR);
		free(ZSpaceI);

		exit(0);

	}

	if ((transferFR = (double *) calloc( NofXpixels*NofYpixels, sizeof(double)) ) == NULL)
	{
   
		fprintf(logfile,"%s\n", "Not enough memory to allocate Real Image data: Exit");
        
		free(ZSpaceR);
		free(ZSpaceI);
		free(reconSignal);
				
		exit(0);

	}

	if ((transferFI = (double *) calloc( NofXpixels*NofYpixels, sizeof(double)) ) == NULL)
	{
   
		fprintf(logfile,"%s\n", "Not enough memory to allocate Real Image data: Exit");
   
		// FIFO memory deallocation method
		free(ZSpaceR);
		free(ZSpaceI);
		free(reconSignal);
		free(transferFR);

		exit(0);

	}

	} // allocate memory

	
	//// read image data and initialize pointers
    sprintf(imageFilename, "%s%s", "Z-SpaceR-", filename);

    if ((image = fopen(imageFilename,"rb+"))==NULL)
	{
	
	 fprintf(logfile, "%s%s\n", "Cannot open Image File: ", imageFilename);
	 
	 // FIFO memory deallocation method
	 free(ZSpaceR);
	 free(ZSpaceI);
	 free(reconSignal);
	 free(transferFR);
	 free(transferFI);

	 exit(0);

	} else { // read data and initialize pointers

		double number = 0.0;

		for (i=0; i<NofYpixels; i++)
		{ 
			for (j=0; j<NofXpixels; j++)
			{

				index = ((j*NofYpixels)+i);

				fread(&number,sizeof(double),1,image);
				
				*(ZSpaceR+index) = (double) number;

		
			}

		}

		fclose(image);

	}// read data and initialize pointers


    char imageFilename2[128];

	sprintf(imageFilename2, "%s%s", "Z-SpaceI-", filename);


    if ((image = fopen(imageFilename2,"rb+"))==NULL)
	{
	
	 fprintf(logfile, "%s%s\n", "Cannot open Image File: ", imageFilename2);

	 // FIFO memory deallocation method
	 free(ZSpaceR);
	 free(ZSpaceI);
	 free(reconSignal);
	 free(transferFR);
	 free(transferFI);
	
	 exit(0);

	} else { // read data and initialize pointers

		double number = 0.0;

		for (i=0; i<NofYpixels; i++)
		{ 
			for (j=0; j<NofXpixels; j++)
			{

				index = ((j*NofYpixels)+i);

				fread(&number,sizeof(double),1,image);
				
				*(ZSpaceI+index) = (double) number;

			}

		}

		fclose(image);


		for (i=0; i<NofYpixels; i++)
		{ 
			for (j=0; j<NofXpixels; j++)
			{

				index = ((j*NofYpixels)+i);

				*(reconSignal+index) = (double)0.0;
					
			}

		}


	}// read data and initialize pointers


	//// read image data and initialize pointers
    sprintf(imageFilename, "%s", "Z-SpaceR-TF.img");

    if ((image = fopen(imageFilename,"rb+"))==NULL)
	{
	
	 fprintf(logfile, "%s%s\n", "Cannot open Image File: ", imageFilename);
	 
	 // FIFO memory deallocation method
	 free(ZSpaceR);
	 free(ZSpaceI);
	 free(reconSignal);
	 free(transferFR);
	 free(transferFI);
	
	 exit(0);

	} else { // read data and initialize pointers

		double number = 0.0;

		for (i=0; i<NofYpixels; i++)
		{ 
			for (j=0; j<NofXpixels; j++)
			{

				index = ((j*NofYpixels)+i);

				fread(&number,sizeof(double),1,image);
				
				*(transferFR+index) = (double) number;

		
			}

		}

		fclose(image);

	}// read data and initialize pointers

	//// read image data and initialize pointers
    sprintf(imageFilename, "%s", "Z-SpaceI-TF.img");

    if ((image = fopen(imageFilename,"rb+"))==NULL)
	{
	
	 fprintf(logfile, "%s%s\n", "Cannot open Image File: ", imageFilename);
	 
	 // FIFO memory deallocation method
	 free(ZSpaceR);
	 free(ZSpaceI);
	 free(reconSignal);
	 free(transferFR);
	 free(transferFI);
	
	 exit(0);

	} else { // read data and initialize pointers

		double number = 0.0;

		for (i=0; i<NofYpixels; i++)
		{ 
			for (j=0; j<NofXpixels; j++)
			{

				index = ((j*NofYpixels)+i);

				fread(&number,sizeof(double),1,image);
				
				*(transferFI+index) = (double) number;

		
			}

		}

		fclose(image);

	}// read data and initialize pointers

	double real = 0.0, imaginary = 0.0;
	double realComponent = 0.0, imaginaryComponent = 0.0;
	double exponent, r;

	double magnitude = (double) sqrt ( (double)m_Real*m_Real + (double)m_Imaginary*m_Imaginary );
	double Zphase = (double)2.0*pi*atan2( (double)m_Imaginary, (double)m_Real ) / ((double)NofXpixels*NofYpixels);

	///// INV Z Transform //////
	for (i=0; i<NofYpixels; i++)
	{ ///process Z-Space data

		for (j=0; j<NofXpixels; j++)
		{
		
	    	dx = ((int) i - NofYpixels/2);
		    dy = ((int) j - NofXpixels/2);
		
			k2 = ((int)(dy * NofYpixels) + dx); 

			w = ((j*NofYpixels)+i);

			
			real = (double)0.0;
			imaginary = (double)0.0;	
			
			for (int s=0; s<NofYpixels; s++)
			{ ///process Z-Space data

				for (int p=0; p<NofXpixels; p++)
				{ 

					ds = ((int) s - NofYpixels/2);
		            dp = ((int) p - NofXpixels/2);

					k3 = ((int)(ds * NofXpixels) + dp);  
				
					t = ((p*NofYpixels)+s);
					
					
					phase = ((double) (2.0 * pi * k2 * k3) / (NofXpixels*NofYpixels));

					exponent = (double) 2.0 * pi * t * (double)Zphase / ((double) pow((double)NofXpixels*NofYpixels, 2.0));

					exponent = (double)fabs((double)exponent);

					r = (double) pow( (double)magnitude, (double)exponent );

					real += ((double) *(ZSpaceR + t) * (double) *(transferFR + t) * (double)cos((double)phase)) -
						    ((double) *(ZSpaceI + t) * (double) *(transferFI + t) * (double)sin((double)phase));
		            
					real *= (double) r;
                    
					imaginary += ((double) *(ZSpaceR + t) * (double) *(transferFR + t) * (double)sin((double)phase)) +
						         ((double) *(ZSpaceI + t) * (double) *(transferFI + t) * (double)cos((double)phase));


					imaginary *= (double) r;
	                				
				}

		}///process Z-Space data 

			realComponent = (double)real * real;

			if ( (_isnan((double)realComponent)) == 0 ) { }

			else { realComponent = (double) 0.0; }


			imaginaryComponent = (double)imaginary * imaginary;

			if ( (_isnan((double)imaginaryComponent)) == 0 ) { }

			else { imaginaryComponent = (double) 0.0; }

		
			
			*(reconSignal+w) =  (double) sqrt( ((double) realComponent) + ((double) imaginaryComponent) );
            
			*(reconSignal+w) /= (double)scale;
			
		}
	} ///process Z-Space data


	double savedata = 0.0;
	FILE * pf;
	char reconFilename[128];

	sprintf(reconFilename, "%s%s", "FilteredSignal-", filename);


    fprintf(logfile, "%s\t%s\n", "Now Saving Filtered Signal in File: ", reconFilename);

    if ((pf = fopen(reconFilename,"wb+"))==NULL)
	{

	 fprintf(logfile, "%s\n", "Cannot open file to save Filtered Signal");

	 // FIFO memory deallocation method
	 free(ZSpaceR);
	 free(ZSpaceI);
	 free(reconSignal);
	 free(transferFR);
	 free(transferFI);

	 exit(0);
	
	} else { // save data


	for (i=0; i<NofYpixels; i++)
	{ ///save Z-Space data
		for (j=0; j<NofXpixels; j++)
		{

			index = ((j*NofYpixels)+i);

			savedata = (double)*(reconSignal+index);
          
            fwrite(&savedata,sizeof(double),1,pf);

		}
	} ///save Z-Space data

	fprintf(logfile,"%s\n", "Filtered Signal Saved");

	fclose (pf);
	} // save data
    printf("%s\n", "Inverse Z Processing Completed");
    fprintf(logfile,"%s\n", "Inverse Z Processing Completed");

	fclose(logfile);
	
		
	 // FIFO memory deallocation method
	 free(ZSpaceR);
	 free(ZSpaceI);
	 free(reconSignal);
	 free(transferFR);
	 free(transferFI);

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

	double * kSpaceR = 0;
	double * kSpaceI = 0;
	double * Signal = 0;

	FILE * logfile;
	
	char logfilename[128]="Fourier-T.log";

  	if ((logfile = fopen(logfilename,"w+"))==NULL)
	{

		printf("%s\n %s\n" , "Unable to open log File", "Now Exit");

		exit(0);
	
	} else { // allocate memory 


	if ((kSpaceR = (double *) calloc( NofXpixels*NofYpixels, sizeof(double)) ) == NULL)
	{
   
		fprintf(logfile,"%s\n", "Not enough memory to allocate Real Image data: Exit");
   
		exit(0);

	}

	if ((kSpaceI = (double *) calloc( NofXpixels*NofYpixels, sizeof(double)) ) == NULL)
	{
   
		fprintf(logfile,"%s\n", "Not enough memory to allocate Real Image data: Exit");

		// FIFO memory deallocation method
		free(kSpaceR);
		exit(0);

	}

	if ((Signal = (double *) calloc( NofXpixels*NofYpixels, sizeof(double)) ) == NULL)
	{
   
		fprintf(logfile,"%s\n", "Not enough memory to allocate Real Image data: Exit");

		// FIFO memory deallocation method
		free(kSpaceR);
		free(kSpaceI);
		exit(0);

	}

	} // allocate memory 

	//// read image data and initialize pointers
	double number = 0.0;

		for (i=0; i<NofYpixels; i++)
		{ 
			for (j=0; j<NofXpixels; j++)
			{

				index = ((j*NofYpixels)+i);
				
				*(kSpaceR+index) = (double) 0.0;

				*(kSpaceI+index) = (double) 0.0;

			}

		}

	FILE * pf;
	char SignalFilename[128];
	double readData;
	
	sprintf(SignalFilename, "%s", imageFilename);

	if ((pf = fopen(SignalFilename,"rb+"))==NULL)
	{

	 fprintf(logfile, "%s\n", "Cannot open file to read Signal");

	 // FIFO memory deallocation method
	 free(kSpaceR);
	 free(kSpaceI);
	 free(Signal);

	 exit(0);
	
	} else { // read data


	for (i=0; i<rcyres; i++)
	{ ///read signal data
		for (j=0; j<rcxres; j++)
		{

			index = ((j*rcyres)+i);
          
            fread(&readData,sizeof(double),1,pf);

			*(Signal+index) = (double)readData;

		}
	} ///read signal data

	fprintf(logfile,"%s\n", "Signal Read in DOUBLE (64bits) format");

	fclose (pf);
	} // save data

	double phase, complexR, complexI;
	
	///// Fourier Transform //////
	for (i=0; i<NofYpixels; i++)
	{ ///calculate k-space data

		for (j=0; j<NofXpixels; j++)
		{

	
			dx = ((int) i - NofYpixels/2);
		    dy = ((int) j - NofXpixels/2);

			k2 = ((int)(dy*NofYpixels)+dx); 

			w = ((j*NofYpixels)+i);

			for (int s=0; s<NofYpixels; s++)
			{ ///calculate k-space data 
				for (int p=0; p<NofXpixels; p++)
				{ 
					

		     		ds = ((int) s - NofYpixels/2);
		            dp = ((int) p - NofXpixels/2);
 
				    k3 = ((int)(ds*NofXpixels)+dp); 

					t = ((p*NofYpixels)+s);

					phase = ((double) (2.0 * pi * k2 * k3) / (NofXpixels*NofYpixels));

					//** nayuki.eigenstate.org/page/how-to-implement-the-discrete-fourier-transform (begin)**/
					complexR = (double) cos( (double)phase ) + (double) sin( (double)phase ); 

					complexI = -(double) sin( (double)phase ) + (double) cos( (double)phase ); 
					//** nayuki.eigenstate.org/page/how-to-implement-the-discrete-fourier-transform (end)**/
				
					*(kSpaceR+w) += (double) *(Signal+t) * (double) complexR;

					*(kSpaceI+w) += (double) *(Signal+t) * (double) complexI;

			}

		}///calculate k-space data 

			    
		}
	} ///calculate k-space data

	///// Fourier Transform //////
	double savedata = 0.0;
	char FTfilename[128];

	sprintf(FTfilename, "%s%s", "K-SpaceR-", imageFilename);

    fprintf(logfile, "%s\t%s\n", "Now Saving K-Space Signal (Real) in File: ", FTfilename);

    if ((pf = fopen(FTfilename,"wb+"))==NULL)
	{

	 fprintf(logfile, "%s\n", "Cannot open file to save K-Space Signal");


	 // FIFO memory deallocation method
 	 free(kSpaceR);
 	 free(kSpaceI);
	 free(Signal);

	 exit(0);
	
	} else { // save data


	for (i=0; i<NofYpixels; i++)
	{ ///save k-space data
		for (j=0; j<NofXpixels; j++)
		{

			index = ((j*NofYpixels)+i);

			savedata = (double)*(kSpaceR+index);
          
            fwrite(&savedata,sizeof(double),1,pf);

		}
	} ///save k-space data

	fprintf(logfile,"%s\n", "K-Space Signal (Real) Saved");

	fclose (pf);
	} // save data



	sprintf(FTfilename, "%s%s", "K-SpaceI-", imageFilename);

    fprintf(logfile, "%s\t%s\n", "Now Saving K-Space Signal (Imaginary) in File: ", FTfilename);

    if ((pf = fopen(FTfilename,"wb+"))==NULL)
	{

	 fprintf(logfile, "%s\n", "Cannot open file to save K-Space Signal");

	 // FIFO memory deallocation method
	 free(kSpaceR);
	 free(kSpaceI);
	 free(Signal);

	 exit(0);
	
	} else { // save data


	for (i=0; i<NofYpixels; i++)
	{ ///save k-space data
		for (j=0; j<NofXpixels; j++)
		{

			index = ((j*NofYpixels)+i);

			savedata = (double)*(kSpaceI+index);
          
            fwrite(&savedata,sizeof(double),1,pf);

		}
	} ///save k-space data

	fprintf(logfile,"%s\n", "K-Space Signal (Imaginary) Saved");

	fclose (pf);
	
	} // save data

	sprintf(FTfilename, "%s%s", "K-SpaceM-", imageFilename);

    fprintf_s(logfile, "%s\t%s\n", "Now Saving K-Space Magnitude of the Signal in File: ", FTfilename);

	if ((pf = fopen(FTfilename,"wb+"))==NULL)
	{

	 fprintf_s(logfile, "%s\n", "Cannot open file to save K-Space Magnitude of the Signal");

	 // FIFO memory deallocation method
	 free(kSpaceR);
	 free(kSpaceI);
	 free(Signal);

	 exit(0);
	
	} else { // save data
		
		// K-Space Magnitude (begin)
		for (int s=0; s<(int)NofYpixels; s++)
		{ 
			for (int p=0; p<(int)NofXpixels; p++)
			{ 
			
		
			index = ((p*NofYpixels)+s);

			savedata = (double) sqrt( (double)*(kSpaceR+index)*(double)*(kSpaceR+index) + 
		   		                      (double)*(kSpaceI+index)*(double)*(kSpaceI+index) );
          
            fwrite(&savedata,sizeof(double),1,pf);
			
		}
	} // K-Space Magnitude (end)

	fprintf_s(logfile,"%s\n", "K-Space Magnitude of the Signal Saved");

	fclose (pf);
	} // save data

	printf("%s\n", "FT Processing Completed");
    fprintf_s(logfile,"%s\n", "FT Processing Completed");

	fclose(logfile);
	

	// FIFO memory deallocation method
	free(kSpaceR);
	free(kSpaceI);
	free(Signal);

}

