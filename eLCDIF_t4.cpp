#include "eLCDIF_t4.h"


/*

#### Public Functions ####

*/

FLASHMEM void eLCDIF_t4::begin(BUS_WIDTH busWidth, WORD_LENGTH colorDepth, eLCDIF_t4_config config){
  internal_config = config;
  initLCDPins();
  setVideoClock(4*config.clk_num, config.clk_den);
  initLCDIF(config, busWidth, colorDepth);
};

void eLCDIF_t4::setCurrentBufferAddress(void*buffer){
  arm_dcache_flush_delete((void*)buffer,internal_config.height*internal_config.width);
  LCDIF_CUR_BUF = (uint32_t)buffer;
  

};
void eLCDIF_t4::setNextBufferAddress(void*buffer){
  arm_dcache_flush_delete((void*)buffer,internal_config.height*internal_config.width);
  LCDIF_NEXT_BUF = (uint32_t)buffer;
  

};
FLASHMEM void eLCDIF_t4::onCompleteCallback(CBF callback){
  _callback = callback;

};

FLASHMEM void eLCDIF_t4::runLCD(){
  attachInterruptVector(IRQ_LCDIF, LCDIF_ISR);
  NVIC_SET_PRIORITY(IRQ_LCDIF, 32);
  NVIC_ENABLE_IRQ(IRQ_LCDIF);


  Serial.println("Unmasking frame interrupt");
  // unmask CUR_FRAME_DONE interrupt
  LCDIF_CTRL1_SET = LCDIF_CTRL1_CUR_FRAME_DONE_IRQ_EN;
  // VSYNC_EDGE interrupt also available to notify beginning of raster
  //LCDIF_CTRL1_SET = LCDIF_CTRL1_VSYNC_EDGE_IRQ_EN;
  Serial.println("Running LCD");
  // start LCD
  LCDIF_CTRL_SET = LCDIF_CTRL_RUN | LCDIF_CTRL_DOTCLK_MODE;

};




/*

#### Private Functions ####

*/

FLASHMEM void eLCDIF_t4::setVideoClock(int num, int den){
  int post_divide = 0;
  while (num < 27*den) num <<= 1, ++post_divide;
  int div_select = num / den;
  num -= div_select * den;

  // div_select valid range: 27-54
  float freq = ((float)num / den + div_select) * 24.0f / (1 << post_divide);
  Serial.print("VID_PLL: ");
  Serial.print(freq);
  Serial.print("Mhz, div_select: ");
  Serial.println(div_select);

  // switch video PLL to bypass, enable, set div_select
  CCM_ANALOG_PLL_VIDEO = CCM_ANALOG_PLL_VIDEO_BYPASS | CCM_ANALOG_PLL_VIDEO_ENABLE | CCM_ANALOG_PLL_VIDEO_DIV_SELECT(div_select);
  // clear misc2 vid post-divider
  CCM_ANALOG_MISC2_CLR = CCM_ANALOG_MISC2_VIDEO_DIV(3);
  switch (post_divide) {
      case 0: // div by 1
        CCM_ANALOG_PLL_VIDEO_SET = CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT(2);
        break;
      case 1: // div by 2
        CCM_ANALOG_PLL_VIDEO_SET = CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT(1);
        break;
      // div by 4
      // case 2: PLL_VIDEO pos_div_select already set to 0
      case 3: // div by 8 (4*2)
        CCM_ANALOG_MISC2_SET = CCM_ANALOG_MISC2_VIDEO_DIV(1);
        break;
      case 4: // div by 16 (4*4)
        CCM_ANALOG_MISC2_SET = CCM_ANALOG_MISC2_VIDEO_DIV(3);
        break;
  }
  CCM_ANALOG_PLL_VIDEO_NUM = num;
  CCM_ANALOG_PLL_VIDEO_DENOM = den;
  // ensure PLL is powered
  CCM_ANALOG_PLL_VIDEO_CLR = CCM_ANALOG_PLL_VIDEO_POWERDOWN;
  // wait for lock
  Serial.print("Waiting for PLL Lock...");
  while (!(CCM_ANALOG_PLL_VIDEO & CCM_ANALOG_PLL_VIDEO_LOCK));
  // deactivate bypass
  CCM_ANALOG_PLL_VIDEO_CLR = CCM_ANALOG_PLL_VIDEO_BYPASS;
  Serial.println("done.");


  Serial.print("Configuring LCD pix_clk source...");
  // gate clocks from lcd
  CCM_CCGR2 &= ~CCM_CCGR2_LCD(CCM_CCGR_ON);
  CCM_CCGR3 &= ~CCM_CCGR3_LCDIF_PIX(CCM_CCGR_ON);
  // set LCDIF source to PLL5, pre-divide by 4
  uint32_t r = CCM_CSCDR2;
  r &= ~(CCM_CSCDR2_LCDIF_PRE_CLK_SEL(7) | CCM_CSCDR2_LCDIF_PRED(7));
  r |= CCM_CSCDR2_LCDIF_PRE_CLK_SEL(2) | CCM_CSCDR2_LCDIF_PRED(3);
  CCM_CSCDR2 = r;
  // set LCDIF post-divide to 1
  CCM_CBCMR &= ~CCM_CBCMR_LCDIF_PODF(7);
  CCM_CCGR2 |= CCM_CCGR2_LCD(CCM_CCGR_ON);
  CCM_CCGR3 |= CCM_CCGR3_LCDIF_PIX(CCM_CCGR_ON);
  Serial.println("done.");

};
FLASHMEM void eLCDIF_t4::initLCDPins(){
  //Configure pads to ALT0 - eLCDIF signals
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_00 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_01 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_02 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_03 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_04 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_05 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_06 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_07 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_08 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_09 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_10 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_11 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_12 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_13 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_14 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_15 = 0;

  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_00 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_01 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_02 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_03 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_04 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_05 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_06 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_07 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_08 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_09 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_10 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_11 = 0;



  // configure the LCD pins as outputs, high stength drive
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B0_00 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B0_01 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B0_02 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B0_03 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B0_04 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B0_05 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B0_06 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B0_07 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B0_08 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B0_09 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B0_10 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B0_11 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B0_12 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B0_13 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B0_14 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B0_15 = 0xFF;

  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_00 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_01 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_02 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_03 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_04 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_05 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_06 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_07 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_08 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_09 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_10 = 0xFF;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_11 = 0xFF;
  

};

FLASHMEM void eLCDIF_t4::initLCDIF(eLCDIF_t4_config config, int busWidth, int colorDepth){
  Serial.print("Resetting LCDIF...");
  // reset LCDIF
  // ungate clock and wait for it to clear
  LCDIF_CTRL_CLR = LCDIF_CTRL_CLKGATE;
  while (LCDIF_CTRL & LCDIF_CTRL_CLKGATE);
  Serial.print("poking reset...");
  /* trigger reset, wait for clock gate to enable - this is what the manual says to do...
   * but it doesn't work; the clock gate never re-activates, at least not in the register
   * so the best we can do is to make sure the reset flag is reflected and assume it's done the job
   */
  LCDIF_CTRL_SET = LCDIF_CTRL_SFTRST;
  while (!(LCDIF_CTRL & LCDIF_CTRL_SFTRST));
  
  Serial.print("re-enabling clock...");
  // clear reset and ungate clock again
  LCDIF_CTRL_CLR = LCDIF_CTRL_SFTRST | LCDIF_CTRL_CLKGATE;
  Serial.println("done.");

  Serial.println("Initializing LCDIF registers...");
  // 8 bits in, using LUT
  LCDIF_CTRL = LCDIF_CTRL_WORD_LENGTH(colorDepth) | LCDIF_CTRL_LCD_DATABUS_WIDTH(busWidth) | LCDIF_CTRL_DOTCLK_MODE | LCDIF_CTRL_BYPASS_COUNT | LCDIF_CTRL_MASTER;
  // recover on underflow = garbage will be displayed if memory is too slow, but at least it keeps running instead of aborting
  LCDIF_CTRL1 = LCDIF_CTRL1_RECOVER_ON_UNDERFLOW | LCDIF_CTRL1_BYTE_PACKING_FORMAT(0x07);
  LCDIF_CTRL2 = LCDIF_CTRL2_OUTSTANDING_REQ(3) | LCDIF_CTRL2_BURST_LEN_8(1);
  LCDIF_TRANSFER_COUNT = LCDIF_TRANSFER_COUNT_V_COUNT(config.height) | LCDIF_TRANSFER_COUNT_H_COUNT(config.width);
  // set vsync and hsync signal polarity (depends on mode/resolution), vsync length
  LCDIF_VDCTRL0 = LCDIF_VDCTRL0_ENABLE_PRESENT | LCDIF_VDCTRL0_VSYNC_PERIOD_UNIT | LCDIF_VDCTRL0_VSYNC_PULSE_WIDTH_UNIT | LCDIF_VDCTRL0_VSYNC_PULSE_WIDTH(config.vsw) | config.vpolarity | config.hpolarity;
  // total lines
  LCDIF_VDCTRL1 = config.height+config.vfp+config.vsw+config.vbp;
  // hsync length, line = width+HBP+HSW+HFP
  LCDIF_VDCTRL2 = LCDIF_VDCTRL2_HSYNC_PULSE_WIDTH(config.hsw) | LCDIF_VDCTRL2_HSYNC_PERIOD(config.width+config.hfp+config.hsw+config.hbp);
  // horizontal wait = back porch + sync, vertical wait = back porch + sync
  LCDIF_VDCTRL3 = LCDIF_VDCTRL3_HORIZONTAL_WAIT_CNT(config.hsw+config.hbp) | LCDIF_VDCTRL3_VERTICAL_WAIT_CNT(config.vsw+config.vbp);
  LCDIF_VDCTRL4 = LCDIF_VDCTRL4_SYNC_SIGNALS_ON | LCDIF_VDCTRL4_DOTCLK_H_VALID_DATA_CNT(config.width);
  Serial.printf("LCDIF_CTRL: 0x%x\nLCDIF_CTRL1: 0x%x\nLCDIF_TRANSFER_COUNT: 0x%x\nLCDIF_VDCTRL0: 0x%x\nLCDIF_VDCTRL1: 0x%x\nLCDIF_VDCTRL2: 0x%x\nLCDIF_VDCTRL3: 0x%x\nLCDIF_VDCTRL4: 0x%x\n", LCDIF_CTRL, LCDIF_CTRL1,LCDIF_TRANSFER_COUNT, LCDIF_VDCTRL0, LCDIF_VDCTRL1, LCDIF_VDCTRL2, LCDIF_VDCTRL3, LCDIF_VDCTRL4);

  Serial.println("done.");


};

eLCDIF_t4::CBF eLCDIF_t4::_callback = nullptr;


FASTRUN void eLCDIF_t4::LCDIF_ISR(void) {
  uint32_t intStatus = LCDIF_CTRL1 & (LCDIF_CTRL1_BM_ERROR_IRQ | LCDIF_CTRL1_OVERFLOW_IRQ | LCDIF_CTRL1_UNDERFLOW_IRQ | LCDIF_CTRL1_CUR_FRAME_DONE_IRQ | LCDIF_CTRL1_VSYNC_EDGE_IRQ);
  // clear all pending LCD interrupts
  LCDIF_CTRL1_CLR = intStatus;

  if (intStatus & (LCDIF_CTRL1_CUR_FRAME_DONE_IRQ | LCDIF_CTRL1_VSYNC_EDGE_IRQ)) {
    if (_callback){
    _callback();
    }
  }
  
  asm volatile("dsb");
}


void eLCDIF_t4::InitLUT(void) {
  /* index 6 in the "classic" 16-color palette should be brown instead of dark yellow.
   * The values of green and green-intensity are swapped for this entry to accomplish this
   */
   
  // bits in palette entries match the LCD_DATA* signals
  static const uint32_t red = 1 << 6;                        // red = pin 6
  static const uint32_t green = 1 << 8;                      // green = pin 32
  static const uint32_t blue = 1 << 7;                       // blue = pin 9
  static const uint32_t intensity_g = 1 << 12;               // intensity_g = pin 8
  static const uint32_t intensity = (1 << 13) | intensity_g; // intensity_rb = pin 7

  /* index 0 is output during blanking period!
   * The CRT monitor uses that time to measure black levels, if color
   * lines are active their voltage levels will be sampled as the
   * new ground/black reference.
   * tl,dr: index 0 should ALWAYS be black.
   */

  static const uint32_t fgColorTable[16] = {0, blue, green, blue|green, red, red|blue, red|intensity_g, red|green|blue,
                                            intensity, intensity|blue, intensity|green, intensity|blue|green, intensity|red, intensity|red|blue, intensity|red|green, intensity|red|green|blue};

  // clear LUT1, just to ensure we don't accidentally end up using it
  // (LUT1 is used when the lowest bit of the framebuffer address is set)
  LCDIF_LUT0_ADDR = 0;
  LCDIF_LUT1_ADDR = 0;
  for (size_t i=0; i < (sizeof(fgColorTable)/sizeof(fgColorTable[0])); i++) {
    LCDIF_LUT0_DATA = fgColorTable[i];
    LCDIF_LUT1_DATA = 0;
  }
  // activate LUT
  LCDIF_LUT_CTRL = 0;
}
