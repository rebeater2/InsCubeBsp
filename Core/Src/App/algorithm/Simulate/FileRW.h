/**
* @file FileRW.h in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/11/20 12:10
* @version 1.0
**/

#ifndef INSCUBEBSP_CORE_SRC_APP_ALGORITHM_SIMULATE_FILERW_H_
#define INSCUBEBSP_CORE_SRC_APP_ALGORITHM_SIMULATE_FILERW_H_
#include <string>

class DataType {
 public:
  typedef enum {
    RECORDER_TYPE_IMU = 0,
    RECORDER_TYPE_GNSS = 1,
  } recorder_data_type_t;
  typedef struct {
    uint32_t millisecond;
    uint32_t microsecond;
  } system_time_t;
  typedef struct {
    uint16_t stat;/*STAT_OUT*/
    int16_t gyro[3];
    int16_t acce[3];
    int16_t temp;/*TEMP_OUT*/
    uint16_t cnt;/*SMPL_CNTR*/
    uint16_t checksum;
    uint32_t id;
  } imu_raw_adi_t; /*size of ADI=2+2*3+2*3+2+2+2+8 =32 bytes*/
  typedef struct {
    recorder_data_type_t type;
    system_time_t tm;
    union {
      imu_raw_adi_t imu;
      /*other sensor data*/
    };
  } recorder_data_t;
};

class ReaderBase {
 public:
  ReaderBase() = default;
  ~ReaderBase() = default;
  bool isOk() const;
 private:
  bool ok;
};

class LogReader : public ReaderBase, DataType {
 public:
  explicit LogReader(const std::string &filename);
  ~LogReader();

 public:
  bool ReadNext(DataType::recorder_data_t &data);

 private:
  std::string filename;
};

#endif //INSCUBEBSP_CORE_SRC_APP_ALGORITHM_SIMULATE_FILERW_H_
