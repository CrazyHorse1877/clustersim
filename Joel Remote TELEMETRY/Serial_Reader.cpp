#include <windows.h>
#include "SFile.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>
using namespace std;

double Calculation(double T1, double T2,double X1, double X2,double Y1, double Y2,double Z1, double Z2,double C1, double C2)
{
	double T_Cal =0;
	double X_Cal =0;
	double Y_Cal =0;
	double Z_Cal =0;
	double C_Cal =0;
	
	double T_test = (sqrt(T1*T1))*0.8;
	double X_test = (sqrt(X1*X1))*0.8;
	double Y_test = (sqrt(Y1*Y1))*0.8;
	double Z_test = (sqrt(Z1*Z1))*0.8;
	double C_test = (sqrt(C1*C1))*0.8;
	
	if (T_test>T2){T1 = T2;}
	if (X_test>X2){X1 = X2;}
	if (Y_test>Y2){Y1 = Y2;}
	if (Z_test>Z2){Z1 = Z2;}
	if (C_test>C2){T1 = T2;}
	

		for (int p = 0;p<20;p++)
		{
			T_Cal = 0.05*p*T1 +0.05*(20-p)*T2;
			X_Cal = 0.05*p*X1 +0.05*(20-p)*X2;
			Y_Cal = 0.05*p*Y1 +0.05*(20-p)*Y2;
			Z_Cal = 0.05*p*Z1 +0.05*(20-p)*Z2;
			C_Cal = 0.05*p*C1 +0.05*(20-p)*C2;
			
			cout<<"\n"<<"\n"<<"Time			:"<<T_Cal<<"\n";
			cout<<"Longitude		:"<<X_Cal<<"\n";
			cout<<"Latitude		:"<<Y_Cal<<"\n";
			cout<<"Altitude		:"<<Z_Cal<<"\n";
			cout<<"Course over ground	:"<<C_Cal<<"\n";
		
			//cout<<T_test<<"\n";

			Sleep(50);
		}
		return 0;
}
double str_to_double(string input) {
    double output = 0;
    bool isBeforeDot = true;

    int i;
    int j = 1;
    int temp;

    for (i = 0; i < (int)input.size(); i++) {
        temp = input[i] - '0'; 

        if (input[i] == '.') {
            isBeforeDot = false;
        } else {
            if (isBeforeDot) {
                output = output * 10 + temp;
            } else {
                output = output + (temp * (double)pow(0.1, j));
                j += 1;
            }
        }
    }

    return output;
}

int main () {
	
	SFile testing;

	if (!testing.IsOpen()) {
		if (testing.Open(1, 4800)) {
			cout << "COM1 open";
		} else {
			cout << "COM1 failed to open!";
		}
	}

	BYTE input;
	string GPS_Time;
	string GPS_Longitude;
	string GPS_Latitude;
	string GPS_Altitude;
	string in[100];
	string in_2[100];
	string OldGPS_Altitude;
	string OldGPS_COG;
	//for interpolation
	double IP_GPSTime[2];
	double IP_GPSCOG[2];
	double IP_GPSLatitude[2];
	double IP_GPSLongitude[2];
	double IP_GPSAltitude[2];
	
	IP_GPSTime[1] = 0;
	IP_GPSLatitude[1] = 0;
	IP_GPSLongitude[1] = 0;
	IP_GPSAltitude[1] = 0;
	IP_GPSCOG[1] = 0;

	while (1) 
	{
		// START OF DO NOT TOUCH
readagain:
	IP_GPSTime[2] = IP_GPSTime[1];
	IP_GPSLatitude[2] = IP_GPSLatitude[1];
	IP_GPSLongitude[2] = IP_GPSLongitude[1];
	IP_GPSAltitude[2] = IP_GPSAltitude[1];
	IP_GPSCOG[2] = IP_GPSCOG[1];
		
		//testing.ReadByte(input);

//		if (input != 'P') {
//			goto readagain;
//		}
		
		testing.ReadByte(input);
		
		int i = 0;
		
		//RMC does not have altitude
		if(in[9] !=""){
		OldGPS_Altitude = in[9];
		}

		if(in_2[8] !=""){
		OldGPS_COG = in_2[8];
		}
		//
		string in[100];
		string in_2[100];
		
		if (input == 'R') {
			goto readRMC;
		}
		

		testing.ReadByte(input);
		
		if (input != 'G') {
			goto readagain;
		}

		testing.ReadByte(input);
		
		if (input != 'A') {
			goto readagain;
		}

		testing.ReadByte(input);
		
		
		while (input != '\n') {
			if (input == ',') {
				i += 1;
			} else {
				in[i] += input;
			}
			//cout << input;
			testing.ReadByte(input);
		}
		i += 1;
		
output_data:
		//Special non-repeated parameters
		if(in[9] ==""){
			in[9] = OldGPS_Altitude;
		}
		if(in_2[8] ==""){
			in_2[8] = OldGPS_COG;
		}
		//Conversion
		IP_GPSTime[1] = str_to_double(in[1]);
		IP_GPSLatitude[1] = str_to_double(in[2]);
		IP_GPSLongitude[1] = str_to_double(in[4]);
		IP_GPSAltitude[1] = str_to_double(in[9]);
		IP_GPSCOG[1] = str_to_double(in_2[8]);
		
		Calculation(IP_GPSTime[1], IP_GPSTime[2],IP_GPSLongitude[1],IP_GPSLongitude[2],
			IP_GPSLatitude[1], IP_GPSLatitude[2],IP_GPSAltitude[1],IP_GPSAltitude[2],
			IP_GPSCOG[1],IP_GPSCOG[2]);


		//Cout
		//cout<<in[1];
		//cout.precision(15); cout<<IP_GPSTime[1]<<"\n";

		//GPS_Time = in[1];
		//GPS_Latitude = in[2];
		//GPS_Longitude = in[4];
		//GPS_Altitude = in[9];
		
//		cout << '\n';
//		cout << "Time is			: " << in[1] << " " << "\n";
//		cout << "Latitude is		: " << in[2] << "\n";
//		cout << "Longitude is		: " << in[4] <<"\n";
//		cout << "Altitude is		: " << in[9] <<"\n";
//		cout << "Course Over Ground is	: " << in_2[8] << "\n";
		cout << "\n";
		OldGPS_Altitude = in[9];

		//

readRMC:

		int i_2 = 0;

		testing.ReadByte(input);

		if (input != 'M') {
			goto readagain;
		}

		testing.ReadByte(input);

		if (input != 'C') {
			goto readagain;
		}
		
		testing.ReadByte(input);
		
		while (input != '\n') {
			if (input == ',') {
				i_2 += 1;
			} else {
				in_2[i_2] += input;
			}
			//cout << input;
			testing.ReadByte(input);
		}
		i_2 += 1;
		
		if(in[1] == "" && in[2] == "" && in[4] == "")
		{
			in[1] = in_2[1];	
			in[2] = in_2[3];
			in[4] = in_2[5];
			in[9] = OldGPS_Altitude;
			if(in_2[8] !=""){
				OldGPS_COG = in_2[8];}  
		}
	
		// END OF DO NOT TOUCH

			goto output_data;
		

	}

	testing.Close();

	return 0;
}