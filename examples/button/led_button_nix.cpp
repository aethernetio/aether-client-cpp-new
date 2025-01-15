/*
 * Copyright 2024 Aethernet Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "led_button_nix.h"

#if (defined(BUTTON_NIX))

#  include <X11/Xlib.h>
#  include <X11/XKBlib.h>
#  include <X11/keysym.h>

#  include "aether/tele/tele.h"

namespace ae {

LedButtonNix::LedButtonNix(ActionContext action_context)
    : Action{action_context}, button_timeout_{std::chrono::milliseconds{100}} {
  this->InitIOPriv();
}

LedButtonNix::~LedButtonNix() {}

TimePoint LedButtonNix::Update(TimePoint current_time) {
  bool res{false};

  if (current_time > prev_time_ + button_timeout_) {
    prev_time_ = current_time;
    res = GetKeyPriv(BUT_PIN, BUT_MASK);
    if (res != button_state_) {
      AE_TELED_INFO("Button state changed");
      button_state_ = res;
      this->ResultRepeat(*this);
    }
  }
  return current_time;
}

bool LedButtonNix::GetKey(void) {
  bool res = button_state_;

  return res;
}

void LedButtonNix::SetLed(bool led_state) { SetLedPriv(LED_PIN, led_state); }

void LedButtonNix::InitIOPriv(void) {}

bool LedButtonNix::GetKeyPriv(uint16_t key_pin, uint16_t key_mask) {
  bool res{false};

  auto display = XOpenDisplay(nullptr);
  char keys_return[32];
  XQueryKeymap(display, keys_return);
  KeyCode kc2 = XKeysymToKeycode(display, key_pin);
  if (key_pin == XK_Shift_L) {
    res = !!(keys_return[kc2 >> 3] & (1 << (kc2 & 7)));
  }
  XFlush(display);

  XCloseDisplay(display);

  return res;
}

void LedButtonNix::SetLedPriv(uint16_t led_pin, bool led_state) {
  auto display = XOpenDisplay(nullptr);

  if (led_state) {
    XkbLockModifiers(display, XkbUseCoreKbd, CAPSLOCK | NUMLOCK,
                     led_pin & (CAPSLOCK | NUMLOCK));
  } else {
    XkbLockModifiers(display, XkbUseCoreKbd, CAPSLOCK | NUMLOCK,
                     led_pin & (CAPSLOCK & NUMLOCK));
  }
  XFlush(display);

  XCloseDisplay(display);
}
}  // namespace ae

#endif
