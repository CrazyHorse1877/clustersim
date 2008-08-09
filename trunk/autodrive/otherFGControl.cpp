#include <windows.h>
#include <time.h>

#include <iostream>
using namespace std;

const int FG_NET_FDM_VERSION = 13;


// Define a structure containing the top level flight dynamics model
// parameters

class FGNetFDM {

public:

    int version;		// increment when data values change
    int pad;                    // keep doubles 64-bit aligned for some
                                // hardware platforms, such as the Sun
                                // SPARC, which don't like misaligned
                                // data

    enum {
        FG_MAX_ENGINES = 4,
        FG_MAX_WHEELS = 3,
        FG_MAX_TANKS = 4
    };

    // Positions
    double longitude;		// geodetic (radians)
    double latitude;		// geodetic (radians)
    double altitude;		// above sea level (meters)
    float agl;			// above ground level (meters)
    float phi;			// roll (radians)
    float theta;		// pitch (radians)
    float psi;			// yaw or true heading (radians)

    // Velocities
    float phidot;		// roll rate (radians/sec)
    float thetadot;		// pitch rate (radians/sec)
    float psidot;		// yaw rate (radians/sec)
    float vcas;		        // calibrated airspeed
    float climb_rate;		// feet per second
    float v_north;              // north velocity in local/body frame, fps
    float v_east;               // east velocity in local/body frame, fps
    float v_down;               // down/vertical velocity in local/body frame, fps
    float v_wind_body_north;    // north velocity in local/body frame
                                // relative to local airmass, fps
    float v_wind_body_east;     // east velocity in local/body frame
                                // relative to local airmass, fps
    float v_wind_body_down;     // down/vertical velocity in local/body
                                // frame relative to local airmass, fps

    // Stall
    float stall_warning;        // 0.0 - 1.0 indicating the amount of stall

    // Accelerations
    float A_X_pilot;		// X accel in body frame ft/sec^2
    float A_Y_pilot;		// Y accel in body frame ft/sec^2
    float A_Z_pilot;		// Z accel in body frame ft/sec^2

    // Pressure
    
    // Engine status
    int num_engines;		// Number of valid engines
    int eng_state[FG_MAX_ENGINES]; // Engine state (off, cranking, running)
    float rpm[FG_MAX_ENGINES];	// Engine RPM rev/min
    float fuel_flow[FG_MAX_ENGINES]; // Fuel flow gallons/hr
    float EGT[FG_MAX_ENGINES];	// Exhuast gas temp deg F
    float oil_temp[FG_MAX_ENGINES]; // Oil temp deg F
    float oil_px[FG_MAX_ENGINES]; // Oil pressure psi

    // Consumables
    int num_tanks;		// Max number of fuel tanks
    float fuel_quantity[FG_MAX_TANKS];

    // Gear status
    int num_wheels;
    bool wow[FG_MAX_WHEELS];
    float gear_pos[FG_MAX_WHEELS];
    float gear_steer[FG_MAX_WHEELS];
    float gear_compression[FG_MAX_WHEELS];

    // Environment
    time_t cur_time;            // current unix time
    long int warp;              // offset in seconds to unix time
    float visibility;           // visibility in meters (for env. effects)

    // Control surface positions (normalized values)
    float elevator;
    float flaps;
    float left_aileron;
    float right_aileron;
    float rudder;
    float speedbrake;
    float spoilers;
};

bool initialized = false;
SOCKET sendSocket = -1;
struct sockaddr_in sendAddr;

void run();

int main(int argc, char ** argv)
{
    WSAData wd;
    if (WSAStartup(MAKEWORD(2,0),&wd) == 0)
    {
        memset(&sendAddr,0,sizeof(sendAddr));
        sendAddr.sin_family = AF_INET;
        sendAddr.sin_port = htons(5500);
        sendAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

        sendSocket = socket(AF_INET,SOCK_DGRAM,0);
        if (sendSocket != INVALID_SOCKET)
        {
            run();
        }
    }

    return 0;
}

#define D2R (3.14159 / 180.0)

void run()
{
    double latitude = 45.59823;
    double longitude = -120.69202;
    double altitude = 150.0;
    double roll = 0.0;
    double pitch = 0.0;
    double yaw = 0.0;


    while (true)
    {
        Sleep(1000);

        FGNetFDM fdm;
        memset(&fdm,0,sizeof(fdm));
        fdm.version = FG_NET_FDM_VERSION;
        fdm.latitude = latitude * D2R;
        fdm.longitude = longitude * D2R;
        fdm.altitude = altitude;
        fdm.agl = 50;
        fdm.phi = roll;
        fdm.theta = pitch;
        fdm.psi = yaw;

        fdm.num_tanks = 1;
        fdm.fuel_quantity[0] = 100.0;
        fdm.cur_time = time(0);

        int nbytes = sendto(sendSocket,(char *)&fdm,sizeof(fdm),0,(struct sockaddr *)&sendAddr,sizeof(sendAddr));
        if (nbytes == sizeof(fdm))
        {
            cout << "sent telemetry to FlightGear" << endl;
        }
    }
}
