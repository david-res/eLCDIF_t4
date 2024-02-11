#include "SDRAM_t4.h"
#include "eLCDIF_t4.h"
#include "wallpaper.c"

SDRAM_t4 sdram;
eLCDIF_t4 lcd;
eLCDIF_t4_config lcd_config = {480, 8, 4, 4, 800, 8, 4, 4, 25, 24, 0, 0};


void setup() {
  Serial.begin(115200);
  
  //Start the SDRAM at 133Mhz
  if (!sdram.begin(32, 133,0)){
    Serial.println("SDRAM init fail :( ...");
    }
  else {
    Serial.println("SDRAM init succcess ;)");

    // Allocate aligned memory in SDRAM, 64 byte aligned
    size_t sdram_buffer_size = 800 * 480 * sizeof(uint32_t);
    uint32_t * lcdBuffer1 = (uint32_t *)aligned_malloc(sdram_buffer_size, 64);
    Serial.printf("lcdBuffer1 address: 0x%x \n", lcdBuffer1);

    // Start the eLCFIF and set the current and next buffers (same source for this test)
    lcd.begin(BUS_24BIT, WORD_24BIT, lcd_config);
    lcd.setCurrentBufferAddress(lcdBuffer1);
    lcd.setNextBufferAddress(lcdBuffer1);
  

    // Copy the wallpaper image (uint8_t array) to the SDRAM buffer (uint32_t array) 4 bytes at a time
    size_t buffer1_size = sizeof(wallpaper) / sizeof(uint32_t);
    for (size_t i = 0; i < buffer1_size; ++i) {
        lcdBuffer1[i] = *((uint32_t*)(wallpaper + i * sizeof(uint32_t)));
    }

    //Once everyhing is ready, set the run bit on the eLCDIF to start pushing data to the display
    lcd.runLCD();
  }
}

void loop() {
  delay(1000);
  Serial.println(millis());
}


// Adjusts the pointer to the next multiple of 'alignment'
static inline void *align_forward(void *ptr, size_t alignment) {
    uintptr_t p = (uintptr_t)ptr;
    return (void *)((p + alignment - 1) & ~(alignment - 1));
}

void *aligned_malloc(size_t size, size_t alignment) {
    // Allocate extra space to ensure alignment
    void *ptr = sdram_malloc(size + alignment - 1);
    if (ptr == NULL) {
        return NULL; // Allocation failed
    }
    return align_forward(ptr, alignment);
}