/**
* @file bsp.h in InsCube
* @author linfe
* @comment
* Create on 2022/8/25 23:01
* @version 1.0
**/

#ifndef INSCUBE_CORE_SRC_BSP_BSP_H_
#define INSCUBE_CORE_SRC_BSP_BSP_H_

#include "cmsis_os.h"
#define bsp_malloc(size) pvPortMalloc(size)
/*当前模式不支持free操作*/
#define bsp_free(size) vPortFree(size)

typedef struct {
  uint32_t millisecond;
  uint32_t microsecond;
} system_time_t;
#define TM_TO_DOUBLE(tm)   ((double)(tm).millisecond*1e-3+(double)(tm).microsecond*1e-6)
typedef enum {
  bsp_io_ok = 0,
  bsp_io_error,
  bsp_io_init_error,
  bsp_io_wd_error,                        /*write error*/
  bsp_io_rd_error,                        /*read error*/
  bsp_io_not_supported,                   /*the function is NULL*/
} bsp_io_error_t;                        /*Error code for bsp*/

typedef enum{
  bsp_os_ok = 0,
  bsp_os_msg_timeout,
  bsp_os_msg_full,
}bsp_os_error_t;


typedef enum {
  bsp_io_state_reset = 0,
  bsp_io_state_set = 1,
  bsp_io_state_unknown = 2,
} bsp_io_state_t;

typedef struct {
  size_t ( *write )(uint8_t *buffer, size_t length);
  size_t (*read)(uint8_t *buffer, size_t length);
  bsp_io_error_t (*ioctl)(bsp_io_state_t io_state);
  bsp_io_error_t (*init)();
  bsp_io_error_t (*deinit)();
} bsp_device_t;

typedef enum {
  gpio_exit = 0,
  spi_tx_int,
  spi_rx_int,
} bsp_interrupt_type_t;
typedef struct {
  bsp_interrupt_type_t type;
  void *bsp_interrupt_callback;
  void *para;
  uint16_t gpio_pin;

} bsp_interrupt_dev_t;

/*public functions*/

void bsp_init();

/**
 * write data to device
 * @param dev point to peripherals
 * @param buffer data to be written
 * @param size
 * @return
 */
bsp_io_error_t dev_write(bsp_device_t *dev, uint8_t *buffer, size_t size);

/**
 * read data from device
 * @param dev point to peripherals
 * @param buffer data
 * @param size
 * @return
 */
size_t dev_read(bsp_device_t *dev, uint8_t *buffer, size_t size);
bsp_io_error_t dev_ioctl(bsp_device_t *dev, bsp_io_state_t state);
bsp_io_error_t dev_init(bsp_device_t *dev);
bsp_io_error_t dev_deinit(bsp_device_t *dev);

void bsp_delay_ms(uint32_t ms);

typedef enum {
  gpio_up,
} bsp_irq_type_t;

typedef struct {
  uint16_t gpio_pin;
  bsp_device_t device_;
  void (*callback)(void *);
  void *parameter;
}bsp_gpio_irq_desc_t;

typedef struct{
  uint8_t uart_rx_buffer[128];
  bsp_device_t *uart_dev;
}bsp_uart_irq_desc_t;

void register_irq(bsp_irq_type_t type, void *callback);
void register_gpio_irq(uint16_t gpio_pin, void *callback, void *parameter);
void get_system_time(system_time_t *tm);

typedef osPriority bsp_priority_t;
typedef QueueHandle_t bsp_message_queue_t;

typedef struct {
  char task_name[25];
  void (*task_fn)(void *p);
  uint32_t stack_size;
  void *parameter;
  bsp_priority_t priority;
  void *handle;
} bsp_task_t;

typedef struct {
  QueueHandle_t queue;
  size_t item_size;
  uint32_t capacity;
  uint8_t isr_send;
  uint8_t isr_get;
} bsp_msg_t;

/*tasks*/
void bsp_create_task(bsp_task_t *p);
void bsp_suspend_task(bsp_task_t *p);
void bsp_delete_task(bsp_task_t *p);
void bsp_msg_init(bsp_msg_t *p);
bsp_os_error_t bsp_msg_send(bsp_msg_t *p, void *item);
bsp_os_error_t bsp_msg_get(bsp_msg_t *p, void *buffer, uint32_t max_delay);

void bsp_cpu_timeinfo(char *buffer);

/*queues*/
void bsp_queue_push(bsp_message_queue_t queue, void *item);

/*available devices on this board  */
extern bsp_device_t spi1_dev;
extern bsp_device_t spi4_dev;
extern bsp_device_t spi5_dev;

extern bsp_device_t gpio_out_K4_dev;
extern bsp_device_t gpio_out_K5_dev;
extern bsp_device_t gpio_out_K6_dev;

extern bsp_device_t uart1_dev;
extern bsp_device_t uart2_dev;
extern bsp_device_t uart3_dev;
extern bsp_device_t uart4_dev;
extern bsp_device_t uart5_dev;
extern bsp_device_t uart6_dev;

#endif //INSCUBE_CORE_SRC_BSP_BSP_H_
