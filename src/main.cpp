#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <tgbot/tgbot.h>
#include "../include/getinfo.h"

using namespace std;
using namespace TgBot;

const int questionsNumber = 19;
bool is_digit(string str);

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
		bot.getApi().sendMessage(message->chat->id, "Добро пожаловать, я бот-опросник канала Stat_0+\n<a href = \"t.me/st_opr\">Ссылка на канал</a>\n<a href = \"t.me/SEKVANTO\">Связаться с организатором</a>\n/help - получение дополнительной информации\n/meme - мем\n<a href = \"https://github.com/sekvanto/survey_bot\">Исходники бота на гитхабе</a>\nЕсли вы нашли какой-либо баг, обратитесь к организатору либо создайте тему на гитхабе", true, 0, make_shared< GenericReply >(), "HTML");
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
	bool FLAG=false; // индикатор ожидания ответа на вопрос
	bool SSS=false; // short sleeper syndrome

	bot.getEvents().onCommand("go", [&bot, &iterator, &FLAG, &SSS](Message::Ptr message) {
			if(message->chat->type != Chat::Type::Private)
					bot.getApi().sendMessage(message->chat->id, "Напиши в личку для прохождения");

			// начинаем опрос

			else {
				string filename = "../../qbot_extra/data/" + to_string(message->from->id);
				string itername = filename + "_iterator";
				ifstream check_file (itername); // проверяем наличие итератор-файла попыткой его открыть
				string buf;
				ofstream iterator_write;
				if (check_file.is_open()){
					getline(check_file, buf);
					iterator = stoi(buf);
					check_file.close();
					//
					cout << "Iterator is: " << iterator << endl;
				}
				else {
					check_file.close();
					iterator_write.open(itername);
					iterator_write << "0";
					iterator_write.close();
					// запишем юзерку в filename
					iterator_write.open(filename);
					iterator_write << message->from->username << endl;
					iterator_write.close();
				}
			
				string Strings[questionsNumber+1];
				int index = 0;
				ifstream getq ("../attachments/questions");
				if (getq.is_open()) while(getline(getq, Strings[index])) ++index; // записываем вопросы из файла в массив
				getq.close();
				cout << "Iterator now: " << iterator << endl;

				bot.getApi().sendMessage(message->chat->id, "Вопрос (отправьте ваш ответ сообщением, которое будет записано в базу данных). После ответа на вопрос нажмите /go для получения следующего");
				if (SSS==false && iterator==8) iterator = 15;
				if (SSS==true && iterator==15) iterator = questionsNumber;
				if (iterator < questionsNumber) {
						// задаем вопрос и устанавливаем FLAG
						bot.getApi().sendMessage(message->chat->id, Strings[iterator]);
						FLAG=true;
				}
				else bot.getApi().sendMessage(message->chat->id, "Вы уже прошли опрос, либо же произошла ошибка. Обратитесь к @sekvanto, если что-то пошло не так");
			}
	});


	// ликвидируем всю структуру кастомизирования отдельно взятых вопросов, позже имплементировать
	// сюда добавить мгновенную проверку на SSS

	
	// any message
	bot.getEvents().onAnyMessage([&bot, &iterator, &FLAG, &SSS](Message::Ptr message) {
			if (message->chat->type == Chat::Type::Private) printf("User wrote %s\n", message->text.c_str());
			if (StringTools::startsWith(message->text, "/")) return;
			if (message->chat->type == Chat::Type::Private) {

				string filename = "../../qbot_extra/data/" + to_string(message->from->id);
				string itername = filename + "_iterator";
				ifstream check_file (itername); // проверяем наличие итер-файла попыткой его открыть
				string buf;
				ofstream iterator_write;
				if (!check_file.is_open()){
					check_file.close();
					iterator_write.open(itername);
					iterator_write << "0";
					iterator_write.close();
					// запишем юзерку в filename
					iterator_write.open(filename);
					iterator_write << message->from->username << endl;
					iterator_write.close();
				}
				ofstream write;
				write.open(filename, ios::app); // открываем файл в режиме добавления текста
				write << endl << iterator << ". " << message->text << endl;

				// проверка на SSS, часть 1
				if (iterator==1) {
					if (!is_digit(message->text.c_str())) {
						FLAG=false;
						bot.getApi().sendMessage(message->chat->id, "Введите корректный ответ, целое число.");
					}
					else if (stoi(message->text.c_str()) < 7) SSS=true;
				}
				// проверка на SSS, часть 2
				if (iterator==2) {
					if (!is_digit(message->text.c_str())) {
							FLAG=false;
							bot.getApi().sendMessage(message->chat->id, "Введите корректный ответ, целое число.");
					}
					else if (stoi(message->text.c_str()) > 6) SSS=false;
				}
				if (iterator==18) bot.getApi().sendMessage(message->chat->id, "Итак, спасибо за ваши ответы. Не обещаю скорых результатов, так как поиск новых участников всегда затруднителен, к примеру, в прошлый раз исследование с выборкой всего в 30 человек заняло несколько месяцев.
Результаты будут опубликованы на канале (/start, чтобы узнать подробнее), как промежуточные, так и конечные. Можете заглянуть к нам через несколько месяцев ;). Для того, чтобы получить уведомление об окончании опроса, напишите: \"Я хочу получить уведомление\"");

				write.close();
				if (FLAG==true) {
					++iterator;
					FLAG=false;
					iterator_write.open(itername);
					iterator_write << iterator;
					iterator_write.close();
				}
				bot.getApi().sendMessage(message->chat->id, "В файл было записано: " + message->text);
				bot.getApi().sendMessage(1035974933, "User sent " + message->text + ". Their username: " + message->from->username);
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


bool is_digit(string S) {
	for (int i=0, n = S.length(); i<n; i++) {
		if (S[i] < '0' || S[i] > '9') return false;
	}
	return true;	
}
