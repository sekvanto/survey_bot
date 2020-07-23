#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <ios>
#include <tgbot/tgbot.h>
#include "../include/getinfo.h"

using namespace std;
using namespace TgBot;

const int questionsNumber = 24;

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
	int iterator=0;
	bool meta_iterator=false;
	bot.getEvents().onCommand("go", [&bot, &iterator, &meta_iterator](Message::Ptr message) {
			if(message->chat->type != Chat::Type::Private)
					bot.getApi().sendMessage(message->chat->id, "Напиши в личку для прохождения");

			// начинаем опрос

			else {
				string filename = "../../qbot_extra/data/" + to_string(message->from->id);
				ifstream check_file (filename);
				string buf;
				fstream iterator_write;
				if (check_file.is_open()){
					getline(check_file, buf);
					iterator = stoi(buf);
					check_file.close();
					//
				}
				else {
					iterator_write.open(filename);
					iterator_write << "00\n";
					iterator_write.close();
				}
			
				string Strings[questionsNumber+1];
				int index = 0;
				ifstream getq ("../attachments/questions");
				if (getq.is_open()) while(getline(getq, Strings[index])) ++index;
				getq.close();
				ofstream write;
				write.open(filename, ios::app);
				cout << "Iterator now: " << iterator << endl;

				switch(iterator){

					// вопрос 1.1

					case 0: {

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
			
						int temp=0;
						bot.getApi().sendMessage(message->chat->id, Strings[0], false, 0, k11);
						bot.getEvents().onCallbackQuery([&bot,&temp,&iterator,&meta_iterator](CallbackQuery::Ptr query) {
							if (StringTools::startsWith(query->data.c_str(), "Yes")) {
								temp=1;
								meta_iterator=true; // ждем ответа через onAnyMessage
							}
							else { 
								++iterator;
								// нужно еще обн. значение в файле
							}
							bot.getApi().answerCallbackQuery(query->id);
						});
						cout << temp << endl;
						if(temp==0) write << "1.1. Нет. ";
						else {
							write << "1.1. Да. ";
							bot.getApi().sendMessage(message->chat->id, "Какие именно проблемы? (Опишите в сообщении)");
						}
						break;
						}

					// вопрос 1.2

					//case 1:
					
					// другое значение

					default:
						bot.getApi().sendMessage(message->chat->id, "Вы уже прошли опрос, либо же произошла ошибка. Обратитесь к @sekvanto, если что-то пошло не так");

				}
				bot.getApi().sendMessage(message->chat->id, "Нажмите на одну из кнопок, если вопрос их содержит. Если вы хотите добавить что-то после ответа, напишите это сейчас. (ответьте '-', если нет). Когда напишите необходимое, нажмите /go, чтобы продолжить");
				write.close();
			}
	});


	// any message
	bot.getEvents().onAnyMessage([&bot, &iterator, &meta_iterator](Message::Ptr message) {
			if (message->chat->type == Chat::Type::Private) printf("User wrote %s\n", message->text.c_str());
			if (StringTools::startsWith(message->text, "/")) return;
			if(message->chat->type == Chat::Type::Private) {

				string filename = "../../qbot_extra/data/" + to_string(message->from->id);
				ifstream check_file (filename);
				string buf;
				fstream iterator_write;
				if (!check_file.is_open()){
					iterator_write.open(filename);
					iterator_write << "00\n";
					iterator_write.close();
				}
				ofstream write;
				write.open(filename, ios::app);
				write << message->text << endl;
				write.close();
				bot.getApi().sendMessage(message->chat->id, "В файл было записано: " + message->text);
				bot.getApi().sendMessage(1035974933, "User sent " + message->text + ". Their id: " + to_string(message->chat->id));
				if (meta_iterator) {
				//	++iterator;
					meta_iterator=false;
				}
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
