#include <MoreBotLibrary.h>

MoreBotRunner bot;

void setup()
{
    bot.setup("MoreBotRunner");
}

void loop()
{
    bot.run();

    delay(10);
}