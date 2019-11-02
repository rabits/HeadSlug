#include "buttons.h"
#include "Particle.h"
#include <clickButton.h>
#include "common.h"

ClickButton button_on_head(ONHEAD_PIN, LOW, CLICKBTN_PULLUP);
ClickButton button_touch(TOUCH_PIN, LOW, CLICKBTN_PULLUP);

extern bool headslug_on_head;

extern void buttonOnHeadPressed();
extern void buttonOnHeadReleased();
extern void buttonTouchShort();
extern void buttonTouchLong();

void buttonsSetup() {
    button_on_head.debounceTime   = 20;

    button_touch.debounceTime   = 20;
    button_touch.multiclickTime = 250;
    button_touch.longClickTime  = 1000;
}

void buttonsCheck() {
    button_on_head.Update();
    button_touch.Update();

    if( ! button_on_head.depressed && headslug_on_head )
        buttonOnHeadReleased();
    else if( button_on_head.depressed && ! headslug_on_head )
        buttonOnHeadPressed();

    if( button_touch.clicks == -1 )
        buttonTouchLong();
    else if( button_touch.clicks > 0 )
        buttonTouchShort();
}
