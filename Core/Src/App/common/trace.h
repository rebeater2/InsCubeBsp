/**
* @file trace.h in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/9/4 9:25
* @version 1.0
**/

#ifndef INSCUBEBSP_CORE_SRC_APP_COMMON_TRACE_H_
#define INSCUBEBSP_CORE_SRC_APP_COMMON_TRACE_H_

/**
 * 通用打印函数，可以通过修改实现不同外设输出，参数列表同printf
 * @param fmt 格式
 * @param ... 变量
 * @return
 */
extern int common_trace(const char *fmt, ...);

void trace_init();

#endif //INSCUBEBSP_CORE_SRC_APP_COMMON_TRACE_H_
