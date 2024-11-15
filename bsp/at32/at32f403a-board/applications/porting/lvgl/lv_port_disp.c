/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-18     Meco Man     The first version
 */
#include <lvgl.h>
//#include <drv_lcd.h>

#include "at32f403a_407_dma.h"


#define SPI1_TX_DMA_CLOCK               CRM_DMA1_PERIPH_CLOCK
#define SPI1_TX_DMA_CHANNEL             DMA1_CHANNEL3
#define SPI1_TX_DMA_IRQ                 DMA1_Channel3_IRQn

static void spi_dma_init(uint8_t size)
{
    dma_init_type dma_init_struct;

    /* search dma base and channel index */
    //_dma_base_channel_check(instance);

    /* config dma channel */
    dma_default_para_init(&dma_init_struct);
    dma_init_struct.peripheral_inc_enable = FALSE;
    dma_init_struct.memory_inc_enable = TRUE;
    dma_init_struct.peripheral_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
    dma_init_struct.priority = DMA_PRIORITY_MEDIUM;
    dma_init_struct.loop_mode_enable = FALSE;

    crm_periph_clock_enable(SPI1_TX_DMA_CLOCK, TRUE);
    dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;

    dma_init(SPI1_TX_DMA_CHANNEL, &dma_init_struct);

    /* dma irq should set in dma tx mode */
    nvic_irq_enable(SPI1_TX_DMA_IRQ, 0, 1);
}


#define LCD_W 240
#define LCD_H 240

#define MY_DISP_HOR_RES     240
#define DISP_BUFFER_LINES   (240/10)

/*A static or global variable to store the buffers*/
static lv_disp_draw_buf_t disp_buf;

/*Descriptor of a display driver*/
static lv_disp_drv_t disp_drv;

/*Static or global buffer(s). The second buffer is optional*/
static lv_color_t buf_1[MY_DISP_HOR_RES * DISP_BUFFER_LINES];
static lv_color_t buf_2[MY_DISP_HOR_RES * DISP_BUFFER_LINES];

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    uint16_t x1=area->x1;
    uint16_t y1=area->y1;
    uint16_t x2=area->x2;
    uint16_t y2=area->y2;

    lcd_spi_config(8);
    spi_dma_init(0x01);
    LCD_Address_Set(x1,y1,x2,y2);
    
    lcd_spi_config(16);
    spi_send_data(color_p, (x2-x1+1)*(y2-y1+1));
	
    lv_disp_flush_ready(disp_drv);
}

void lv_port_disp_init(void)
{
    /*Initialize `disp_buf` with the buffer(s). With only one buffer use NULL instead buf_2 */
    lv_disp_draw_buf_init(&disp_buf, buf_1, buf_2, MY_DISP_HOR_RES * DISP_BUFFER_LINES);

    lv_disp_drv_init(&disp_drv); /*Basic initialization*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = LCD_W;
    disp_drv.ver_res = LCD_H;

    /*Set a display buffer*/
    disp_drv.draw_buf = &disp_buf;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;
		disp_drv.full_refresh = 1;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

static void anim_x_cb(void * var, int32_t v)
{
    lv_obj_set_x(var, v);
}

static void anim_size_cb(void * var, int32_t v)
{
    lv_obj_set_size(var, v, v);
}

/**
 * Create a playback animation
 */
void lv_example_anim_xxxxxxxxxxx(void)
{

    lv_obj_t * obj = lv_obj_create(lv_scr_act());
    lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, 0);

    lv_obj_align(obj, LV_ALIGN_LEFT_MID, 10, 0);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_values(&a, 10, 50);
    lv_anim_set_time(&a, 1000);
    lv_anim_set_playback_delay(&a, 100);
    lv_anim_set_playback_time(&a, 300);
    lv_anim_set_repeat_delay(&a, 500);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);

    lv_anim_set_exec_cb(&a, anim_size_cb);
    lv_anim_start(&a);
    lv_anim_set_exec_cb(&a, anim_x_cb);
    lv_anim_set_values(&a, 10, 240);
    lv_anim_start(&a);
}

void lv_user_gui_init(void)
{
	lv_example_anim_xxxxxxxxxxx();
}


