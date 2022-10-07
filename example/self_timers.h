// *************************************************************
// Code for setting up ESP32 Timers
// *************************************************************
// The base clock of timers is 80MHz --> 12.5ns

// jjtimer0      will tic at 10us  ( 10us = 800 * 12.5ns)
// jjtimer1      will tic at 100ns (100ns =   8 * 12.5ns)

// jjtimer0_intr will alarm every jjper0 tics (jjper0 = 100 for alarm rate of 1ms)
// jjtimer1_intr will alarm every jjper1 tics (jjper1 = 10000000 / (jjsound_freq * 2))
// *************************************************************
hw_timer_t *jjtimer0;
hw_timer_t *jjtimer1;

int32_t jjper0;               // jjtimer0 alarm setting
int32_t jjper1;               // jjtimer1 alarm setting

int32_t jjtime_on;            // sound pulse duration
int32_t jjtime_tper;          // sound pulse period

bool    jjsound_on;           // sound pulse itself
int32_t jjsound_freq;         // sound pulse frequency

int32_t jjcnt0;               // sound pulse counter variable

bool    jjstate1;             // jjtimer1 toggle state variable
bool    jjstate2;             // jjtimer1 toggle state variable
bool    jjspk;

uint8_t jjbdx;
uint8_t jjbnum;

void IRAM_ATTR jjtimer0_intr() {
  if (jjcnt0 == 0) {
    jjsound_on = true;
    jjbdx = (jjbdx + 1) % jjbnum;
  } else if (jjcnt0 == jjtime_on) {
    jjsound_on = false;
  }

  jjcnt0 = jjcnt0 + 1;
  if (jjcnt0 >= jjtime_tper) {
    jjcnt0 = 0;
  }

}

void IRAM_ATTR jjtimer1_intr() {
  if (jjsound_on) {
    jjstate1 = !jjstate1;
    if (jjstate1) {
      jjstate2 = !jjstate2;
    }
  } else {
    jjstate1 = false;
    jjstate2 = false;
  }
  jjspk = (jjbdx == 0) ? jjstate1 : jjstate2;
  digitalWrite(SPK_PIN, jjspk);
}


void jjsetup() {
  jjtimer0 = timerBegin(0, 800, true);
  timerAttachInterrupt(jjtimer0, &jjtimer0_intr, true);
  timerAlarmDisable(jjtimer0);

  jjtimer1 = timerBegin(1, 8, true);
  timerAttachInterrupt(jjtimer1, &jjtimer1_intr, true);
  timerAlarmDisable(jjtimer1);

  // jjtimer0 activates 100 tics --> 100 * 10us = 1ms
  jjper0 = 100;

  // jjtimer1 activates 2 times per period of jjsound_freq;
  jjsound_freq = 440;
  jjper1 = 10000000 / (jjsound_freq * 2);

  jjstate1 = false;
  jjstate2 = false;

  jjbnum = 4;
  jjbdx  = 0;

}

void jjenable0() {
  timerAlarmWrite(jjtimer0, jjper0, true);
  timerAlarmEnable(jjtimer0);
}

void jjenable1() {
  jjper1 = 10000000 / (jjsound_freq * 2);
  jjstate1 = false;
  jjstate2 = false;
  timerAlarmWrite(jjtimer1, jjper1, true);
  timerAlarmEnable(jjtimer1);
}

void jjdisable0() {
  timerAlarmDisable(jjtimer0);
}

void jjdisable1() {
  timerAlarmDisable(jjtimer1);
}


void jjTurnOn() {
  jjsound_on  = false;
  jjcnt0      = 0;
  jjtime_on   = 200;
  jjtime_tper = 600;
  jjenable0();
  jjenable1();
}

void jjTurnOff() {
  jjdisable1();
  jjdisable0();
}


void jjTurnOn_kitchen(uint32_t tlength) {
  jjbnum      = 1;
  jjbdx       = 0;
  jjsound_on  = false;
  jjcnt0      = 1;
  jjtime_on   = 0;
  jjtime_tper = tlength;
  jjenable0();
  jjenable1();
}

void jjTurnOff_kitchen() {
  jjdisable1();
  jjdisable0();
  jjsound_on = false;
}


void jjTurnOn_metronome(uint16_t bpm, uint16_t cnt, uint16_t frq) {
  jjbnum       = cnt;
  jjsound_freq = frq;

  jjbdx       = jjbnum - 1;
  jjsound_on  = false;
  jjcnt0      = 0;
  jjtime_tper = 60000/bpm;
  jjtime_on   = jjtime_tper / 2;
  if (jjtime_tper > 400) {
    jjtime_on = 200;
  } else {
    jjtime_on = jjtime_tper / 2;
  }
  jjenable0();
  jjenable1();
}

void jjTurnOff_metronome() {
  jjdisable1();
  jjdisable0();
  jjsound_on = false;
}

// *************************************************************

