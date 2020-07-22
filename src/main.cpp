#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <tgbot/tgbot.h>
#include "../include/getinfo.h"

using namespace std;
using namespace TgBot;

const int questionsNumber = 25;

int main() {
	string token(getenv("TOKEN"));
    printf("Token: %s\n", token.c_str());
	Bot bot(token);

	// meme
	const string photoFilePath = "../attachments/meme.jpg";
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


	// go
	int iterator = 0;
	bot.getEvents().onCommand("go", [&bot, &iterator](Message::Ptr message) {
			if(message->chat->type != Chat::Type::Private)
					bot.getApi().sendMessage(message->chat->id, "Напиши в личку для прохождения");

			// начинаем опрос

			else {
				string filename = "../../qbot_extra/data/" + to_string(message->from->id);
				ifstream check_file (filename);
				if (check_file.is_open()){
					char temp_c = check_file.get();
					while(check_file.good()){
						++iterator;
						temp_c = check_file.get();
					}
					check_file.close();
				}
			
				string Strings[questionsNumber+1];
				int index = 0;
				ifstream getq ("../attachments/questions");
				if (getq.is_open()) while(getline(getq, Strings[index])) ++index;
				getq.close();
				ofstream write;
				write.open(filename.c_str(), ios::out | ios::app);
				
				// 0) вопрос 1.1

				InlineKeyboardMarkup::Ptr k11(new InlineKeyboardMarkup);
				vector<InlineKeyboardButton::Ptr> r111;
				InlineKeyboardButton::Ptr b111(new InlineKeyboardButton);
				InlineKeyboardButton::Ptr b112(new InlineKeyboardButton);
				b111->text = "Нет";
				b111->callbackData = "No";
				b112->text = "Да";
				b112->callbackData = "Yes";
				r111.push_back(b111);
				r111.push_back(b112);
				k11->inlineKeyboard.push_back(r111);
			
				if (iterator == 0) {
					bot.getApi().sendMessage(message->chat->id, Strings[0], false, 0, k11);
					bot.getEvents().onCallbackQuery([&bot, &k11, &write](CallbackQuery::Ptr query) {
							bot.getApi().answerCallbackQuery(query->id);
							if (StringTools::startsWith(query->data, "No")) {
								//write << "Нет. ";
							}
					});
				}

				write.close();
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
