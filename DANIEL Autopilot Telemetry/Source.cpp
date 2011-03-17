#include <stdio.h>

#include "c_sdk_fly.h"
#include "c_sdk_initializeuav.h"
#include "c_sdk_readwrite.h"
#include "c_sdk_telemetry.h"

#include "../../../include/c-multi.h"
#include "../../../include/mperrors.h"
#include "../../../include/mpfields.h"
#include "../../../include/mpconsts.h"


long CreatePlane(long planeId)
{
    long retVal = 0;
    long value = 0;

    /* Do...while loop is used so we can break out of them if there is an error */
    do
    {
        /* This Funtion will create the uav#.dll */
        printf("\nCreating UAV %d", planeId);
        retVal = mpCreate(planeId);
        if ( retVal != MP_OK )      
        {
            printf("\nERROR - could not Create UAV #: %d Error: %d", planeId, retVal);  
            break;
        }

        /* This Function will initialize the simulate and load the default.fly */
        retVal = mpInitFly(planeId, "default.fly");
        if ( retVal != MP_OK )
        {
            printf("\nERROR - could not Initialize Simulator #: %d Error: %d", planeId, retVal);    
            break;
        }

		/* This function will Initialize the comport to the planeId */
        
		//retVal = mpInitLink(planeId, "com4"); /* by leaving comport blank "" we tell it to use the entry from the uav<planeId>.ini */
        //if ( retVal != MP_OK )
        //{
         //   printf("\nERROR - could not Initialize Link To Comport Error: %d", retVal); 
         //   break;
        //}


    } while (retVal != MP_OK);

    return retVal;

}
long InitializeFlyPlane(long planeId)
{
    long retVal = 0;
    long value = 0;

    /* Do...while loop is used so we can break out of them if there is an error */
    do
    {
        /* Since we dont want the simulator to run in a background thread we use mpFly to start it */
        printf("\nAttempting to Start Flying");
        retVal = mpFly(planeId);
        if (retVal != MP_OK)
        {
            printf("\nError : mpFly, retVal: %d", retVal);
            break;
        }
       
        /* Check the take off state, if it is equl to 1 then we are ready to take off, set it to 2 */
        retVal = mpReadVar(planeId, MPFID_TAKEOFF_STATE, &value, SIM_MODE, QUICK_MODE);
        if (retVal != MP_OK)
        {
            printf("\nError : mpReadVar, retVal: %d", retVal);
            break;
        }
        if (value == 1)
        {
            /* set takeoff state to "2" to initiate takeoff */
            retVal = mpWriteVar(planeId , MPFID_TAKEOFF_STATE , 2, SIM_MODE, QUICK_MODE);
            if (retVal != MP_OK)
            {
                printf("\nError : mpWriteVar, retVal: %d", retVal);
                break;
            }
            printf(".......TakeOff Initiated!");
        }
        else
        {
            printf(".......TakeOff Failed");
            retVal = MP_BAD_VAR_ID;
        }
        /* retVal != MP_OK is used becuse the last thing we try to do is set the takeoff field to 2
           if that is good then we can exit */
    } while(retVal != MP_OK);

    return retVal;
}
long CheckInitialize(long planeId)
{
    long retVal = MP_MORE;
    long value = 0;
    long readCount = 0;

    /* Even though in our .ini file there is a read try timeout and attempts, we are doing a bit extra here, just to attempt
    to make sure the autopilot is initialized */
    while ((readCount < MAXREADATTEMPT) && (retVal != MP_OK))
    {
        ++readCount;
        printf("\n%d. Checking To See if Board Is Initialized", readCount);
        retVal = mpReadVar(planeId, MPFID_KOUNTER, &value, AUTO_MODE, QUICK_MODE);
        if (retVal != MP_OK)
        {
            printf(".......failed Error: %d", retVal);

        }
        else
        {
            printf(".......passed");
        }
    }

    if (retVal != MP_OK)
    {
        printf("\nFailed, Board is Not Initialized");
        return MP_BAD_VAR_ID;
    }
    else
    {
        printf("\nSuccess, Board is Initialized");
        return MP_OK;
    }
}

long FlyPlane(long planeId)
{
    long retVal = 0;
    long progressCount = 0;
    long timeIndex = 0;
    MPSTDTELEMETRYDATA pStdTlmData;
    int pitch = 0;
    int roll = 0;
	float curr_pitch;
	float curr_roll;
	float GPSN;
	float GPSE;
	int TIMET;
	float Heading;
	float Altitude;
    
    printf("\nFlying");
    do
    {
        retVal = mpFly(planeId);
        if (retVal != MP_OK)
        {
            printf("\nError : mpFly, retVal: %d", retVal);
            break;
        }

        /* becuase this is the simulator we have to include this additional step to get std telemetry */
        retVal = mpResponseStuffServos(planeId, 0, 0, 0, 0, &pitch, &roll, SIM_MODE, NORMAL_MODE);

        retVal = mpGetStandardTelemetry(planeId, &pStdTlmData);

        /*printf("\rStatus: %d Pitch: %d\tRoll: %d\tGPSN: %d GPSE: %d", pStdTlmData.status, pStdTlmData.tlmPitch, pStdTlmData.tlmRoll, pStdTlmData.n, pStdTlmData.e);*/
        
		curr_pitch = pStdTlmData.tlmPitch/17.87217;
		curr_roll = pStdTlmData.tlmRoll /17.87217;
		GPSN = pStdTlmData.n/8726646.26;
		GPSE = pStdTlmData.e/8726646.26;
		TIMET = timeIndex;
		Heading = pStdTlmData.hdg/100;
		Altitude = -pStdTlmData.alt/8; //IN FEET!!!

		
		printf("\nPitch: %.3f\  Roll: %.3f\  GPSN: %.6f\  GPSE: %.6f\  Heading: %.3f\  Altitude: %.3f\  Time:  %d" , curr_pitch, curr_roll, GPSN, GPSE, Heading, Altitude, TIMET );

		//printf("\rPitch: %d\Roll: %d\GPSN: %d GPSE: %d\tHeading: %d", pStdTlmData.tlmPitch, pStdTlmData.tlmRoll, pStdTlmData.n, pStdTlmData.e, (pStdTlmData.hdg/100));
		
		
		progressCount++;
        timeIndex++;
    } while (timeIndex < 200);    

    return retVal;
}
long ReadFromPlane(long planeId)
{
    long retVal = MP_ERROR;
    long value = 0;

    /* Do...while loop is used so we can break out of them if there is an error */
    do
    {
        //an example of how to use the mpQueueReadVar, mpReadVar, mpQueueGetVar
        //queue can hold a maximun of 15 values
        
        //mpQueueReadVar put the varId into the ques of what to read in this case the queue will be
        //queue: 0 - MPFID_MP2K_VERSION, 
        //       1 - MPFID_GPS_TIME, 
        //       2 - MPFID_MAIN_BATTERY_VOLT
        retVal = mpQueueReadVar(planeId, MPFID_MP2K_VERSION);
        retVal = mpQueueReadVar(planeId, MPFID_COMPASS_HEADING);
        retVal = mpQueueReadVar(planeId, MPFID_SERV_BATTERY_VOLT);

        //if the first mpQueueReadVar fails then the next 2 will So instead of checking
        //each individual mpQueueReadVar we check the last 1
        if (retVal != MP_OK)
        {
            printf("\nError : mpQueueGetVar, retVal: %d", retVal);
            break;
        }

        //this assigns values to the queue spot 0,1,2 according to which varId is associated with them
        retVal = mpReadVar(planeId, 0, NULL, AUTO_MODE, QUICK_MODE);

        //now we read those value from the queue, we read the queue in order 0,1,2
        for (int count = 0; count < 3 ; count++)
        {
            retVal = mpQueueGetVar(planeId, count, &value);
            if (retVal != MP_OK)
            {
                printf("\nError : mpQueueGetVar, retVal: %d", retVal);
            }
            else
            {
                printf("\nmpQueueGetVar: %d", value);
            }
        }

        // you can also use mpReadVar to read a single varible
        /* retVal = mpReadVar(planeId, MPFID_CURRENT_ALTITUDE, &value, AUTO_MODE, QUICK_MODE);
        if (retVal != MP_OK)
        {
            printf("\nError : mpReadVar %d, retVal: %d", value, retVal);
            break;
        }
        else
        {
            printf("\nmpReadVar: %d", MPFID_CURRENT_ALTITUDE);
        }
		 */

    } while (retVal == -MP_ERROR);
    return retVal;
}
long ReadCommandBuffer(long planeId)
{
    long retVal = MP_ERROR;
    long value = 0;
	int * p1 = 0;


    /* Do...while loop is used so we can break out of them if there is an error */
    do
    {

        retVal = mpReceiveVars(planeId, p1, SKIP_INVERT+SKIP_PROG);

			printf("\nCode: %d", *p1);
            break;


    } while (retVal == -MP_ERROR);
    return retVal;
}

long DeletePlane(long planeId)
{
    long retVal = 0;

     /* Do...while loop is used so we can break out of them if there is an error */
    do
    {
        /* This function will Close the Simulator, if it fails will still have other thing to do, so dont break */
        printf("\nClosing Simulator %d", planeId);
        retVal = mpCloseFly(planeId);
        if (retVal != MP_OK)
        {
            printf(".......Could Not Close Simulator");
        }
        else
        {
            printf(".......passed");
        }

        /* This function will Close the Link to the MP Autopilot, if it fails we still have to attempt to delete plane
           so dont break */
        /* We've already called this just before we start up the simulator, therefore there is no need to do is here */
        
        printf("\nClosing Link to Plane: %d", planeId);
        retVal = mpCloseLink(planeId);
        if (retVal != MP_OK)
        {
            printf(".......Could Not Close the Link to the MP Autopilot");
        }
        else
        {
            printf(".......passed");
        }
        

        /* This function will delete the uav */
        printf("\nDeleting Plane: %d", planeId);
        retVal = mpDelete(planeId);
        if (retVal != MP_OK)
        {
            printf(".......Could Not Delete Plane: %d", planeId);
        }
        else
        {
            printf(".......passed");
        }

    } while (retVal != MP_OK);

    return retVal;
}

int main(const int argc, const char *argv[])
{
long planeId = 1;
long mperr;
long retVal;
int key_in_1;
int exit_key = 0;
	do
	{
		printf("\n1 for Create, 2 for Initialize, 3 for Fly, 4 for Delete, 5 for autopilot check, 6 for exit, 7 for readvalue, 8 for read comm buffer");
		key_in_1 = getchar();
		switch( key_in_1 )
			 {
				case '1' : 
					mperr = CreatePlane(planeId);
									if (mperr != MP_OK)
										{
											printf("Error");
										}
					break;
				case '2' : mperr = InitializeFlyPlane(planeId);
														if (mperr != MP_OK)
										{
											printf("Error");
										}
					break;
				case '3' : mperr = FlyPlane(planeId);
														if (mperr != MP_OK)
										{
											printf("Error");
										}
					break;
				case '4' : mperr = DeletePlane(planeId);
														if (mperr != MP_OK)
										{
											printf("Error");
										}
					break;
				case '5' : mperr = CheckInitialize(planeId);
														if (mperr != MP_OK)
										{
											printf("Error");
										}
					break;
				case '6' : exit_key = 1;
					break;
				case '7' : mperr = ReadFromPlane(planeId);
														if (mperr != MP_OK)
										{
											printf("Error");
										}
					break;
				case '8' : mperr = ReadCommandBuffer(planeId);
														if (mperr != MP_OK)
										{
											printf("Error");
										}
					break;
			 }
	} while (exit_key !=1);

return mperr;
}