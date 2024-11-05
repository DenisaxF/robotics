#include <Arduino.h>
#define RED_LED 11
#define BLUE_LED 9
#define GREEN_LED 10
#define startButton 2
#define difficultyButton 3
#define WHITE 0
#define GREEN 1
#define OFF 2
#define RED 3

volatile bool buttonPressed = false;
volatile bool dificultyPressed = false;
volatile bool gameActive = false;
volatile bool wordReady = false;
volatile bool wordCompleted = false;
String currentWord = "";
String dificulty = "Easy";
String userInput = "";
unsigned int index = 0;
unsigned int timer1_compare = 10;
volatile unsigned int seconds = 0;

int score = 0;
unsigned long roundStartTime = 0;
const unsigned long ROUND_DURATION = 30000;

const char *dictionar[] = {
    "machine", "cat", "apple", "withdraw", "applies",
    "computer", "park", "museum", "waterpark", "settle",
    "language", "speak", "nest", "best", "wardrobe", "blueberry",
    "peach", "watermelon", "hammer", "street", "corner", "shop",
    "nails", "coffin", "circus", "cherry", "about", "update",
    "mountain", "fountain", "summer", "autumn", "fall", "spring", "winter",
    "cheeseburger", "restaurant", "writer", "exclusive", "headphones",
    "helmet", "motorbike", "bicycle", "princess", "castle", "kingdom",
    "lives", "travells", "is", "monkey", "painting", "dancing", "fight",
    "sword", "library", "eats", "thinking", "queue"};
const int numWords = sizeof(dictionar) / sizeof(dictionar[0]);

String getRandomWord()
{
    int index = random(numWords);
    return String(dictionar[index]);
}

void timer1ISR()
{
    if (gameActive && !wordCompleted)
    {
        currentWord = getRandomWord();
        wordReady = true;
    }
}

void setupTimer1()
{
    noInterrupts();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12);
    OCR1A = 62500;
    TIMSK1 |= (1 << OCIE1A);
    interrupts();
}

ISR(TIMER1_COMPA_vect)
{
    seconds++;
    if (seconds >= timer1_compare)
    {
        seconds = 0;
        timer1ISR();
    }
}

void handleButton()
{
    buttonPressed = true;
}

void handleDifficulty()
{
    dificultyPressed = true;
}

void level()
{
    if (dificulty == "Easy")
    {
        dificulty = "Medium";
        Serial.println("Medium mode on");
        timer1_compare = 5;
        setupTimer1(); // 5 secunde pentru dificultate medie
    }
    else if (dificulty == "Medium")
    {
        dificulty = "Hard";
        Serial.println("Hard mode on");
        timer1_compare = 3;
        setupTimer1(); // 3 secunde pentru dificultate mare
    }
    else
    {
        dificulty = "Easy";
        Serial.println("Easy mode on");
        timer1_compare = 10;
        setupTimer1(); // 10 secunde pentru dificultate ușoară
    }
}

void setup()
{
    Serial.begin(9600);
    Serial.println("Bun venit! Alegeti dificultea");

    randomSeed(analogRead(0));

    pinMode(RED_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(startButton, INPUT_PULLUP);
    pinMode(difficultyButton, INPUT_PULLUP);

    digitalWrite(RED_LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);

    attachInterrupt(digitalPinToInterrupt(startButton), handleButton, FALLING);
    attachInterrupt(digitalPinToInterrupt(difficultyButton), handleDifficulty, FALLING);
}

void setColor(int color)
{
    switch (color)
    {
    case WHITE:
        digitalWrite(RED_LED, HIGH);
        digitalWrite(BLUE_LED, HIGH);
        digitalWrite(GREEN_LED, HIGH);
        break;
    case GREEN:
        digitalWrite(RED_LED, LOW);
        digitalWrite(BLUE_LED, LOW);
        digitalWrite(GREEN_LED, HIGH);
        break;
    case OFF:
        digitalWrite(RED_LED, LOW);
        digitalWrite(BLUE_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        break;
    case RED:
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(BLUE_LED, LOW);
        digitalWrite(RED_LED, HIGH);
        break;
    }
}

void blink()
{
    for (int i = 3; i >= 1; i--)
    {
        setColor(OFF);
        delay(500);
        setColor(WHITE);
        Serial.println(i);
        delay(500);
    }
}

void clearSerialBuffer()
{
    while (Serial.available() > 0)
    {
        Serial.read();
    }
}

void gameRound()
{
    while (Serial.available())
    {
        char incomingChar = Serial.read();
        if (incomingChar == '\b' && userInput.length() > 0)
        {
            userInput.remove(userInput.length() - 1);
            Serial.print('\b');
            setColor(GREEN);
        }
        else if (incomingChar != '\b')
        {
            userInput += incomingChar;
            Serial.print(incomingChar);

            if (incomingChar == currentWord[index])
            {
                setColor(GREEN);
                index++;

                if (index == currentWord.length())
                {
                    score++;
                    wordCompleted = true;
                    currentWord = getRandomWord();
                    Serial.print('\n');
                    Serial.println(currentWord);
                    index = 0;
                    userInput = "";
                }
            }
            else
            {
                setColor(RED);
            }
        }
    }
}

void loop()
{
    if (gameActive && (millis() - roundStartTime >= ROUND_DURATION))
    {
        Serial.print("Timp expirat! Ai scris corect ");
        Serial.print(score);
        Serial.println(" cuvinte.");

        gameActive = false;
        score = 0;
        userInput = "";
        index = 0;
        return;
    }

    if (buttonPressed)
    {
        delay(200);
        if (!gameActive)
        {
            blink();
            setColor(GREEN);
            currentWord = getRandomWord();
            Serial.println(currentWord);
            roundStartTime = millis();
            score = 0;
            gameActive = true;
        }
        else
        {
            gameActive = false;
            setup();
        }
        buttonPressed = false;
        clearSerialBuffer();
    }

    if (dificultyPressed)
    {
        delay(200);
        if (!gameActive)
        {
            level();
        }
        dificultyPressed = false;
    }

    if (wordReady)
    {
        Serial.print('\n');
        Serial.println(currentWord);
        userInput = "";
        index = 0;
        wordReady = false;
        wordCompleted = false;
    }

    if (gameActive && Serial.available())
    {
        gameRound();
    }
}