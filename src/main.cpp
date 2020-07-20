#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <tgbot/tgbot.h>
#include "../include/getinfo.h"
#include "../include/questions.h"

using namespace std;
using namespace TgBot;

void ExtraInput();
const int questionsNumber = 25;

int main() {
	string token(getenv("TOKEN"));
    printf("Token: %s\n", token.c_str());
	Bot bot(token);

	// meme
	const string photoFilePath = "sticker/meme.jpg";
	const string photoMimeType = "image/jpeg";
	bot.getEvents().onCommand("meme", [&bot, &photoFilePath, &photoMimeType](Message::Ptr message) {
		bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile(photoFilePath, photoMimeType));
	});


	// start
    bot.getEvents().onCommand("start", [&bot](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Добро пожаловать\n<a href = \"t.me/st_opr\">Ссылка на канал</a>\n<a href = \"t.me/SEKVANTO\">Связаться с организатором</a>\n/help - получение дополнительной информации\n/meme - мем", true, 0, make_shared< GenericReply >(), "HTML");
    });
  

	// help
	bot.getEvents().onCommand("help", [&bot](Message::Ptr message) {
		bot.getApi().sendMessage(message->chat->id, "/go - начать прохождение активного опроса\n/get - получить описание какого-либо опроса\n\nАктивный опрос: 2. Длительность сна, приобретенные аномалии. Сновидения. Депривация сна\n\nПредыдущие опросы: 1. Зависимость ухудшения зрительной функции от наследственных и приобретенных факторов");
	});


	// go
	int iterator = 0;
	bot.getEvents().onCommand("go", [&bot, &iterator](Message::Ptr message) {
			string filename = "..qbot_extra/data/" + to_string(message->from->id);
			ifstream check_file (filename);
			if (check_file.is_open()){
				char temp_c = check_file.get();
				while(check_file.good()){
					++iterator;
					temp_c = check_file.get();
				}
				--iterator;
				check_file.close();
			}
			if(message->chat->type == Chat::Type::Private)
				for(int i = iterator; i < questionsNumber; i++)
					Questions(i, filename, message);
			else bot.getApi().sendMessage(message->chat->id, "Напиши в личку для прохождения");
	});
	

	// get
	bot.getEvents().onCommand("get", [&bot](Message::Ptr message) {
			InlineKeyboardMarkup::Ptr QNumber(new InlineKeyboardMarkup);
			vector<InlineKeyboardButton::Ptr> Qrow;
			InlineKeyboardButton::Ptr Num1(new InlineKeyboardButton);
			InlineKeyboardButton::Ptr Num2(new InlineKeyboardButton);
			Num1->text = "1";
			Num1->callbackData = "1";
			Num2->text = "2";
			Num2->callbackData = "2";
			Qrow.push_back(Num1);
			Qrow.push_back(Num2);
			QNumber->inlineKeyboard.push_back(Qrow);

			if(message->chat->type != Chat::Type::Private) 
				bot.getApi().sendMessage(message->chat->id, "Напиши в личку, чтобы узнать");
			else {
				bot.getApi().sendMessage(message->chat->id, "Выберите номер опроса.", false, 0, QNumber);
				bot.getEvents().onCallbackQuery([&bot, &QNumber](CallbackQuery::Ptr query) {
						bot.getApi().answerCallbackQuery(query->id);
						GetInfo(stoi(query->data), query->message, query);
				});
			}
	});


	signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (exception& e) {
        printf("error: %s\n", e.what());
    }

    return 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////



void ExtraInput() {
}
