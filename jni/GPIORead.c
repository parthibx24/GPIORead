#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "meta_gpio.h"
#define TAG "[GPIOTST] "
#define NULL_FT {0,0}


//#define NULL 0

#define GIO_RETERR(res, fmt, args...)                                       \
    do {                                                                    \
        printf(TAG "%s:%04d: " fmt"\n", __FUNCTION__, __LINE__, ##args);    \
        return res;                                                         \
    } while(0)

/* GPIO MODE CONTROL VALUE*/
typedef enum {
    GPIO_MODE_GPIO  = 0,
    GPIO_MODE_00    = 0,
    GPIO_MODE_01    = 1,
    GPIO_MODE_02    = 2,
    GPIO_MODE_03    = 3,
    
    GPIO_MODE_MAX,    
    GPIO_MODE_DEFAULT = GPIO_MODE_01,
} GPIO_MODE;

/* GPIO DIRECTION */
typedef enum {
    GPIO_DIR_IN     = 0,
    GPIO_DIR_OUT    = 1,

    GPIO_DIR_MAX,
    GPIO_DIR_DEFAULT = GPIO_DIR_IN,
} GPIO_DIR;

/* GPIO PULL ENABLE*/
typedef enum {
    GPIO_PULL_DISABLE = 0,
    GPIO_PULL_ENABLE  = 1,

    GPIO_PULL_EN_MAX,
    GPIO_PULL_EN_DEFAULT = GPIO_PULL_ENABLE,
} GPIO_PULL_EN;

/* GPIO PULL-UP/PULL-DOWN*/
typedef enum {
    GPIO_PULL_DOWN  = 0,
    GPIO_PULL_UP    = 1,

    GPIO_PULL_MAX,
    GPIO_PULL_DEFAULT = GPIO_PULL_DOWN
} GPIO_PULL;

/* GPIO INVERSION */
typedef enum {
    GPIO_DATA_UNINV = 0,
    GPIO_DATA_INV   = 1,

    GPIO_DATA_INV_MAX,
    GPIO_DATA_INV_DEFAULT = GPIO_DATA_UNINV
} GPIO_INVERSION;

/* GPIO OUTPUT */
typedef enum {
    GPIO_OUT_ZERO = 0,
    GPIO_OUT_ONE  = 1,

    GPIO_OUT_MAX,
    GPIO_OUT_DEFAULT = GPIO_OUT_ZERO,
    GPIO_DATA_OUT_DEFAULT = GPIO_OUT_ZERO,  /*compatible with DCT*/
} GPIO_OUT;
/******************************************************************************
 * GET_MODE_STA
 *****************************************************************************/
int test_gpio_mode(HW_GPIO pin)
{
    GPIO_REQ req_get = {NULL_FT, pin, GET_MODE_STA};
     
    GPIO_CNF cnf = Meta_GPIO_OP(req_get, NULL, 0);
	
	printf("Mode: %d, ", (unsigned char)cnf.data);
    
    return META_SUCCESS;
}
/******************************************************************************
 * GET_DIR_STA
 * GET_DATA_IN
 * GET_DATA_OUT
 *****************************************************************************/
int test_gpio_dir(HW_GPIO pin)
{
    GPIO_REQ req_get = {NULL_FT, pin, GET_DIR_STA};
    GPIO_REQ req_get_dat_in  = {NULL_FT, pin, GET_DATA_IN};
    GPIO_REQ req_get_dat_out = {NULL_FT, pin, GET_DATA_OUT};
     
    GPIO_CNF cnf = Meta_GPIO_OP(req_get, NULL, 0);
	printf("Direction: %d, ", (unsigned char)cnf.data);
    
	cnf = Meta_GPIO_OP(req_get_dat_in, NULL, 0);
	printf("DirIN: %d, ", (unsigned char)cnf.data);
	
    cnf = Meta_GPIO_OP(req_get_dat_out, NULL, 0);
	printf("DirOUT: %d, ", (unsigned char)cnf.data);
   
    return META_SUCCESS;
}
/******************************************************************************
 * GET_PULLEN_STA
 *****************************************************************************/
int test_gpio_pullen(HW_GPIO pin)
{
    GPIO_REQ req_get = {NULL_FT, pin, GET_PULLEN_STA};
     
    GPIO_CNF cnf = Meta_GPIO_OP(req_get, NULL, 0);
	printf("Pull Enable: %d, ", (unsigned char)cnf.data);
    
    return META_SUCCESS;
}
/******************************************************************************
 * GET_PULL_STA
 *****************************************************************************/
int test_gpio_pull(HW_GPIO pin)
{
    GPIO_REQ req_get = {NULL_FT, pin, GET_PULL_STA};
	 
    GPIO_CNF cnf = Meta_GPIO_OP(req_get, NULL, 0);
	printf("PU/PD: %d, ", (unsigned char)cnf.data);

    return META_SUCCESS;
}
/******************************************************************************
 * GET_INV_STA
 *****************************************************************************/
int test_gpio_inv(HW_GPIO pin)
{
    GPIO_REQ req_get = {NULL_FT, pin, GET_INV_STA};
     
    GPIO_CNF cnf = Meta_GPIO_OP(req_get, NULL, 0);
    printf("Inversion: %d", (unsigned char)cnf.data);
    
    return META_SUCCESS;
}
/******************************************************************************
 * test main entry
 *****************************************************************************/
int main(int argc, const char** args)
{
    int pin;
    int res;
    extern void Meta_GPIO_Debug(bool);

    if (false == Meta_GPIO_Init())
    {
        printf(TAG "Meta_GPIO_Init() fail");
        return -1;
    }
    if (argc == 2 && !strcmp(args[1], "-d")) 
    {
        Meta_GPIO_Debug(true);
        printf(TAG "debug on");
    } 
    else
    {
        Meta_GPIO_Debug(false);
        printf(TAG "debug off");
    }
	printf("\n");
    for (pin = HW_GPIO0; pin < HW_GPIO_MAX; pin++)        
    {
        printf("GPIO[%d]: ", pin);
        if (META_SUCCESS != (res = test_gpio_mode(pin)))
        {
            printf(TAG "test_gpio_mode() fail, res = %d\n", res);
            break;
        }
        if (META_SUCCESS != (res = test_gpio_dir(pin)))
        {
            printf(TAG "test_gpio_dir() fail, res = %d\n", res);
            break;
        }
        if (META_SUCCESS != (res = test_gpio_pullen(pin)))
        {
            printf(TAG "test_gpio_pullen() fail, res = %d\n", res);
            break;
        }
        if (META_SUCCESS != (res = test_gpio_pull(pin)))
        {
            printf(TAG "test_gpio_pull() fail, res = %d\n", res);
            break;
        }
        if (META_SUCCESS != (res = test_gpio_inv(pin)))
        {
            printf(TAG "test_gpio_inv() fail, res = %d\n", res);
            break;
        }
        printf("\n");
    }
    if (false == Meta_GPIO_Deinit())
    {
        printf(TAG "Meta_GPIO_DeInit() fail\n");
        return -1;
    }
    return 0;
} 