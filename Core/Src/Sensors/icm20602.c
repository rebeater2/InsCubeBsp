#include "main.h"
#include "icm20602.h"
#include "cmsis_os.h"

//加速度量程
#define ICM20_ACCEL_FS_2G            (0<<3)
#define ICM20_ACCEL_FS_4G            (1<<3)
#define ICM20_ACCEL_FS_8G            (2<<3)
#define ICM20_ACCEL_FS_16G            (3<<3)
//角速度量程
#define ICM20_GYRO_FS_250            (0<<3)
#define ICM20_GYRO_FS_500            (1<<3)
#define ICM20_GYRO_FS_1000            (2<<3)
#define ICM20_GYRO_FS_2000            (3<<3)

//CONFIG DPF
#define DLPF_BW_250            0x00    //Rate=8k
#define DLPF_BW_176            0x01
#define DLPF_BW_92            0x02
#define DLPF_BW_41            0x03
#define DLPF_BW_20            0x04
#define DLPF_BW_10            0x05
#define DLPF_BW_5            0x06
#define DLPF_BW_328            0x06    //Rate=8k
//ACCEL_CONFIG2
#define ACCEL_AVER_4            (0x00<<4)    //Rate=8k
#define ACCEL_AVER_8            (0x01<<4)
#define ACCEL_AVER_16            (0x02<<4)
#define ACCEL_AVER_32            (0x03<<4)
//ACCEL_DLPF
#define ACCEL_DLPF_BW_218            0x00
//#define ACCEL_DLPF_BW_218         	0x01
#define ACCEL_DLPF_BW_99            0x02
#define ACCEL_DLPF_BW_44            0x03
#define ACCEL_DLPF_BW_21            0x04
#define ACCEL_DLPF_BW_10            0x05
#define ACCEL_DLPF_BW_5            0x06
#define ACCEL_DLPF_BW_420           0x06

#define    FSYNC_INT_MODE_EN        (1<<2) //in ICM20_INT_PIN_CFG reg(0x37)
/*ICM20_INT_ENABLE*/
#define    FIFO_OFLOW_EN                (1<<4) //in ICM20_INT_ENABLE reg(0x38)
#define    FSYNC_INT_EN                 (1<<3) //in ICM20_INT_ENABLE reg(0x38)
#define    GDRIVE_INT_EN                (1<<2) //in ICM20_INT_ENABLE reg(0x38)
#define    DATA_RDY_INT_EN              (1<<0) //in ICM20_INT_ENABLE reg(0x38)
#define    GYRO_FIFO_EN                 (1<<4) //in ICM20_FIFO_EN reg(0x23)
#define    ACCEL_FIFO_EN                (1<<3) //in ICM20_FIFO_EN reg(0x23)
#define    FIFO_EN                      (1<<6) //in ICM20_USER_CTRL(0x6A)
#define    FIFO_RST                    (1<<2) // in ICM20_USER_CTRL(0x6A)

/*---------------------------------------------------------------------------------*/
//ICM20602_INT_PIN_CFG
#define INT_LEVEL_H                        (1<<7)
#define INT_LEVEL_L                        (0<<7)//0 ?C The logic level for INT/DRDY pin is active high.
#define INT_OPEN_H                        (1<<6)//1 ?C INT/DRDY pin is configured as open drain.
#define INT_OPEN_L                        (0<<6)
#define LATCH_INT_EN_H                (1<<5)//1 ?C INT/DRDY pin level held until interrupt status is cleared.
#define LATCH_INT_EN_L                (0<<5)//0 - more robust
#define INT_RD_CLEAR_H                (1<<4)
#define INT_RD_CLEAR_L                (0<<4)//0 ?C Interrupt status is cleared only by reading INT_STATUS register
#define FSYNC_INT_LEVEL_H            (1<<3)//0 ?C The logic level for the FSYNC pin as an interrupt is active high.
#define FSYNC_INT_LEVEL_L            (0<<3)

extern SPI_HandleTypeDef hspi1;

float _accel_scale;
float _gyro_scale;

volatile uint32_t icm20602_id = 0; /*通过外部改变此变量在四个IMU之间切换*/
void SPI1_CS_DOWN(uint32_t id) {
  switch (id) {
	case 0:HAL_GPIO_WritePin(SPI1_CS_0_GPIO_Port, SPI1_CS_0_Pin, GPIO_PIN_RESET);
	  break;
	case 1:HAL_GPIO_WritePin(SPI1_CS_1_GPIO_Port, SPI1_CS_1_Pin, GPIO_PIN_RESET);
	  break;
	case 2:HAL_GPIO_WritePin(SPI1_CS_2_GPIO_Port, SPI1_CS_2_Pin, GPIO_PIN_RESET);
	  break;
	case 3:HAL_GPIO_WritePin(SPI1_CS_3_GPIO_Port, SPI1_CS_3_Pin, GPIO_PIN_RESET);
	  break;
	default:break;
  }
}
void SPI1_CS_UP(uint32_t id) {
  switch (icm20602_id) {
	case 0:HAL_GPIO_WritePin(SPI1_CS_0_GPIO_Port, SPI1_CS_0_Pin, GPIO_PIN_SET);
	  break;
	case 1:HAL_GPIO_WritePin(SPI1_CS_1_GPIO_Port, SPI1_CS_1_Pin, GPIO_PIN_SET);
	  break;
	case 2:HAL_GPIO_WritePin(SPI1_CS_2_GPIO_Port, SPI1_CS_2_Pin, GPIO_PIN_SET);
	  break;
	case 3:HAL_GPIO_WritePin(SPI1_CS_3_GPIO_Port, SPI1_CS_3_Pin, GPIO_PIN_SET);
	  break;
	default:break;
  }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602 SPI写寄存器
//  @param      cmd     寄存器地址
//  @param      val     需要写入的数据
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void icm20602_spi_write_reg(uint8_t cmd, uint8_t val, uint32_t id) {
  uint8_t dat[2];
  SPI1_CS_DOWN(id);

  dat[0] = cmd | ICM20602_SPI_W;
  dat[1] = val;
  HAL_SPI_Transmit(&hspi1, dat, 2, 0xFFFF);

  SPI1_CS_UP(id);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602 根据量程设置比例换算关系
//  @param      fs      量程
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void icm_set_gyro_fullscale(uint8_t fs, uint32_t id) {
  switch (fs) {
	case ICM20_GYRO_FS_250:_gyro_scale = 1.0f / 131.068f;    //32767/250
	  break;
	case ICM20_GYRO_FS_500:_gyro_scale = 1.0f / 65.534f;
	  break;
	case ICM20_GYRO_FS_1000:_gyro_scale = 1.0f / 32.767f;
	  break;
	case ICM20_GYRO_FS_2000:_gyro_scale = 1.0f / 16.3835f;
	  break;
	default:fs = ICM20_GYRO_FS_2000;
	  _gyro_scale = 1.0f / 16.3835f;
	  break;
  }
  icm20602_spi_write_reg(ICM20602_GYRO_CONFIG, fs, id);
}

void icm_set_accel_fullscale(uint8_t fs, uint32_t id) {
  switch (fs) {
	case ICM20_ACCEL_FS_2G:_accel_scale = 1.0f / 16384.0f;
	  break;
	case ICM20_ACCEL_FS_4G:_accel_scale = 1.0f / 8192.0f;
	  break;
	case ICM20_ACCEL_FS_8G:_accel_scale = 1.0f / 4096.0f;
	  break;
	case ICM20_ACCEL_FS_16G:_accel_scale = 1.0f / 2048.0f;
	  break;
	default:fs = ICM20_ACCEL_FS_8G;
	  _accel_scale = 1.0f / 4096.0f;
	  break;

  }
  icm20602_spi_write_reg(ICM20602_ACCEL_CONFIG, fs, id);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602 SPI读寄存器
//  @param      cmd     寄存器地址
//  @param      *val    接收数据的地址
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------

void icm20602_spi_read_reg(uint8_t cmd, uint8_t *val, uint32_t id) {
  uint8_t dat[2];

  SPI1_CS_DOWN(id);
  dat[0] = cmd | ICM20602_SPI_R;
  dat[1] = *val;
  HAL_SPI_TransmitReceive(&hspi1, dat, dat, 2, 0xFFFF);

  SPI1_CS_UP(id);

  *val = dat[1];
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602 SPI多字节读寄存器
//  @param      cmd     寄存器地址
//  @param      *val    接收数据的地址
//  @param      num     读取数量
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
/*void icm20602_spi_read_regs(uint8_t *val, uint8_t num, uint32_t id) {
  SPI1_CS_DOWN(id);
  HAL_SPI_TransmitReceive(&hspi1, val, val, num, 0xffff);
  SPI1_CS_UP(id);
}*/
void icm20602_spi_read_regs(uint8_t *val, uint8_t num, uint32_t id) {
//void icm20602_spi_read_regs(uint8_t *reg, uint8_t *buff, int len, uint32_t id) {
  SPI1_CS_DOWN(id);
//  HAL_SPI_TransmitReceive(&hspi1, reg, buff, len, 0xff);
  HAL_SPI_TransmitReceive(&hspi1, val, val, num, 500);
  SPI1_CS_UP(id);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602自检函数
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void icm20602_self3_check(uint32_t id) {
  uint8_t dat = 0;
  while (0x12 != dat)   //读取ICM20602 ID
  {
	icm20602_spi_read_reg(ICM20602_WHO_AM_I, &dat, id);
	delay(10);
//卡在这里原因有以下几点
//1 ICM20602坏了，如果是新的这样的概率极低
//2 接线错误或者没有接好
//3 可能你需要外接上拉电阻，上拉到3.3V
  }

}

void icm20602_enable_fifo(uint32_t id) {
  delay(50);
  icm20602_spi_write_reg(ICM20602_FIFO_EN, GYRO_FIFO_EN | ACCEL_FIFO_EN, id);
  delay(50);
//  icm20602_spi_write_reg(ICM20602_FIFO_WM_TH1,0x00,id);
//  delay_ms(50);
//  icm20602_spi_write_reg(ICM20602_FIFO_WM_TH2,14,id);
//  delay_ms(50);
  icm20602_spi_write_reg(ICM20602_USER_CTRL, FIFO_EN, id);

}
/**
 * INT_LEVEL  	1 – The logic level for INT/DRDY pin is active low.
 *				0 – The logic level for INT/DRDY pin is active high.
 */
void icm20602_enable_interrupt(uint32_t id) {
  delay(50);
  icm20602_spi_write_reg(ICM20602_INT_ENABLE, DATA_RDY_INT_EN, id);
  delay(50);
  icm20602_spi_write_reg(ICM20602_INT_PIN_CFG, INT_LEVEL_L | INT_OPEN_L | LATCH_INT_EN_L | INT_RD_CLEAR_H, id);
  delay(50);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      初始化ICM20602
//  @param      mode: 0 small scale , 1 big scale
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void icm20602_init_spi(IMU_MODE mode, IMU_RATE rate, uint32_t id) {
  uint8_t val = 0x0;

  icm20602_self3_check(id);//检测

  icm20602_spi_write_reg(ICM20602_PWR_MGMT_1, 0x80, id);//复位设备
  delay(50);
  do {   //等待复位成功
	icm20602_spi_read_reg(ICM20602_PWR_MGMT_1, &val, id);
  } while (0x41 != val);
  delay(50);
  icm20602_spi_write_reg(ICM20602_PWR_MGMT_1, 0x01, id);            //使能温度传感器
  delay(50);
  icm20602_spi_write_reg(ICM20602_PWR_MGMT_2, 0x00, id);            //开启陀螺仪和加速度计[8:0]0000 0000
  delay(50);
  //采样速率 SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
  /*200 Hz*/
  switch (rate) {
	case IMU_RATE_200Hz:icm20602_spi_write_reg(ICM20602_SMPLRT_DIV, 4, id);/*1000/(4+1)*/
	  break;
	case IMU_RATE_100Hz:icm20602_spi_write_reg(ICM20602_SMPLRT_DIV, 9, id);
	  break;
	case IMU_RATE_50Hz:icm20602_spi_write_reg(ICM20602_SMPLRT_DIV, 19, id);
	  break;
	case IMU_RATE_10Hz:icm20602_spi_write_reg(ICM20602_SMPLRT_DIV, 99, id);
	  break;
	case IMU_RATE_1k:icm20602_spi_write_reg(ICM20602_SMPLRT_DIV, 0, id);
	  break;
	default:icm20602_spi_write_reg(ICM20602_SMPLRT_DIV, 4, id);
	  break;
  }
  delay(50);
  icm20602_spi_write_reg(ICM20602_CONFIG, DLPF_BW_328, id);            //176HZ 1KHZ
  delay(50);
  if (mode == IMU_MODE_CAR) {
	icm_set_gyro_fullscale(ICM20_GYRO_FS_250, id);
	icm_set_accel_fullscale(ICM20_ACCEL_FS_2G, id);
  } else {/*脚载需要更大量程*/
	icm_set_gyro_fullscale(ICM20_GYRO_FS_1000, id);
	icm_set_accel_fullscale(ICM20_ACCEL_FS_8G, id);
  }
  delay(50);
  icm20602_spi_write_reg(ICM20602_ACCEL_CONFIG_2,
						 ACCEL_AVER_4 | ACCEL_DLPF_BW_10,
						 id);            //Average 4 samples   44.8HZ   //0x23 Average 16 samples
  delay(50);
  icm20602_enable_fifo(id);
  icm20602_enable_interrupt(id);
  //    uint8_t temp, temp1;
//    icm20602_spi_write_reg(ICM20602_XG_OFFS_USRH, 0x00);
//    icm20602_spi_write_reg(ICM20602_XG_OFFS_USRL, 0x00);
//    icm20602_spi_write_reg(ICM20602_YG_OFFS_USRH, 0x00);
//    icm20602_spi_write_reg(ICM20602_YG_OFFS_USRL, 0x00);
//    icm20602_spi_write_reg(ICM20602_ZG_OFFS_USRH, 0x00);
//    icm20602_spi_write_reg(ICM20602_ZG_OFFS_USRL, 0x00);
//    icm20602_spi_write_reg(ICM20602_XA_OFFSET_H,0x00);
//    icm20602_spi_write_reg(ICM20602_XA_OFFSET_L,0x00);
//    icm20602_spi_write_reg(ICM20602_XA_OFFSET_H,0x00);
//    icm20602_spi_write_reg(ICM20602_YA_OFFSET_H,0x00);
//    icm20602_spi_write_reg(ICM20602_YA_OFFSET_L,0x00);
//    icm20602_spi_write_reg(ICM20602_ZA_OFFSET_H,0x00);
//    icm20602_spi_write_reg(ICM20602_ZA_OFFSET_L,0x00);
//    icm20602_spi_write_reg(,0x00);
//    printf("self test\n");
//    icm20602_spi_read_reg(ICM20602_SELF_TEST_X_ACCEL, &temp);
//    printf("acce_x  %x\n", temp);
//    icm20602_spi_read_reg(ICM20602_SELF_TEST_Y_ACCEL, &temp);
//    printf("acce_y  %x\n", temp);
//    icm20602_spi_read_reg(ICM20602_SELF_TEST_Z_ACCEL, &temp);
//    printf("acce_z  %x\n", temp);
//    icm20602_spi_read_reg(ICM20602_SELF_TEST_X_GYRO, &temp);
//    printf("gyro_x  %x\n", temp);
//    icm20602_spi_read_reg(ICM20602_SELF_TEST_Y_GYRO, &temp);
//    printf("gyro_y %x\n", temp);
//    icm20602_spi_read_reg(ICM20602_SELF_TEST_Z_GYRO, &temp);
//    printf("gyro_z  %x\n", temp);
//    printf("bias test\n");
//    icm20602_spi_read_reg(ICM20602_XG_OFFS_USRH, &temp);
//    icm20602_spi_read_reg(ICM20602_XG_OFFS_USRL, &temp1);
//    printf("gyro_x  %x %x\n", temp, temp1);
//    icm20602_spi_read_reg(ICM20602_YG_OFFS_USRH, &temp);
//    icm20602_spi_read_reg(ICM20602_YG_OFFS_USRL, &temp1);
//    printf("gyro_y   %x %x\n", temp, temp1);
//    icm20602_spi_read_reg(ICM20602_ZG_OFFS_USRH, &temp);
//    icm20602_spi_read_reg(ICM20602_ZG_OFFS_USRL, &temp1);
//    printf("gyro_z   %x %x\n", temp, temp1);
//    icm20602_spi_read_reg(ICM20602_XA_OFFSET_H, &temp);
//    icm20602_spi_read_reg(ICM20602_XA_OFFSET_L, &temp1);
//    printf("acce_x   %x %x\n", temp, temp1);
//    icm20602_spi_read_reg(ICM20602_YA_OFFSET_H, &temp);
//    icm20602_spi_read_reg(ICM20602_YA_OFFSET_L, &temp1);
//    printf("acce_y  %x %x\n", temp, temp1);
//    icm20602_spi_read_reg(ICM20602_ZA_OFFSET_H, &temp);
//    icm20602_spi_read_reg(ICM20602_ZA_OFFSET_L, &temp1);
//    printf("acce_z   %x %x\n", temp, temp1);

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取ICM20602加速度计数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
/*void get_icm20602_accdata_spi(uint32_t id) {
  struct {
	uint8_t reg;
	uint8_t dat[6];
  } buf;

  buf.reg = ICM20602_ACCEL_XOUT_H | ICM20602_SPI_R;

  icm20602_spi_read_regs(&buf.reg, 7, id);
  icm_acc_x = ((((uint16_t)buf.dat[0]) << 8) | buf.dat[1]);
  icm_acc_y = ((((uint16_t)buf.dat[2]) << 8) | buf.dat[3]);
  icm_acc_z = ((((uint16_t)buf.dat[4]) << 8) | buf.dat[5]);
}*/

#if 0
/**
 * read imu data from fifo
 * @param data [0~2]:gyro  [3:5]:acce
 * @return 0
 */
uint16_t icm20602_read_fifo(int16_t data[6]) {
	struct {
		uint8_t reg;
		uint8_t dat[14];/* 59~72*/
	} buf;
	uint8_t dl, dh;
	icm20602_spi_read_reg(ICM20602_FIFO_COUNTL, &dl);
	icm20602_spi_read_reg(ICM20602_FIFO_COUNTH, &dh);
	uint16_t fifo_length = ((uint16_t) dh) << 8u | dl;
	buf.reg = ICM20602_FIFO_R_W | ICM20602_SPI_R;
/*    if(fifo_length > 14){
		for(int i = fifo_length;i>=14;i--)
			icm20602_spi_read_reg(ICM20602_FIFO_R_W,&dh);
	}*/
	icm20602_spi_read_regs(&buf.reg, 15);
	data[3] = ((uint16_t) buf.dat[0] << 8u) | buf.dat[1];
	data[4] = ((uint16_t) buf.dat[2] << 8u) | buf.dat[3];
	data[5] = ((uint16_t) buf.dat[4] << 8u) | buf.dat[5];
	data[0] = ((uint16_t) buf.dat[8] << 8u) | buf.dat[9];
	data[1] = ((uint16_t) buf.dat[10] << 8u) | buf.dat[11];
	data[2] = ((uint16_t) buf.dat[12] << 8u) | buf.dat[13];
	return fifo_length;
}
#endif

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取ICM20602陀螺仪数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
/*void get_icm20602_gyro_spi(int id) {
  struct {
	uint8_t reg;
	uint8_t dat[6];
  } buf;

  buf.reg = ICM20602_GYRO_XOUT_H | ICM20602_SPI_R;

  icm20602_spi_read_regs(&buf.reg, 7, id);
  icm_gyro_x = (int16_t)(((uint16_t)buf.dat[0] << 8 | buf.dat[1]));
  icm_gyro_y = (int16_t)(((uint16_t)buf.dat[2] << 8 | buf.dat[3]));
  icm_gyro_z = (int16_t)(((uint16_t)buf.dat[4] << 8 | buf.dat[5]));
}*/

uint8_t icm20602_status(uint32_t id) {
  uint8_t retVal = 0;
  uint8_t temp;
  icm20602_spi_read_reg(ICM20602_FSYNC_INT, &temp, id);
  retVal |= temp;
  icm20602_spi_read_reg(ICM20602_INT_STATUS, &temp, id);
  retVal |= temp;
  return retVal;
}

/**
 * read imu data from fifo
 * @param data [0~2]:gyro  [3:5]:acce
 * @return 0
 */
uint16_t icm20602_read_fifo(ImuRawIcm *icm, uint32_t id) {
  int16_t *data = icm->acce;
  struct {
	uint8_t reg;
	uint8_t dat[14];/* 59~72*/
  } buf;
/*  uint8_t dl, dh;
  icm20602_spi_read_reg(ICM20602_FIFO_COUNTL, &dl,id);
  icm20602_spi_read_reg(ICM20602_FIFO_COUNTH, &dh,id);
  uint16_t fifo_length = ((uint16_t) dh) << 8u | dl;*/
  buf.reg = ICM20602_FIFO_R_W | ICM20602_SPI_R;
  /*    if(fifo_length > 14){
		  for(int i = fifo_length;i>=14;i--)
			  icm20602_spi_read_reg(ICM20602_FIFO_R_W,&dh);
	  }*/
/*  SPI1_CS_DOWN(id);
  HAL_SPI_Transmit(&hspi1, (uint8_t *) &buf.reg, 1, 0xfff);
  HAL_SPI_Receive(&hspi1, (uint8_t *) icm->acce, 14, 0xfff);
  SPI1_CS_UP(id);*/
  icm20602_spi_read_regs(&buf.reg, 15, id);
//  memcpy(icm->acce, buf.dat, 14);
  icm->acce[0] = (((uint16_t)buf.dat[0]) << 8u) | buf.dat[1];
  icm->acce[1] = ((uint16_t)buf.dat[2] << 8u) | buf.dat[3];
  icm->acce[2] = ((uint16_t)buf.dat[4] << 8u) | buf.dat[5];
  icm->temp = ((uint16_t)buf.dat[6] << 8u) | buf.dat[7];
  icm->gyro[0] = ((uint16_t)buf.dat[8] << 8u) | buf.dat[9];
  icm->gyro[1] = ((uint16_t)buf.dat[10] << 8u) | buf.dat[11];
  icm->gyro[2] = ((uint16_t)buf.dat[12] << 8u) | buf.dat[13];
//  uint8_t dl, dh;
////  icm20602_spi_read_reg(ICM20602_FIFO_COUNTL, &dl, id);
////  icm20602_spi_read_reg(ICM20602_FIFO_COUNTH, &dh, id);
//  uint16_t fifo_length  =  14; //((uint16_t)dh) << 8u | dl;
//
//  uint8_t reg = ICM20602_FIFO_R_W | ICM20602_SPI_R;
//  uint8_t w_buff[15];
//  w_buff[0] = reg;
//  uint8_t *data = (uint8_t *)icm->acce;
//  icm20602_spi_read_regs(w_buff, data, 15, id);
  return 14;
}

void Icm20602ResetFifo(uint32_t id) {
  icm20602_spi_write_reg(ICM20602_USER_CTRL, FIFO_RST, id);
  delay(1);
  icm20602_spi_write_reg(ICM20602_FIFO_EN, GYRO_FIFO_EN | ACCEL_FIFO_EN, id);
  icm20602_spi_write_reg(ICM20602_USER_CTRL, FIFO_EN, id);
  delay(1);
}
#if ENABLE_MULTI_IMUS == 1
void Icm20602MultiInit(IMU_MODE mode, IMU_RATE rate) {
//  HAL_NVIC_DisableIRQ(ICM_INT_1_EXTI_IRQn);
//  HAL_NVIC_DisableIRQ(ICM_INT_3_EXTI_IRQn);
  uint32_t state = 0;
  for (int i = 0; i < 4; i++) {
	icm20602_id = i;
	//	HAL_NVIC_DisableIRQ(ICM_INT_0_EXTI_IRQn);
	icm20602_init_spi(mode, rate, icm20602_id);
	state = icm20602_status(icm20602_id);
	delay(10);
	trace(Info, "IMU %d : %d\n", icm20602_id, state);
  }
/*
	icm20602_id = 1;
//	HAL_NVIC_DisableIRQ(ICM_INT_1_EXTI_IRQn);
	icm20602_init_spi(mode, rate,icm20602_id);
//	state = icm20602_status(icm20602_id);
	delay(100);
	trace(Info,"IMU %d : %d\n",icm20602_id,state);

	icm20602_id = 2;
	icm20602_init_spi(mode, rate,icm20602_id);
	state = icm20602_status(icm20602_id);
	delay(100);
	trace(Info,"IMU %d : %d\n",icm20602_id,state);

	icm20602_id = 3;
	icm20602_init_spi(mode, rate,icm20602_id);
	state = icm20602_status(icm20602_id);
	delay(100);
	trace(Info,"IMU %d : %d\n",icm20602_id,state);*/

//	HAL_NVIC_SetPriority(ICM_INT_0_EXTI_IRQn, 5, 0);
//	HAL_NVIC_EnableIRQ(ICM_INT_0_EXTI_IRQn);
//	HAL_NVIC_SetPriority(ICM_INT_3_EXTI_IRQn, 5, 0);
//	HAL_NVIC_EnableIRQ(ICM_INT_3_EXTI_IRQn);
//	HAL_NVIC_SetPriority(ICM_INT_2_EXTI_IRQn, 5, 0);
//	HAL_NVIC_EnableIRQ(ICM_INT_2_EXTI_IRQn);
//	HAL_NVIC_SetPriority(ICM_INT_1_EXTI_IRQn, 5, 0);
//	HAL_NVIC_EnableIRQ(ICM_INT_1_EXTI_IRQn);

// icm20602_init_spi(mode, rate);
//  icm20602_id = 0;
}
#endif


