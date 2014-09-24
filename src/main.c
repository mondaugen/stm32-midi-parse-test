/* Includes ------------------------------------------------------------------*/
#include <stdlib.h> 
#include "stm32f4xx.h" 
#include "leds.h" 
#include "midi_lowlevel.h" 
#include "mm_midimsgbuilder.h" 

#define MIDI_MSG_DEBUG 
#define NUM_MIDI_MSGS 10 

extern char midiBuffer[]; /* for debugging */

static MIDIMsgBuilder_State_t lastState;
static MIDIMsgBuilder midiMsgBuilder;
static int curNumMIDIMsgs = 0;
static MIDIMsg * midiMsgs[NUM_MIDI_MSGS];

int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
        system_stm32f4xx.c file
     */

    /* Initialize MIDI Message builder */
    MIDIMsgBuilder_init(&midiMsgBuilder);

    /* Enable LEDs so we can toggle them */
    LEDs_Init();
    
    /* Set up midi */
    MIDI_low_level_setup();

    while (1) {
        MIDI_process_buffer();
    }
}

void do_stuff_with_msg(MIDIMsg *msg)
{
    if (curNumMIDIMsgs < NUM_MIDI_MSGS) {
        midiMsgs[curNumMIDIMsgs] = msg;
        curNumMIDIMsgs++;
    }
}

void MIDI_process_byte(char byte)
{
    switch (MIDIMsgBuilder_update(&midiMsgBuilder,byte)) {
        case MIDIMsgBuilder_State_WAIT_STATUS:
            break;
        case MIDIMsgBuilder_State_WAIT_DATA:
            break;
        case MIDIMsgBuilder_State_COMPLETE:
            do_stuff_with_msg(midiMsgBuilder.msg);
            MIDIMsgBuilder_init(&midiMsgBuilder); /* reset builder */
            break;
    }
    lastState = midiMsgBuilder.state;
}
