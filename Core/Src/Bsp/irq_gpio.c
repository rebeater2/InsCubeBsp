/**
* @file irq_gpio.c in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/9/4 17:49
* @version 1.0
**/
#include "bsp.h"

/**
 * GPIO 中断服务函数链表
 */
typedef struct gpio_irq_list_t {
  uint16_t gpio_pin;									/*触发中断的引脚*/
  uint16_t id;											/*中断ID*/
  void (*callback)(system_time_t *tm, void *);			/*中断服务函数地址*/
  void *parameter;										/*中断服务函数*/
  struct gpio_irq_list_t *next;
} gpio_irq_list_t;


gpio_irq_list_t *gpio_irq_list = NULL;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	system_time_t tm={0};
	get_system_time(&tm);
	gpio_irq_list_t *p = gpio_irq_list;
	while (p != NULL) {
		if (p->gpio_pin == GPIO_Pin) {
			p->callback(&tm, p->parameter);
			return;
		}
		p = p->next;
	}
}

/**
 *
 * @param gpio_pin
 * @param callback
 * @param parameter
 */
void register_gpio_irq(uint16_t gpio_pin, void *callback, void *parameter) {
	gpio_irq_list_t *item = bsp_malloc(sizeof(gpio_irq_list_t));
	item->next = NULL;
	item->gpio_pin = gpio_pin;
	item->callback = callback;
	item->parameter = parameter;
	gpio_irq_list_t *p = gpio_irq_list;
	if (p == NULL) {
		gpio_irq_list = item;
		return;
	}
	while (p->next != NULL) {
		if (p->gpio_pin == gpio_pin) {
			p->callback = callback;
			p->parameter = parameter;
		}
		p = p->next;
	}
	p->next = item;
}