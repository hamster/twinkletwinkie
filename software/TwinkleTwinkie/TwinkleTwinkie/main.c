#define F_CPU 20000000

#include <atmel_start.h>
#include <util/delay.h>

extern volatile uint8_t measurement_done_touch;
extern volatile uint8_t LED_PWM[3];
extern volatile uint16_t timeTicks;

// Logarithmic brightness levels
const uint8_t pwm_log[] = {30,43,59,78,102,137,196,255};

typedef enum {
	MODE_TWINKLE,
	MODE_RANDOM,
	MODE_CYLON
} RUNMODE;

int main(void){
	
	system_init();
	touch_init();
	
	cpu_irq_enable(); /* Global Interrupt Enable */
	
	LED1_set_level(true);
	_delay_ms(100);
	LED2_set_level(true);
	_delay_ms(100);
	LED3_set_level(true);
	_delay_ms(100);
	LED1_set_level(false);
	_delay_ms(100);
	LED2_set_level(false);
	_delay_ms(100);
	LED3_set_level(false);
	_delay_ms(100);
	
	TIMER_0_init();
	
	LED_PWM[0] = 14;
	LED_PWM[1] = 14;
	LED_PWM[2] = 14;
	
	uint8_t brightnessPosition = 0;
	bool directionUp = true;
	bool twinkling = false;
	
	uint8_t step = 0;
	
	RUNMODE mode = MODE_TWINKLE;
	
	bool touched = false;
	bool changed = false;
	uint8_t twinkleLED = 0;
	uint8_t delay = 0;
	uint8_t delayTop = 100;
	

	/* Replace with your application code */
	while (1) {
		
		touch_process();
		if (measurement_done_touch == 1) {
			if((get_sensor_state(0) & KEY_TOUCHED_MASK) != 0) {
				touched = true;
				} else {
				if(touched){
					switch(mode){
						case MODE_TWINKLE:
							mode = MODE_RANDOM;
							break;
						case MODE_RANDOM:
							mode = MODE_CYLON;
							break;
						case MODE_CYLON:
							mode = MODE_TWINKLE;
							break;
					}
					touched = false;
					changed = true;
				}
			}
		}
		
		if(!touched){
			if(changed){
				// Mode is changing
				LED_PWM[0] = 14;
				LED_PWM[1] = 14;
				LED_PWM[2] = 14;
				
				brightnessPosition = 0;
				directionUp = true;
				twinkling = false;
				step = 0;
				delay = 0;
				
				changed = false;
			}
			
			if(mode == MODE_TWINKLE){
				if(!twinkling){
					// Not twinkling, wait a random time before starting a twinkle
					if((rand() % 50) == 0){
						// Time to twinkle
						twinkling = true;
						twinkleLED = rand() % 3;
					}
				}
				
				if(delay == 0 && twinkling){
					if(directionUp){
						// Go up in brightness
						LED_PWM[twinkleLED] = pwm_log[brightnessPosition++];
						if(brightnessPosition == 6){
							directionUp = false;
						}
					}
					else{
						// Go down in brightness
						LED_PWM[twinkleLED] = pwm_log[--brightnessPosition];
						if(brightnessPosition == 0){
							directionUp = true;
							twinkling = false;
							LED_PWM[0] = 14;
							LED_PWM[1] = 14;
							LED_PWM[2] = 14;
						}
					}
					delay = delayTop;
				}
				else{
					delay -= 5;
				}
			}
			
			if(mode == MODE_RANDOM){
				if(delay == 0){
					LED_PWM[0] = rand() % 196;
					LED_PWM[1] = rand() % 196;
					LED_PWM[2] = rand() % 196;
					delay = delayTop;
				}
				else{
					delay--;
				}
			}
			
			if(mode == MODE_CYLON){
				if(delay == 0){
					switch(step){
						case 0:
							LED_PWM[0] = 196;
							LED_PWM[1] = 14;
							LED_PWM[2] = 14;
							step = 1;
							break;
						case 1:
							LED_PWM[0] = 14;
							LED_PWM[1] = 196;
							LED_PWM[2] = 14;
							step = 2;
							break;
						case 2:
							LED_PWM[0] = 14;
							LED_PWM[1] = 14;
							LED_PWM[2] = 196;
							step = 3;
							break;
						case 3:
							LED_PWM[0] = 14;
							LED_PWM[1] = 196;
							LED_PWM[2] = 14;
							step = 0;
							break;
					}
					delay = delayTop;
				}
				else{
					delay--;
				}
			}
			
		}
		else{
			// Mode is about to change, light up all the LEDs
			LED_PWM[0] = 184;
			LED_PWM[1] = 184;
			LED_PWM[2] = 184;
		}
		
		_delay_ms(1);
		
	}
}
