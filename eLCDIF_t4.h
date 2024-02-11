#ifndef _ELCDIF_T4_h
#define _ELCDIF_T4_h

#include "Arduino.h"
#include "imxrt.h"


#define LCDIF_CTRL_SFTRST   ((uint32_t)1 << 31)
#define LCDIF_CTRL_CLKGATE  ((uint32_t)1 << 30)
#define LCDIF_CTRL_BYPASS_COUNT  ((uint32_t)1 << 19)
#define LCDIF_CTRL_DOTCLK_MODE  ((uint32_t)1 << 17)
#define LCDIF_CTRL_LCD_DATABUS_WIDTH(n)  ((uint32_t)(((n) & 0x3) << 10))
#define LCDIF_CTRL_WORD_LENGTH(n)  ((uint32_t)(((n) & 0x3) << 8))
#define LCDIF_CTRL_MASTER   ((uint32_t)1 << 5)
#define LCDIF_CTRL_RUN  ((uint32_t)1 << 0)
#define LCDIF_CTRL1_IMAGE_DATA_SELECT  ((uint32_t)1 << 31)
#define LCDIF_CTRL1_CS_OUT_SELECT ((uint32_t)1 << 30)
#define LCDIF_CTRL1_BM_ERROR_IRQ_EN ((uint32_t)1 << 26)
#define LCDIF_CTRL1_BM_ERROR_IRQ ((uint32_t)1 << 25)
#define LCDIF_CTRL1_RECOVER_ON_UNDERFLOW ((uint32_t)1 << 24)
#define LCDIF_CTRL1_INTERLACE_FIELDS ((uint32_t)1 << 23)
#define LCDIF_CTRL1_START_INTERLACE_FROM_SECOND_FIELD  ((uint32_t)1 << 22)
#define LCDIF_CTRL1_FIFO_CLEAR ((uint32_t)1 << 21)
#define LCDIF_CTRL1_IRQ_ON_ALTERNATE_FIELDS ((uint32_t)1 << 20)
#define LCDIF_CTRL1_BYTE_PACKING_FORMAT(n) ((uint32_t)(((n) & 0xF) << 16))
#define LCDIF_CTRL1_OVERFLOW_IRQ_EN ((uint32_t)1 << 15)
#define LCDIF_CTRL1_UNDERFLOW_IRQ_EN  ((uint32_t)1 << 14)
#define LCDIF_CTRL1_CUR_FRAME_DONE_IRQ_EN  ((uint32_t)1 << 13)
#define LCDIF_CTRL1_VSYNC_EDGE_IRQ_EN  ((uint32_t)1 << 12)
#define LCDIF_CTRL1_OVERFLOW_IRQ  ((uint32_t)1 << 11)
#define LCDIF_CTRL1_UNDERFLOW_IRQ  ((uint32_t)1 << 10)
#define LCDIF_CTRL1_CUR_FRAME_DONE_IRQ  ((uint32_t)1 << 9)
#define LCDIF_CTRL1_VSYNC_EDGE_IRQ  ((uint32_t)1 << 8)
#define LCDIF_TRANSFER_COUNT_V_COUNT(n) ((uint32_t)(((n) & 0xFFFF) << 16))
#define LCDIF_TRANSFER_COUNT_H_COUNT(n) ((uint32_t)(((n) & 0xFFFF) << 0))
#define LCDIF_VDCTRL0_ENABLE_PRESENT ((uint32_t)1 << 28)
#define LCDIF_VDCTRL0_VSYNC_POL ((uint32_t)1 << 27)
#define LCDIF_VDCTRL0_HSYNC_POL ((uint32_t)1 << 26)
#define LCDIF_VDCTRL0_DOTCLK_POL ((uint32_t)1 << 25)
#define LCDIF_VDCTRL0_ENABLE_POL  ((uint32_t)1 << 24)
#define LCDIF_VDCTRL0_VSYNC_PERIOD_UNIT ((uint32_t)1 << 21)
#define LCDIF_VDCTRL0_VSYNC_PULSE_WIDTH_UNIT ((uint32_t)1 << 20)
#define LCDIF_VDCTRL0_HALF_LINE ((uint32_t)1 << 19)
#define LCDIF_VDCTRL0_HALF_LINE_MODE ((uint32_t)1 << 18)
#define LCDIF_VDCTRL0_VSYNC_PULSE_WIDTH(n) ((uint32_t)(((n) & 0x3FFFF) << 0))
#define LCDIF_VDCTRL2_HSYNC_PULSE_WIDTH(n) ((uint32_t)(((n) & 0x3FFF) << 18))
#define LCDIF_VDCTRL2_HSYNC_PERIOD(n)  ((uint32_t)(((n) & 0x3FFFF) << 0))
#define LCDIF_VDCTRL3_MUX_SYNC_SIGNALS ((uint32_t)1 << 29)
#define LCDIF_VDCTRL3_VSYNC_ONLY ((uint32_t)1 << 28)
#define LCDIF_VDCTRL3_HORIZONTAL_WAIT_CNT(n) ((uint32_t)(((n) & 0xFFF) << 16))
#define LCDIF_VDCTRL3_VERTICAL_WAIT_CNT(n) ((uint32_t)(((n) & 0xFFFF) << 0))
#define LCDIF_VDCTRL4_DOTCLK_DLY_SEL(n) ((uint32_t)(((n) & 0x7) << 29))
#define LCDIF_VDCTRL4_SYNC_SIGNALS_ON ((uint32_t)1 << 18)
#define LCDIF_VDCTRL4_DOTCLK_H_VALID_DATA_CNT(n)  ((uint32_t)(((n) & 0xFFFF) << 0))

#define IMXRT_LCDIF_LUT		(*(IMXRT_REGISTER32_t *)(IMXRT_LCDIF_ADDRESS+0xb00))
#define LCDIF_LUT_CTRL		(IMXRT_LCDIF_LUT.offset000)
#define LCDIF_LUT0_ADDR		(IMXRT_LCDIF_LUT.offset010)
#define LCDIF_LUT0_DATA		(IMXRT_LCDIF_LUT.offset020)
#define LCDIF_LUT1_ADDR		(IMXRT_LCDIF_LUT.offset030)
#define LCDIF_LUT1_DATA		(IMXRT_LCDIF_LUT.offset040)


enum BUS_WIDTH {
        BUS_8BIT = 1,
        BUS_16BIT = 0,
        BUS_18BIT = 2,
        BUS_24BIT = 3
    };
enum WORD_LENGTH{
        WORD_8BIT = 1,
        WORD_16BIT = 0,
        WORD_18BIT = 2,
        WORD_24BIT = 3
    };

typedef struct {
  uint32_t height;
  uint32_t vfp; // vertical front porch
  uint32_t vsw; // vertical sync width
  uint32_t vbp; // vertical back porch
  uint32_t width;
  uint32_t hfp; // horizontal front porch
  uint32_t hsw; // horizontal sync width
  uint32_t hbp; // horizontal back porch
  // clk_num * 24MHz / clk_den = pixel clock
  uint32_t clk_num; // pix_clk numerator
  uint32_t clk_den; // pix_clk denominator
  uint32_t vpolarity; // 0 (active low vsync/negative) or LCDIF_VDCTRL0_VSYNC_POL (active high/positive)
  uint32_t hpolarity; // 0 (active low hsync/negative) or LCDIF_VDCTRL0_HSYNC_POL (active high/positive)
} eLCDIF_t4_config;

class eLCDIF_t4 {
    public:
    void begin(BUS_WIDTH busWidth, WORD_LENGTH colorDepth, eLCDIF_t4_config config);
    void setCurrentBufferAddress(const void*buffer);
    void setNextBufferAddress(const void*buffer);
    typedef void(*CBF)();
    static CBF _callback;
    void onCompleteCallback(CBF callback);
    
    private:
    int _busWidth, _colorDepth;
    void setVideoClock(int num, int den);
    void initPins();
    void initLCDIF(eLCDIF_t4_config config);
  
    static volatile bool s_frameDone;
    static void LCDIF_ISR();
    void lcdifCallback();
    bool customCallback = false;
    eLCDIF_t4 *LCDIFcallback;

    void InitLUT();
};
#endif
