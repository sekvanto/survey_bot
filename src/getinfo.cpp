#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <tgbot/tgbot.h>
using namespace std;
using namespace TgBot;

void GetInfo(int number, Message::Ptr message, CallbackQuery::Ptr query) {
	string token(getenv("TOKEN"));
	Bot bot(token);
	
	switch(number) {
		case 1:
			bot.getApi().sendMessage(query->message->chat->id, "Опрос 1.0 \"Зависимость ухудшения зрительной функции от наследственных и приобретенных факторов\"");
			bot.getApi().sendMessage(query->message->chat->id, "<a href = \"t.me/st_opr/6\">Исследуемые вопросы</a>\n<a href = \"t.me/st_opr/15\">Статистические данные</a>\n<a href = \"t.me/st_opr/15\">Итоги</a>", true, 0, make_shared<GenericReply>(), "HTML");
			break;
		case 2:
			bot.getApi().sendMessage(query->message->chat->id, "<b>Длительность сна, приобретенные аномалии. Сновидения. Депривация сна</b>\n\nПриветствую! Данный опрос предназначен для поиска новых возможностей оптимизации сна и выявления интересных корреляций между некоторыми факторами (результаты будут доступны после окончания всеобщего опроса и обработки данных).\nИсследования и попытки узнать, можно ли <a href = \"hms.harvard.edu/news/sleep-death-gut\">сократить сон</a> без последствий для того или иного организма имеют большую значимость как для практических целей человека, так и для всеобщей научной мысли. Некоторые люди генетически могут обходиться намного меньшим количеством сна, а некоторые сумели жить без него на протяжение многих десятилетий после неудачного химического эксперимента или иного экстремального происшествия.\nМне хотелось бы изучить опыт тех, кто примет участие в данном опросе.", true, 0, make_shared<GenericReply>(), "HTML");
			bot.getApi().sendMessage(query->message->chat->id, "<a href = \"t.me/st_opr/80\">Исследуемые вопросы</a>\n", true, 0, make_shared<GenericReply>(), "HTML");
			bot.getApi().sendMessage(query->message->chat->id, "Таблица с промежуточными результатами. Конечные появятся при получении данных от выборки необходимого объема.\ndocs.google.com/spreadsheets/d/18ckg4u5DhaPt6ZuVDvaljHDzuiOFZm_dF7YSicu5J0g/edit?usp=sharing", true);
			break;
	}
}
