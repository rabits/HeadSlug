#ifndef VOICE_H
#define	VOICE_H

void voiceSetup();
void voiceStop();
void voiceSetVolume();
void voiceWait(); // If you need to wait until it's done

// *yawns* how much did I sleep?
void sayWakeUp();

// Hey you! Pick me up and put on your head!
void sayPickUp();

// What the reason you take me off from your head?!
void sayPutBack();

// Hi body, I'm your new master headslug! From now on you will execute my commands.
void sayHello();

void sayTouch(); // If touched quickly
void sayMood(); // Will tell the current mood
void saySilent(); // Declare the current silent mode

void sayTaskIntro(); // Task intro
void sayTask(); // Task
void sayTaskEnd(); // Task end

void sayRandom(); // Just say a random phrase

void sayBatteryLow();
void sayBatteryCharging();

#endif // VOICE_H
