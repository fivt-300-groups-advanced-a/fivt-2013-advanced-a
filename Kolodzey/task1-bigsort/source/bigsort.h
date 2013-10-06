
//Комментарии такие большие и страшные, потому что doxygen

///\mainpage
///В этом файле объявлено и даже как-то задокументировано всё,
///что используется мной при сортировке во внешней памяти
///
///Спасибо, что читаете документацию.
///
///Чтобы собрать пробный проект, использующий мою ф-ию сортировки,
///используйте make в папке sample
///
///Чтобы собрать тесты, используйте make в корневом каталоге проекта
///(task1-bigsort). Чтобы запустить тесты, зайдите в папку tests
///
///Чтобы посмотреть исходный код, зайдите в папку source
///
///Также рекомендую продолжить читать документацию
///
///Если вы тот, кто пишет этот проект, то медетируйте на
///автогенерируемое todo из комментариев к коду
///
///Если вы пытаетесь использовать код в своём проекте, то читайте
///описания вызовов функций (понять, что что делает)
///
///Если вы мои одногруппники, читайте то, что написано под заголовком
///«Сделано», чтобы разобраться, как ЭТО вообще получилось
///
///Сделано:
///========
///сделанное выписано в обратном хронологическом порядке
///
///понято doxygen, сделана документация
///------------------------------------
/// <a href="http://www.stack.nl/~dimitri/doxygen/manual/index.html">
///документация по doxygen</a>
///
///понято make, собрано googletest
///-------------------------------
///Читайте мои makefile, причём сначала тот, который в sample:
///там просто сборка проекта.
///В task1-bigsort лежит makefile,
///собирающий unit-testы использующие googletest
/// <a href="http://linux.yaroslavl.ru/docs/prog/gnu_make_3-79_russian_manual.html">
///годная документация на русском</a>
///
///понято, как устроен проект
///--------------------------
/// <a href="http://podelise.ru/tw_files2/urls_202/1/d-719/7z-docs/1_html_m2fe3dd4a.jpg">
///Картинка про файлы .o</a>
///
///понято git
///----------
///полезная вещь ---
/// <a href="http://git-scm.com/documentation">документация</a>
///
///
///Установлено:
///============
///mc, make, doxygen, clang/gnu++,
///скачано googletest (googletest даже собирается)
///Работаю под mac os x 10.8, код пишу в sublime.
///Использую itrerm, потому что там f14 = shift + f4, а не shift + f6.

#include <string>
#include <algorithm>
#include <functional>
#include <iostream>

///\brief Пространство имён, в котором всё и находится
namespace bigsort
{

///\brief Функция сортировки пузырьком в обратную сторону
///
///Написана, чтобы потестить, точно так же, как и bubblesort()
///\todo убрать к чёртовой матери, ибо всё равно не используется
	void reversedBubblesort(int a[], int len);


///\brief Функция сортировки пузырьком.
///
///Написана, чтобы потестить систему unit-тестирования
///Cмысловой нагрузки не несёт, так же как и reversedBubblesort()
///\todo убрать к чёртовой матери, ибо всё равно не используется
	void bubblesort(int a[], int len);


///\brief та самая функция, ради которой всё писалось
	template <class T, class Reader,    class Writer, 
	                   class TmpReader, class TmpWriter,
	                   class Sorter, class SortCmp, class MergeCmp>
	void bigsort(T* pValue,
						  Reader reader, Writer writer,
		         		  size_t memory,
		         		  MergeCmp mergecmp, SortCmp sortcmp, Sorter sorter,
		         		  TmpReader tmpReader, TmpWriter tmpWriter)
	{
		std::cout << "Ha-ha-ha, I'm a sorting function!!\n";
	}
};