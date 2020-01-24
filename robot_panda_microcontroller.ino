#include <Cth.h>
#include <stdlib.h>

#include "grippers.h"
#include "ultrasonic.h"
#include "command_interpreter.h"
#include "pneumatic_system.h"

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// ultrasonic parameters
int number_of_sensors = 0;
Ultrasonic *sensors_array;

const int status_led_pin = 13;
const int trigger_pins[] = {7, 5, 28, 30}; // front, back, left, right
const int echo_pins[]    = {6, 4, 29, 31};

// grippers pwm parameters
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Gripper my_gripper = Gripper();
CommandInterpreter cmd_interperter = CommandInterpreter();
PneumaticSystem my_pneumatic_system = PneumaticSystem();

void initialize_ultrasonic_sensors()
{
  // count the number of pins defined
  number_of_sensors = min(sizeof(trigger_pins), sizeof(echo_pins)) / sizeof(int);

  // create a buffer to store the sensor objects
  sensors_array = new Ultrasonic[number_of_sensors];

  // initialize sensors
  for (int i = 0; i < number_of_sensors; i++)
  {
    sensors_array[i].initialize(
      trigger_pins[i],
      echo_pins[i]
    );
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(1);

  pinMode(status_led_pin, OUTPUT);
  initialize_ultrasonic_sensors();

  my_gripper.initialize(&pwm, A6);
  enable_grippers();

  my_pneumatic_system.initialize(47, 45, 43, 49);
  cmd_interperter.initialize('{', '}', ',');

  Scheduler.startLoop(command_execution_handler);
  Scheduler.startLoop(ultrasonic_sensor_handler);
}

void loop()
{
  digitalWrite(status_led_pin, HIGH);
  delay(1000);
  digitalWrite(status_led_pin, LOW);
  delay(1000);
}

void ultrasonic_sensor_handler()
{
  Serial.println("{" + get_distances_as_string_array() + "}");
  Scheduler.delay(100);
}

String get_distances_as_string_array()
{
  char buff[10];
  String msg = "";
  
  for (int i=0; i<number_of_sensors; i++)
  {
    dtostrf((float) sensors_array[i].getDistance(), 4, 5, buff);
    
    if (i != 0)
    {
      msg = msg + "," + String(buff);
    }
    else
    {
      msg = msg + String(buff);
    }
  }
  
  return "\"d\":[" + msg + "]";
}

void command_execution_handler()
{
  if (Serial.available() > 0)
  {
    cmd_interperter.parse_sequence(Serial.readString());
  }

  cmd_interperter.execute_next_instruction();
  Scheduler.delay(100);
}

void move_gripper_jaw(const int jaw_id, const float new_angle)
{
  my_gripper.set_joint_position(jaw_id, new_angle);
}

void enable_grippers()
{
  my_gripper.enable_motors(true);
}

void disable_grippers()
{
  my_gripper.enable_motors(false);
}

void enable_pump()
{
  my_pneumatic_system.activate_pump();
}

void disable_pump()
{
  my_pneumatic_system.deactivate_pump();
}

void enable_relay(const int relay_id)
{
  my_pneumatic_system.activate_relay(relay_id);
}

void disable_relay(const int relay_id)
{
  my_pneumatic_system.deactivate_relay(relay_id);
}

void enable_all_pneumatics()
{
  my_pneumatic_system.activate_all();
}

void disable_all_pneumatics()
{
  my_pneumatic_system.deactivate_all();
}


