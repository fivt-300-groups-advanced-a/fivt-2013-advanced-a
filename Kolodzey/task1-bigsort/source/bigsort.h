
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
///исправлен баг k < totalNumverOfItems
///------------------------------------
///Кусок проблемы был даже не в этом, а в том, 
///что sort_heap сортировал кучу неправильно. Вообще не сортировал :)
///sort_heap заменён на std::sort
///
///сделана работающая функция bigsort
///--------------------------------
///Любой тип, любой сортер, sortcmp включён в sorter
///
///понято, как передать ifstream аргументом
///----------------------------------------
///Нужно помнить, что потоки ввода-вывода нельзя копировать
///Поэтому reader-ы и writer-ы нужно передавать по ссылке
///
///понято, как скомпилировать шаблонную функцию
///--------------------------------------------
///Шаблонная ф-ия должна быть в header-е, иначе проект не соберётся
///Иначе откуда компилятору знать о том, при каких конкретных параметрах
///ты её вызовешь?
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
///Зы. Читайте конспект за 07.10.2013 в папке ../lesson-notes 
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
#include <vector>

///\brief Пространство имён, в котором всё и находится
namespace bigsort
{
///\brief та самая функция, ради которой всё писалось
	template <class T,
	          class Reader,
	          class Writer,
	          class Separator,
	          class TmpStream,
	          class TmpSeparator,
	          class Sorter,
	          class MergeCmp,
	          class DebugStream>

	void bigsort(///ссылка на переменную типа сортируемого значения\n
		         ///swap() должен быть определён для T
				 T 	          &tmpVal,
				 ///ссылка на поток ввода \n
				 ///должен иметь оператор >> переопределённый для T \n
				 ///должен иметь метод seekg(0, reader.beg);
				 ///должен иметь метод tellg();
				 ///должен иметь метод eof();
			     Reader       &reader,
			     ///ссылка на поток вывода \n
			     ///должен иметь оператор << перопределённый для T и Separator
			     Writer       &writer,
			     ///То, чем разделены значения в ответе
			     Separator    separator,
			     ///количество кусков, на котрое пилятся входные данные
		         int          k,
		         ///поток промежуточного ввода/вывода\n
		         ///tmpStream[i] должен иметь оператор >> для T; 
		         ///оператор << для T и TmpSeparator\n
		         ///метод seekg(0, tmpStream[i].beg)
		         ///обращения tmpStream[0] ... tmpStream[k - 1] корректны
		         TmpStream    tmpStream,
		         ///То, чем разделены значения в промежуточном выводе
		         TmpSeparator tmpSeparator,
		         ///Сортировщик в памяти\n
		         ///Вызывается sorter(v.begin(), v.end()), где v - вектор <T>
		         Sorter       sorter,
		         ///Компаратор, с помощью которого происходит объединение 
		         ///промежуточных значений\n
		         ///сравнивает пары <T, int> по T\n
		         MergeCmp     mergecmp,
		         ///Ссылка на поток логов, разговоров с пользователем
		         DebugStream &debugStream)
	{
		debugStream << "Ha-ha-ha, I'm a sorting function!!\n";
		
		reader.seekg(0, reader.end);
		size_t totalSize = reader.tellg();
    	reader.seekg(0, reader.beg);

		debugStream << "totalSize " << totalSize << std::endl;

		///\todo заменить k на задание количества памяти
		size_t blockSize = totalSize / k;

		debugStream << "blockSize " << blockSize << std::endl;

		long long oldPos = 0;
		long long totalNumberOfItems = 0;

		for (int i = 0; i < (k - 1); ++i)
		{
			size_t nowSize = 0;
			std::vector<T> v;
			v.clear();
			debugStream << "I'm reading piece #" << i << std::endl;
			while (nowSize < blockSize)
			{
				/*
				debugStream << "nowSize " << nowSize << std::endl;
				*/
				reader >> tmpVal;
				v.push_back(tmpVal);
				++totalNumberOfItems;
				/*
				debugStream << tmpVal << std::endl;
				*/
				nowSize = (long long)(reader.tellg()) - oldPos;
			}
			oldPos = nowSize;

			debugStream << "I'm sorting piece #" << i << std::endl;
			sorter(v.begin(), v.end());
			debugStream << "I've sorted piece #" << i << std::endl;
			for (int j = 0; j < v.size(); ++j)
				tmpStream[i] << tmpSeparator << v[j];
			debugStream << "I've writed piece #" << i << std::endl;
		}

		std::vector<T> v;
		v.clear();
		debugStream << "I'm reading piece #" << k << std::endl;
		while (!reader.eof())
		{
			reader >> tmpVal;
			v.push_back(tmpVal);
			++totalNumberOfItems;
		}
		debugStream << "I'm sorting piece #" << k << std::endl;
		sorter(v.begin(), v.end());
		debugStream << "I've sorted piece #" << k << std::endl;
		for (int j = 0; j < v.size(); ++j)
			tmpStream[k - 1] << tmpSeparator << v[j];
		debugStream << "I've writed piece #" << k << std::endl;

		for (int i = 0; i < k; ++i)
			tmpStream[i].seekg(0, tmpStream[i].beg);
		/*
		debugStream << "totalNumberOfItems: "
		            << totalNumberOfItems << std::endl;
		*/
		debugStream << "I'm merging" << std::endl;
		std::vector <std::pair<T, int> > mergeHeap;
		std::make_heap(mergeHeap.begin(), mergeHeap.end(), mergecmp);

		for (int i = 0; (i < k) && (totalNumberOfItems > 0); ++i)
		{
			std::pair <T, int> tmp;
			tmpStream[i] >> tmp.first;
			--totalNumberOfItems;
			tmp.second = i;
			mergeHeap.push_back(tmp);
			std::push_heap(mergeHeap.begin(),mergeHeap.end(), mergecmp);
			/*
			debugStream << tmp.second << " " << tmp.first << std::endl;
			*/
		}
		
		debugStream << "totalNumberOfItems: "
		            << totalNumberOfItems << std::endl;
		debugStream << "mergeHeap.size(): "
		            << mergeHeap.size() << std::endl;
		
		///\todo написать так, чтобы компаратор для типа T превращался
		///в компаратор для пары
		
		while (totalNumberOfItems > 0)
		{
			/*debugStream << "OLOLO!" << std::endl;*/
		
			writer << mergeHeap.front().first << separator;
			if (!tmpStream[mergeHeap.front().second].eof())
			{
				std::pair <T, int> tmp;
				int iNowFile = mergeHeap.front().second;		
				tmpStream[iNowFile] >> tmp.first;
				tmp.second = iNowFile;
				--totalNumberOfItems;
				/*
				debugStream << tmp.second << " " << tmp.first << std::endl;
				*/

				mergeHeap.push_back(tmp);
				std::push_heap(mergeHeap.begin(),mergeHeap.end(), mergecmp);

				/*
				debugStream << "Now state of Heap" << std::endl;
				debugStream << "min " << mergeHeap.front().second << " "
				                    << mergeHeap.front().first << std::endl;
				debugStream << "elements" << std::endl;
				for (int i = 0; i < mergeHeap.size(); ++i)
					debugStream << mergeHeap[i].second << " "
				 			  << mergeHeap[i].first << std::endl;
				debugStream << "=======" << std::endl;
				*/
			}
			std::pop_heap(mergeHeap.begin(),mergeHeap.end(), mergecmp);
			mergeHeap.pop_back();
			
			/*
			debugStream << "Now state of Heap" << std::endl;
			debugStream << "min " << mergeHeap.front().second << " "
			                    << mergeHeap.front().first << std::endl;
			debugStream << "elements" << std::endl;
			for (int i = 0; i < mergeHeap.size(); ++i)
				debugStream << mergeHeap[i].second << " "
			 			    << mergeHeap[i].first << std::endl;
			debugStream << "=======" << std::endl;
			*/

			//волшебная константа, благодаря которой разговариваем с юзером
			if (totalNumberOfItems % 1000000 == 0)
				debugStream << totalNumberOfItems << " remaining" << std::endl;
		}

		std::sort(mergeHeap.begin(), mergeHeap.end(), mergecmp);

		for (int i = mergeHeap.size() - 1; i >= 0; --i)
			writer << mergeHeap[i].first << separator;
	}
};