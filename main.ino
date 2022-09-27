#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Screen
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Array af prompts og info
char *prompts[] = {"arduino", "test"};
int lengthOfPrompts = 0;
int prompt;

// Button
bool buttonPressedFlag = false;
int buttonPin = 4;
bool button;
int state = 0;

// timer
int timer = 2000;
int timerDisplay = 0;
int timerLengthS = 300;
int countDown = 0;
int oldTimer;

// deltatime
float oldTime;
float currentTime;
float deltaTime;
float time = 0;

void setup()
{
    lengthOfPrompts = sizeof(prompts) / sizeof(prompts[0]);

    // lcd
    lcd.init();
    lcd.begin(20, 4); // initialize the lcd for 20 chars 4 lines and turn on backlight
    lcd.backlight();
    lcd.setCursor(0, 1);
    lcd.print("Hello, world! Thøger");
    pinMode(4, INPUT);
    lcd.clear();
}

void loop()
{
    // timer
    oldTime = currentTime;
    currentTime = millis();
    deltaTime = currentTime - oldTime;

    // Button on or off
    button = digitalRead(4);
    // Increment state for buttonpress
    if (state != 3)
    {
        if (button == true && buttonPressedFlag == false)
        {
            if (state >= 3)
            {
                state = 2;
            }
            else
            {
                state++;
            }
            // Update of various when buttonclick instead of every loop
            prompt = random(0, lengthOfPrompts);
            oldTimer = countDown;
            timerDisplay = 0;
            lcd.clear();
        }
        buttonPressedFlag = button;
    }
    // hold button
    else if (button == true && state == 3)
    {
        time += deltaTime;
        if (time > timer)
        {
            state = 0;
            time = 0;
            lcd.clear();
        }
    }
    else if (button == false && time > 0)
    {
        state = 2;
        time = 0;
        lcd.clear();
    }
    //

    switch (state)
    {
    case 0: // Neutral Mode
        oldTimer = timerLengthS;
        countDown = timerLengthS;
        lcd.setCursor(0, 0);
        lcd.print("Klar til at trykke");
        lcd.setCursor(0, 1);
        lcd.print("på knappen!");
        break;

    case 1: // Prompts
        lcd.setCursor(0, 0);
        lcd.print(prompts[prompt]);
        lcd.setCursor(0, 1);
        break;

    case 2: // Countdown timer
        lcd.setCursor(0, 0);
        countDown = oldTimer - (timerDisplay / 1000);
        lcd.print(countDown);
        timerDisplay += deltaTime;
        break;

    case 3: // Frozen time
        lcd.setCursor(0, 0);
        lcd.print(oldTimer);
        lcd.setCursor(0, 1);
        lcd.print("Timer stoppede");
        break;

    default:
        break;
    }

    delay(150);
}
