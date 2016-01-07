#include <pebble.h>
#include <stdio.h>

Window *my_window;
TextLayer *text_layer, *text_layer2;
TextLayer *s_date_layer, *s_batt_layer;
static int s_battery_level;
static char words[50][18]={"Be Yourself. :)", "Move On!", "Come Back.", "Look Up. :)", "Dream Big!",
                          "Start Living.", "I Am What I Am.", "Define Yourself", "Be Happy. :)", "Stay Positive!",
                          "Trust Yourself.", "Work Hard!", "Be Honest.", "Game On!", "Stay Strong!",
                          "Enjoy Life.", "Thanks to All.", "Go For It!", "Let It Be.", "Keep It Cool.",
                          "Focus, Focus!", "Never Give Up.", "Because I Can.", "Take Action Now", "Create Memories",
                          "Be Nice.", "Stop, Listen.", "Be Here Now.", "Do Your Best.", "Do It Now.",
                          "Focus And Win.", "Be The Change.", "All Is Well.", "Dare To Win.", "Health is Wealth",
                          "No Worries.", "Why So Serious?", "Seize The Day.", "Be Curious.", "Hakuna Matata!",
                          "Be Wise.", "Time Flys.", "Divide, Conquer.", "Stand Up.", "Look For Good.",
                          "My Precious.", "I'll Be Back.", "Just Believe.", "Derek Rocks!", "Let It Go!"};

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  static char s_buffer[8];

  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M"  : "%I:%M" , tick_time);
  text_layer_set_text(text_layer, s_buffer);

  static char date_buffer[16];
  strftime(date_buffer, sizeof(date_buffer), "%a %d %b", tick_time);
  text_layer_set_text(s_date_layer, date_buffer);
    
  static char batt_buf[8];
  snprintf(batt_buf, 8, "Batt:%d", s_battery_level);
  text_layer_set_text(s_batt_layer, batt_buf);
    
  text_layer_set_text(text_layer2, words[rand()%50]);
}

static void tap_handler(AccelAxisType axis, int32_t direction) {
  text_layer_set_text(text_layer2, words[rand()%50]);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  // battery_callback(battery_state_service_peek());
}

static void battery_callback(BatteryChargeState state) {
  s_battery_level = state.charge_percent;
}

void handle_init(void) {
  my_window = window_create();
  Layer *window_layer = window_get_root_layer(my_window);
  GRect bounds = layer_get_bounds(window_layer);
  static GFont s_time_font, s_date_font;
  srand(time(NULL));    
  
  text_layer = text_layer_create(GRect(0, 48, bounds.size.w, 55));
  text_layer_set_background_color(text_layer, GColorRed);
  text_layer_set_text_color(text_layer, GColorWhite);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
    
  text_layer2 = text_layer_create(GRect(0, 0, bounds.size.w, 168));
  text_layer_set_background_color(text_layer2, GColorBlack);
  text_layer_set_text_color(text_layer2, GColorWhite);
  text_layer_set_font(text_layer2, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(text_layer2, GTextAlignmentCenter);
    
  layer_add_child(window_layer, text_layer_get_layer(text_layer2));
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
    
  s_date_layer = text_layer_create(GRect(0, 103, 144, 37));
  text_layer_set_text_color(s_date_layer, GColorBlack);
  text_layer_set_background_color(s_date_layer, GColorGreen);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
    
  s_batt_layer = text_layer_create(GRect(0, 143, 144, 25)); 
  text_layer_set_text_color(s_batt_layer, GColorBlack);
  text_layer_set_background_color(s_batt_layer, GColorWhite);
  text_layer_set_font(s_batt_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_batt_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_batt_layer));
    
  window_stack_push(my_window, true);
  update_time();

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  battery_callback(battery_state_service_peek());
  battery_state_service_subscribe(battery_callback);
  accel_tap_service_subscribe(tap_handler); 
}

void handle_deinit(void) {
  text_layer_destroy(text_layer);
  text_layer_destroy(text_layer2);
  text_layer_destroy(s_batt_layer);
  text_layer_destroy(s_date_layer);
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
