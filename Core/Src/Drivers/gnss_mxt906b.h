/**
* @file gnss_mxt906b.h in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/10/23 13:11
* @version 1.0
**/

#ifndef INSCUBEBSP_CORE_SRC_DRIVERS_GNSS_MXT906B_H_
#define INSCUBEBSP_CORE_SRC_DRIVERS_GNSS_MXT906B_H_
typedef enum {
  gnss_pos_initial = 0,
  gnss_pos_spp = 1,
  gnss_pos_dgps = 2,
  gnss_pos_fix = 4,
  gnss_pos_float = 5,
  gnss_pos_invalid = 6,
  gnss_pos_reserved = 0xfffffffL  /* sizeof(gnss_mode_t) = 4*/
}gnss_mode_t;
typedef struct {
  double lat;
  double lon;
  float height;
  float pos_std[3];
  gnss_mode_t mode;
}gnss_pos_t;

typedef struct {

}gnss_obs_t;
#endif //INSCUBEBSP_CORE_SRC_DRIVERS_GNSS_MXT906B_H_
