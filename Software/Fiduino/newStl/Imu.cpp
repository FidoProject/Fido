#include "Arduino.h"
#include "Wire.h"
#include "Imu.h"

#define MPU9150_SELF_TEST_X        0x0D
#define MPU9150_SELF_TEST_Y        0x0E
#define MPU9150_SELF_TEST_X        0x0F
#define MPU9150_SELF_TEST_A        0x10
#define MPU9150_SMPLRT_DIV         0x19
#define MPU9150_CONFIG             0x1A
#define MPU9150_GYRO_CONFIG        0x1B
#define MPU9150_ACCEL_CONFIG       0x1C
#define MPU9150_FF_THR             0x1D
#define MPU9150_FF_DUR             0x1E
#define MPU9150_MOT_THR            0x1F
#define MPU9150_MOT_DUR            0x20
#define MPU9150_ZRMOT_THR          0x21
#define MPU9150_ZRMOT_DUR          0x22
#define MPU9150_FIFO_EN            0x23
#define MPU9150_I2C_MST_CTRL       0x24
#define MPU9150_I2C_SLV0_ADDR      0x25
#define MPU9150_I2C_SLV0_REG       0x26
#define MPU9150_I2C_SLV0_CTRL      0x27
#define MPU9150_I2C_SLV1_ADDR      0x28
#define MPU9150_I2C_SLV1_REG       0x29
#define MPU9150_I2C_SLV1_CTRL      0x2A
#define MPU9150_I2C_SLV2_ADDR      0x2B
#define MPU9150_I2C_SLV2_REG       0x2C
#define MPU9150_I2C_SLV2_CTRL      0x2D
#define MPU9150_I2C_SLV3_ADDR      0x2E
#define MPU9150_I2C_SLV3_REG       0x2F
#define MPU9150_I2C_SLV3_CTRL      0x30
#define MPU9150_I2C_SLV4_ADDR      0x31
#define MPU9150_I2C_SLV4_REG       0x32
#define MPU9150_I2C_SLV4_DO        0x33
#define MPU9150_I2C_SLV4_CTRL      0x34
#define MPU9150_I2C_SLV4_DI        0x35
#define MPU9150_I2C_MST_STATUS     0x36
#define MPU9150_INT_PIN_CFG        0x37
#define MPU9150_INT_ENABLE         0x38
#define MPU9150_INT_STATUS         0x3A
#define MPU9150_ACCEL_XOUT_H       0x3B
#define MPU9150_ACCEL_XOUT_L       0x3C
#define MPU9150_ACCEL_YOUT_H       0x3D
#define MPU9150_ACCEL_YOUT_L       0x3E
#define MPU9150_ACCEL_ZOUT_H       0x3F
#define MPU9150_ACCEL_ZOUT_L       0x40
#define MPU9150_TEMP_OUT_H         0x41
#define MPU9150_TEMP_OUT_L         0x42
#define MPU9150_GYRO_XOUT_H        0x43
#define MPU9150_GYRO_XOUT_L        0x44
#define MPU9150_GYRO_YOUT_H        0x45
#define MPU9150_GYRO_YOUT_L        0x46
#define MPU9150_GYRO_ZOUT_H        0x47
#define MPU9150_GYRO_ZOUT_L        0x48
#define MPU9150_EXT_SENS_DATA_00   0x49
#define MPU9150_EXT_SENS_DATA_01   0x4A
#define MPU9150_EXT_SENS_DATA_02   0x4B
#define MPU9150_EXT_SENS_DATA_03   0x4C
#define MPU9150_EXT_SENS_DATA_04   0x4D
#define MPU9150_EXT_SENS_DATA_05   0x4E
#define MPU9150_EXT_SENS_DATA_06   0x4F
#define MPU9150_EXT_SENS_DATA_07   0x50
#define MPU9150_EXT_SENS_DATA_08   0x51
#define MPU9150_EXT_SENS_DATA_09   0x52
#define MPU9150_EXT_SENS_DATA_10   0x53
#define MPU9150_EXT_SENS_DATA_11   0x54
#define MPU9150_EXT_SENS_DATA_12   0x55
#define MPU9150_EXT_SENS_DATA_13   0x56
#define MPU9150_EXT_SENS_DATA_14   0x57
#define MPU9150_EXT_SENS_DATA_15   0x58
#define MPU9150_EXT_SENS_DATA_16   0x59
#define MPU9150_EXT_SENS_DATA_17   0x5A
#define MPU9150_EXT_SENS_DATA_18   0x5B
#define MPU9150_EXT_SENS_DATA_19   0x5C
#define MPU9150_EXT_SENS_DATA_20   0x5D
#define MPU9150_EXT_SENS_DATA_21   0x5E
#define MPU9150_EXT_SENS_DATA_22   0x5F
#define MPU9150_EXT_SENS_DATA_23   0x60
#define MPU9150_MOT_DETECT_STATUS  0x61
#define MPU9150_I2C_SLV0_DO        0x63
#define MPU9150_I2C_SLV1_DO        0x64
#define MPU9150_I2C_SLV2_DO        0x65
#define MPU9150_I2C_SLV3_DO        0x66
#define MPU9150_I2C_MST_DELAY_CTRL 0x67
#define MPU9150_SIGNAL_PATH_RESET  0x68
#define MPU9150_MOT_DETECT_CTRL    0x69
#define MPU9150_USER_CTRL          0x6A
#define MPU9150_PWR_MGMT_1         0x6B
#define MPU9150_PWR_MGMT_2         0x6C
#define MPU9150_FIFO_COUNTH        0x72
#define MPU9150_FIFO_COUNTL        0x73
#define MPU9150_FIFO_R_W           0x74
#define MPU9150_WHO_AM_I           0x75
#define MPU9150_CMPS_XOUT_L        0x4A
#define MPU9150_CMPS_XOUT_H        0x4B
#define MPU9150_CMPS_YOUT_L        0x4C
#define MPU9150_CMPS_YOUT_H        0x4D
#define MPU9150_CMPS_ZOUT_L        0x4E
#define MPU9150_CMPS_ZOUT_H        0x4F

Imu::Imu() {
	Wire.begin();
	writeSensor(MPU9150_PWR_MGMT_1, 0);
	setupCompass();
}

Imu::void setupCompass(){
	MPU9150_I2C_ADDRESS = 0x0C;     

	writeSensor(0x0A, 0x00);
	writeSensor(0x0A, 0x0F);
	writeSensor(0x0A, 0x00);

	MPU9150_I2C_ADDRESS = 0x69;     

	writeSensor(0x24, 0x40);
	writeSensor(0x25, 0x8C);
	writeSensor(0x26, 0x02);
	writeSensor(0x27, 0x88);
	writeSensor(0x28, 0x0C);
	writeSensor(0x29, 0x0A);
	writeSensor(0x2A, 0x81);
	writeSensor(0x64, 0x01);
	writeSensor(0x67, 0x03);
	writeSensor(0x01, 0x80);

	writeSensor(0x34, 0x04);
	writeSensor(0x64, 0x00);
	writeSensor(0x6A, 0x00);
	writeSensor(0x64, 0x01);
	writeSensor(0x6A, 0x20);
	writeSensor(0x34, 0x13);
}

Imu::int readSensor(int addrL, int addrH) {
	Wire.beginTransmission(MPU9150_I2C_ADDRESS);
	Wire.write(addrL);
	Wire.endTransmission(false);

	Wire.requestFrom(MPU9150_I2C_ADDRESS, 1, true);
	byte L = Wire.read();

	Wire.beginTransmission(MPU9150_I2C_ADDRESS);
	Wire.write(addrH);
	Wire.endTransmission(false);

	Wire.requestFrom(MPU9150_I2C_ADDRESS, 1, true);
	byte H = Wire.read();

	return (int16_t)((H<<8)+L);
}

Imu::int readSensor(int addr) {
	Wire.beginTransmission(MPU9150_I2C_ADDRESS);
	Wire.write(addr);
	Wire.endTransmission(false);

	Wire.requestFrom(MPU9150_I2C_ADDRESS, 1, true);
	return Wire.read();
}

Imu::void writeSensor(int addr,int data) {
	Wire.beginTransmission(MPU9150_I2C_ADDRESS);
	Wire.write(addr);
	Wire.write(data);
	Wire.endTransmission(true);
}