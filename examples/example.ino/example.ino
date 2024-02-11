#include "eLCDIF_t4.h"
eLCDIF_t4 lcd;
eLCDIF_t4_config lcd_config = {480, 8, 8, 4, 800, 8, 8, 4, 25, 24, 0, 0};

void setup() {
  // put your setup code here, to run once:
  lcd.begin(BUS_24BIT, WORD_24BIT, lcd_config);

}

void loop() {
  // put your main code here, to run repeatedly:

}
