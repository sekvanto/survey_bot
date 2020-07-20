#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <tgbot/tgbot.h>

using namespace std;
using namespace TgBot;

void Questions(int iterator, string filename, Message::Ptr message) {
	string token(getenv("TOKEN"));
	Bot bot(token);
	
	ofstream write_file;
	write_file.open(filename.c_str(), ofstream::out | ofstream::app);
	string Strings[] = {	
							"1.1. Имеете/имели ли вы хронические проблемы со сном? (Бессонница, апноэ во сне, частые пробуждения ночью, гиперсомния, нарколепсия)",
							"Какие конкретно? (опишите одним сообщением)",
							"1.2. Сколько часов вы спите без будильника? (Среднее)",
							"1.2.1. Сколько часов сна вам необходимо для полного отдыха?",
							"1.3. Часто ли вы запоминаете сновидения?",
							"1.4. Какова структура вашего сна?",
							// Сплю только ночью - Иногда есть дневной сон - Часто/Всегда дневной сон
							// Ночной сон разделен на две части - Полифазный сон/Другое
							"Опишите подробнее свой график сна и опыт (в одном сообщении).",
							"1.5. Сталкивались ли вы с тем, что после длительной депривации сна (недосыпания) вы начинали чувствовать себя хорошо даже при таком малом количестве сна?",
							"1.5.1. Досыпали ли вы на выходных? (Спали ли больше?)",
							"1.5.2. Как часто вы просыпались раньше будильника в этот период?",
							"1.5.3. После возвращения на режим без будильников, спали ли вы все так же мало?",
							"1.6. Во сколько предпочитаете ложиться спать?",
							"1.7. Увлекают ли вас эксперименты над природой человека? (Любого масштаба, как лабораторные, так и личные вроде отказа от Интернета на неделю)",
							"2.1. Имеете ли вы родственников, которым требуется менее 6 часов сна для полного отдыха?",
							"Опишите их подробнее (степень родства; время сна, требуемое для отдыха) одним сообщением.",
							"2.2. Расскажите о своей истории сна с самого детства. Является ли врожденным такое небольшое количество сна? Если нет, то что повлияло на изменение? (Посттравматический синдром, другие психические заболевания/отклонения, определенная диета, определенные изменения в жизни и пр.) Опишите одним сообщением.",
							"2.3. Легко ли вам оставаться без сна на длительное время (более суток)?",
							"2.4. Много ли у вас увлечений?",
							"2.4.1. Как сильно вы заняты днем?",
							"2.4.2. Чувствуете ли сонливость при скуке?",
							"2.5. Пытались ли вы спать больше? Каким был результат? (опишите одним сообщением)",
							"3.1. Сталкивались ли вы с тем, что когда-то вы спали меньше обычного (без помощи будильников) и чувствовали себя хорошо во время бодрствования?",
							"3.1.1. Происходило это систематически, или же единоразово?",
							"3.2. Многим ли вы увлечены?",
							"3.3. Часто ли вы испытываете чувство скуки?"
						};
	string After = "Есть ли вам что добавить по этому вопросу? (опишите одним сообщением)";
	
	// YES-NO
	InlineKeyboardMarkup::Ptr yes_no(new InlineKeyboardMarkup);
	vector<InlineKeyboardButton::Ptr> row_yes_no;
	InlineKeyboardButton::Ptr No(new InlineKeyboardButton);
	No->text = "No";
	No->callbackData = "No";
	InlineKeyboardButton::Ptr Yes(new InlineKeyboardButton);
	Yes->text = "Yes";
	Yes->callbackData = "Yes";
	row_yes_no.push_back(No);
	row_yes_no.push_back(Yes);
	yes_no->inlineKeyboard.push_back(row_yes_no);


//	0) вопрос 1.1	
	if (iterator == 0) {
		bot.getApi().sendMessage(message->chat->id, Strings[0], false, 0, yes_no);	
		bot.getEvents().onCallbackQuery([&bot, &yes_no, &write_file](CallbackQuery::Ptr query) {
				cout << query->data << endl;
				bot.getApi().answerCallbackQuery(query->id);
				if (StringTools::startsWith(query->data, "No")) {
					bot.getApi().answerCallbackQuery(query->id);
					write_file << "Нет. ";
				}
		});

			//	bot.getApi().sendMessage(message->chat->id, "Выберите номер опроса.", false, 0, QNumber);
			//	bot.getEvents().onCallbackQuery([&bot, &QNumber](CallbackQuery::Ptr query) {
			//			bot.getApi().answerCallbackQuery(query->id);
			//			GetInfo(stoi(query->data), query->message, query);
			//	});
	}
	
	
	write_file.close();
}
