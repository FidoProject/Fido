#ifndef NEWVECTOR_H
#define NEWVECTOR_H

namespace io {
	struct TDVect {
        // Components of 3d vector
        double xComp,yComp,zComp;
        
        // Get 3d vector as radius, xy angle, and z angle.
        void getRTP(double& r,double& xy,double& z) {
            r = sqrt(pow(xComp,2)+pow(yComp,2)+pow(zComp,2));
            xy = atan2(xComp,yComp);
            z = acos(zComp/r);
        }
    };

	class Imu {
	public:
		// Initialize and construct IMU
		Imu();

		// Return 3d magnetic field vector
		TDVect getCompass();

		// Return 3d rotational velocity vector
		TDVect getGyro();

		// Return 3d acceleration vector
		TDVect getAccel();

		// Get ambient temperature (0-100)
		int getTemp();
	private:
		void setupCompass();
		int readSensor(int addrL, int addrH);
		int readSensor(int addr);
		void writeSensor(int addr,int data)
	};
};

#endif