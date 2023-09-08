#ifndef _APP_H_
#define _APP_H_

#define LED_CAR_PORT 'A'
#define LED_CAR_G_PIN 0
#define LED_CAR_Y_PIN 1
#define LED_CAR_R_PIN 2

#define LED_PED_PORT 'B'
#define LED_PED_G_PIN 0
#define LED_PED_Y_PIN 1
#define LED_PED_R_PIN 2

// State variables
typedef enum CAR_LED CAR_LED;
enum CAR_LED {
    GREEN,
    YELLOW,
    RED
};

typedef enum TRAFFIC_MODE TRAFFIC_MODE;
enum TRAFFIC_MODE {
	PEDESTRIAN,
	NORMAL
};

void app_init(void);	// tfms_init

void app_run(void);		// tfms_run

void tfms_log_status(const char* msg);

#endif // !_APP_H_
