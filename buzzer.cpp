#include "buzzer.h"
#include "config.h"
#include "driver/ledc.h"

#define BUZZER_LEDC_CHANNEL   LEDC_CHANNEL_2
#define BUZZER_LEDC_TIMER     LEDC_TIMER_2
#define BUZZER_LEDC_SPEED     LEDC_LOW_SPEED_MODE
#define BUZZER_LEDC_RES       LEDC_TIMER_10_BIT

static bool          beeping      = false;
static unsigned long beepStart    = 0;
static int           beepDuration = 0;

void initBuzzer() {
  ledc_timer_config_t timer = {
    .speed_mode      = BUZZER_LEDC_SPEED,
    .duty_resolution = BUZZER_LEDC_RES,
    .timer_num       = BUZZER_LEDC_TIMER,
    .freq_hz         = 1000,
    .clk_cfg         = LEDC_AUTO_CLK
  };
  ledc_timer_config(&timer);

  ledc_channel_config_t channel = {
    .gpio_num   = PIN_BUZZER,
    .speed_mode = BUZZER_LEDC_SPEED,
    .channel    = BUZZER_LEDC_CHANNEL,
    .timer_sel  = BUZZER_LEDC_TIMER,
    .duty       = 0,
    .hpoint     = 0
  };
  ledc_channel_config(&channel);

  Serial.println("[BUZZER] Initialized.");
}

void beepOnce(int frequencyHz, int durationMs) {
  if (beeping) return; // Don't interrupt an ongoing beep

  ledc_set_freq(BUZZER_LEDC_SPEED, BUZZER_LEDC_TIMER, frequencyHz);
  ledc_set_duty(BUZZER_LEDC_SPEED, BUZZER_LEDC_CHANNEL, 512); // 50% duty
  ledc_update_duty(BUZZER_LEDC_SPEED, BUZZER_LEDC_CHANNEL);

  beeping      = true;
  beepStart    = millis();
  beepDuration = durationMs;
}

void updateBuzzer() {
  if (!beeping) return;

  if (millis() - beepStart >= (unsigned long)beepDuration) {
    ledc_set_duty(BUZZER_LEDC_SPEED, BUZZER_LEDC_CHANNEL, 0);
    ledc_update_duty(BUZZER_LEDC_SPEED, BUZZER_LEDC_CHANNEL);
    beeping = false;
  }
}
