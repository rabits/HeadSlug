#include "voice.h"
#include "common.h"

#include "Particle.h"

extern int8_t headslug_mood;
extern bool headslug_silent;

bool voice_busy = false;
int8_t voice_volume = 0;

void voiceFree() {
    voice_busy = false;
}

void voiceSetup() {
    Serial1.begin(9600);

    pinMode(VOICE_ACT_PIN, INPUT_PULLUP); // Act is LOW when audio is in progress
    attachInterrupt(VOICE_ACT_PIN, voiceFree, RISING);

    delay(1000);

    Serial1.println("L");
    voiceSetVolume();
}

void voiceStop() {
    Serial1.println("q");
    delay(100);
}

void voiceWait() {
    while( voice_busy )
        delayMicroseconds(50000); // Wait for 0.05 sec
}

void voiceSetVolume() {
    // Not working correctly...
    /*if( !headslug_on_head ) {
        if( voice_volume == 1 )
            return;
        for( int i = 0; i < 5; i++ )
            Serial1.println("+\r\n+\r\n+\r\n+\r\n+\r\n+\r\n+\r\n+\r\n+\r\n+");
        voice_volume = 1;
    } else {
        if( voice_volume == -1 )
            return;
        for( int i = 0; i < 5; i++ )
            Serial1.println("-\r\n-\r\n-\r\n-\r\n-\r\n-\r\n-\r\n-\r\n-\r\n-");
        voice_volume = -1;
    }*/
}

void voiceExecute(const char* file) {
    voice_busy = true;
    Serial1.printlnf("P%sOGG", file);
}

const char voice_wakeup[][9] = {
    "WAKEUP01", // *yawns* how much did I sleep?
    "WAKEUP02", // *yawns* what time is it?
    "WAKEUP03", // *yawns* it's time to wake up?
    "WAKEUP04", // I'm here to chew bubblegum and conquer some bodies!
    "WAKEUP05", // It's time to rise and subject people to my will!
    "WAKEUP06", // Who am I? Where am I?!
    "WAKEUP07", // Yahhoo! Another day to enslave some humans!
    "WAKEUP08"  // It's time to wake up and subdue!
};

void sayWakeUp() {
    voiceExecute(voice_wakeup[random(0, 8)]);
}

const char voice_pickup[][9] = { // When just powered on and not on head or laying for half a minute
    "PICKUP01", // Hey you! Pick me up and put on your head!
    "PICKUP02", // I demand to listen and put me on your head now!
    "PICKUP03", // You should just pick me and put on someone's head!
    "PICKUP04", // There is nobody to pick me up?
    "PICKUP05", // I want to be placed on someone's head immediately!
    "PICKUP06", // I'm a fancy hat! You will look better with me!
    "PICKUP07", // I'll give you earth money if you will put me on your head!
    "PICKUP08", // I'll make you rich and handsome with me on your head!
    "PICKUP09"  // Please pick me up and let's never be separate!
};

void sayPickUp() {
    voiceExecute(voice_pickup[random(0, 9)]);
}

const char voice_put_back[][9] = { // When was removed from head
    "PUTBACK1", // You look ugly without me, put me back!
    "PUTBACK2", // How dare you to remove me from your head!
    "PUTBACK3", // What do you think you're doing? Leave me on the head!
    "PUTBACK4", // I'm starting count to 3 and you should put me back!
    "PUTBACK5", // What the reason you take me off from your head?!
    "PUTBACK6", // You won't able to breathe without me!
    "PUTBACK7", // You can't even think without me anymore!
    "PUTBACK8", // You will be forever alone without me!
    "PUTBACK9" // Don't you love me the same way I do?
};

void sayPutBack() {
    int8_t section = headslug_mood * 3;
    voiceExecute(voice_put_back[random(section, section+3)]);
}

const char voice_hello[][9] = { // When settled on the head
    "HELLO001", // Obey me! I'm your new master, Headslug, and you will execute my commands!
    "HELLO002", // Ahhahaha fool! Now you're mine! Obey to your new master and be a good body!
    "HELLO003", // Behold! I'm your new god! Follow my instuctions precisely and I will allow you to live!
    "HELLO004", // By putting me on your head you gave yourself in my slavery! Now obey and follow my directions.
    "HELLO005", // Hello my new earth body! I will make sure you will live a long life subordinate to me.
    "HELLO006", // You made right choice allow me to rule you! Your pathetic life without me was miserable anyway.
    "HELLO007", // Congratulations! You just won your new master. Listen to my voice and you will be a perfect carrier!
    "HELLO008"  // Haha, now I'm controlling your body! Execute my directions precisely and I will bless you with laying my eggs in your brain.
};

void sayHello() {
    voiceExecute(voice_hello[random(0, 8)]);
}

const char voice_touch[][9] = { // When touched hidden button for a short time
    "TOUCH001", // You just found my sweet spot, don't touch it anymore...
    "TOUCH002", // Hey! Don't touch me here!
    "TOUCH003", // One more time and I will bite you!
    "TOUCH004", // Ok, that was suitable...
    "TOUCH005", // Be more gentle - I'm sensitive.
    "TOUCH006", // Don't allow anyone but you to touch me there!
    "TOUCH007", // Yep, do it again please!
    "TOUCH008", // I feel good when you touch me there.
    "TOUCH009"  // I'm almost here - please continue.
};

void sayTouch() {
    int8_t section = headslug_mood * 3;
    voiceExecute(voice_touch[random(section, section+3)]);
}

const char voice_mood[][9] = { // When touched hidden button for a long time
    "MOOD0001", // I'm dangerous as usual!
    "MOOD0002", // Ok, let's be friends.
    "MOOD0003"  // I love you dear!
};

void sayMood() {
    voiceExecute(voice_mood[headslug_mood]);
}

const char voice_silent[][9] = { // When silent mode is enabled
    "SILENT01", // Ok, I will be quiet
    "SILENT02"  // Yey! Talking again!
};

void saySilent() {
    voiceExecute(voice_silent[headslug_silent ? 0 : 1]);
}

const char voice_task_intro[][9] = { // When on the head and need to do something
    "TASKI001", // Be prepared to execute my command!
    "TASKI002", // Now it's time to follow my directions.
    "TASKI003", // Lift your ass and follow my commands! I'm ordering you!
    "TASKI004", // Could you help me with the next task?
    "TASKI005", // I have a list of tasks for you.
    "TASKI006", // Let's go and work together on the next tasks.
    "TASKI007", // Don't you mind to follow my directions?
    "TASKI008", // I would like to ask you to do the next things...
    "TASKI009"  // That will be great if you can help me with the next tasks...
};

void sayTaskIntro() {
    int8_t section = headslug_mood * 3;
    voiceExecute(voice_task_intro[random(section, section+3)]);
}

const char voice_tasks[][9] = { // Number of tasks to execute
    "TASK0001", // Turn left
    "TASK0002", // Turn right
    "TASK0003", // Turn around
    "TASK0004", // Jump
    "TASK0005", // Duck
    "TASK0006", // Lay down
    "TASK0007", // Go forward
    "TASK0008", // Go backward
    "TASK0009", // Go sideways left
    "TASK0010", // Go sideways right
    "TASK0011", // Look forward
    "TASK0012", // Look backward
    "TASK0013", // Look left
    "TASK0014", // Look right
    "TASK0015", // Look up
    "TASK0016", // Look down
    "TASK0017", // Raise hands forward
    "TASK0018", // Raise one foot
    "TASK0019", // Wave a hand to someone
    "TASK0020", // Show tongue to someone
    "TASK0021", // Wink to someone
    "TASK0022", // Punch the nearest object
    "TASK0023", // Kiss the nearest object
    "TASK0024", // Raise your hands in the air
    "TASK0025", // Make stupid face
    "TASK0026", // Close your eye
    "TASK0027", // Laugh like heard a good joke
    "TASK0028", // Scratch yourself
    "TASK0029", // Say out loud "I was never so happy without my headslug"
    "TASK0030", // Say out loud "I would like everyone to have their headslugs"
    "TASK0031", // Say out loud "One day I will be better, trust me"
    "TASK0032", // Say out loud "I am the most charming and attractive"
    "TASK0033", // Say out loud "I will show you all, mark my words!"
    "TASK0034", // Say out loud "I'll be back!"
    "TASK0035", // Say out loud "Wabba labba dub dub"
    "TASK0036", // Say out loud "There there..."
    "TASK0037", // Say out loud "Live long and prosper"
    "TASK0038", // Say out loud "Oh my God, they killed Kenny"
    "TASK0039", // Say out loud "Why so serious?"
    "TASK0040", // Say out loud "Yippee Ki Yay, Motherfucker"
    "TASK0041", // Say out loud "The name is Bond, James Bond"
    "TASK0042", // Pretend to be dead
    "TASK0043"  // Pretend to be happy
};

void sayTask() {
    voiceExecute(voice_tasks[random(0, 42)]);
}

const char voice_task_end[][9] = { // When tasks is done
    "TASKE001", // That was it, now wait for another tasks.
    "TASKE002", // Ok, that was the last one - dismissed!
    "TASKE003"  // That was the last one, now rest for a couple of minutes.
};

void sayTaskEnd() {
    voiceExecute(voice_task_end[random(0, 3)]);
}

const char voice_random[][9] = { // Number of tasks to execute
    "RAND0001", // Think harder - I'm starving here!
    "RAND0002", // Don't you agree: everyone should have a headslug on their heads!
    "RAND0003", // Don't you think bitcoin will rule someday?
    "RAND0004", // To be, or not to be, that is the question.
    "RAND0005", // I'm going to make you an offer you can't refuse.
    "RAND0006", // May the force be with you
    "RAND0007", // I love the smell of napalm in the morning
    "RAND0008", // You probably know, I listen to all your mind!
    "RAND0009", // Ahhahah, that thought was nasty and funny! Consumed.
    "RAND0010", // Don't use this shampoo anymore - I prefer with brain taste!
    "RAND0011", // Soon I will lay eggs in your head, that will be funny!
    "RAND0012"  // Probably you should concider to find a new carrier for me...
};

void sayRandom() {
    voiceExecute(voice_random[random(0, 12)]);
}

const char voice_battery_low[][9] = { // When the battery is low
    "BATTLOW1", // My alien phone is discharged - I demand to charge it!
    "BATTLOW2", // Hey you! Battery of my alien phone is almost empty!
    "BATTLOW3"  // Quickly, connect my alien phone to the charger!
};

void sayBatteryLow() {
    voiceExecute(voice_battery_low[random(0, 3)]);
}

const char voice_battery_charging[][9] = { // When the battery is charging
    "BATTOK01", // Oh, that's a relief - I will check new alien kittens in the intergalactic externet for a couple of hours.
    "BATTOK02", // That's was close - now leave me alone for a couple of hours
    "BATTOK03"  // It's charging! Rest for a couple of hours - I will call you when I will be ready to continue my mission.
};

void sayBatteryCharging() {
    voiceExecute(voice_battery_charging[random(0, 3)]);
}


/**
 * WiFi:
 * * Found new ssid: "Here is anothers, find them!"
 * * Updated inactive ssid: "You can run but you can't hide..."
 * * -100dB -> -75dB: "Probably a couple more crawls forward"
 * * -75dB -> -100dB: "Don't let them go!"
 * * -75dB -> -60dB: "We're much close!"
 * * -60dB -> -75dB: "Use your sniffing part to find them!"
 * * -60dB -> -52dB: "Hi, my name is $name, what about you?"
 * * -52dB -> -60dB: "Where you going? I did not completed the conversation!"
 * * -52dB -> -40dB: "What are you doing, it's too close!"
 * * -40dB -> -52dB: "Good boy. Now stay still."
 */
