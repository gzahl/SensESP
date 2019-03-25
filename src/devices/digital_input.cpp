#include "digital_input.h"

#include <FunctionalInterrupt.h>

#include "sensesp.h"

DigitalInput::DigitalInput(
    uint8_t pin, int pin_mode, int interrupt_type,
    String id, String schema)
    : Device{id, schema}, pin{pin}, interrupt_type{interrupt_type} {
  pinMode(pin, pin_mode);
  Serial.println(digitalRead(pin));
}

DigitalInputValue::DigitalInputValue(
    uint8_t pin, int pin_mode, int interrupt_type,
    String id, String schema)
    : DigitalInput{pin, pin_mode, interrupt_type, id, schema} {}

void DigitalInputValue::enable() {
  app.onInterrupt(
    pin, interrupt_type,
    [this](){
      value = digitalRead(pin);
      triggered = true;
    });
  app.onTick(
    [this](){
      if (triggered) {
        triggered = false;
        notify();
      }
    }
  );
}

bool DigitalInputValue::get() {
  return value;
}

DigitalInputCounter::DigitalInputCounter(
    uint8_t pin, int pin_mode, int interrupt_type,
    uint read_delay,
    String id, String schema)
    : DigitalInput{pin, pin_mode, interrupt_type, id, schema},
      read_delay{read_delay} {}

void DigitalInputCounter::enable() {
  app.onInterrupt(pin, interrupt_type,
                  [this](){
    this->counter++;
  });
  app.onRepeat(read_delay, [this](){
    noInterrupts();
    value = counter;
    counter = 0;
    interrupts();
    notify();
  });
}

uint DigitalInputCounter::get() {
  return value;
}