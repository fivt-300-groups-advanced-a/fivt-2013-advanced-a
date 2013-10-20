
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
	template <class T, class Reader,    class Writer, 
	                   class TmpStream,
	                   class Sorter, class MergeCmp,
	                   class Separator, class TmpSeparator>

	void bigsort(T 	          &tmpVal,
			     Reader       &reader,
			     Writer       &writer,
		         int          k,
		         MergeCmp     mergecmp,
		         Sorter       sorter,
		         TmpStream    tmpStream,
		         Separator    separator,
		         TmpSeparator tmpSeparator)
	{
		std::cout << "Ha-ha-ha, I'm a sorting function!!\n";
		
		reader.seekg (0, reader.end);
		size_t totalSize = reader.tellg();
    	reader.seekg(0, reader.beg);

		std::cout << "totalSize " << totalSize << std::endl;

		size_t blockSize = totalSize / k;

		std::cout << "blockSize " << blockSize << std::endl;

		long long oldPos = 0;
		long long totalNumberOfItems = 0;

		for (int i = 0; i < (k - 1); ++i)
		{
			size_t nowSize = 0;
			std::vector<T> v;
			v.clear();
			std::cout << "I'm reading piece #" << i << std::endl;
			while (nowSize < blockSize)
			{
				/*
				std::cout << "nowSize " << nowSize << std::endl;
				*/
				reader >> tmpVal;
				v.push_back(tmpVal);
				++totalNumberOfItems;
				/*
				std::cout << tmpVal << std::endl;
				*/
				nowSize = (long long)(reader.tellg()) - oldPos;
			}
			oldPos = nowSize;

			std::cout << "I'm sorting piece #" << i << std::endl;
			sorter(v.begin(), v.end());
			std::cout << "I've sorted piece #" << i << std::endl;
			for (int j = 0; j < v.size(); ++j)
				tmpStream[i] << tmpSeparator << v[j];
			std::cout << "I've writed piece #" << i << std::endl;
		}

		std::vector<T> v;
		v.clear();
		std::cout << "I'm reading piece #" << k << std::endl;
		while (!reader.eof())
		{
			reader >> tmpVal;
			v.push_back(tmpVal);
			++totalNumberOfItems;
		}
		std::cout << "I'm sorting piece #" << k << std::endl;
		sorter(v.begin(), v.end());
		std::cout << "I've sorted piece #" << k << std::endl;
		for (int j = 0; j < v.size(); ++j)
			tmpStream[k - 1] << tmpSeparator << v[j];
		std::cout << "I've writed piece #" << k << std::endl;

		for (int i = 0; i < k; ++i)
			tmpStream[i].seekg(0, tmpStream[i].beg);
		
		std::cout << "I'm merging" << std::endl;
		std::vector <std::pair<T, int> > mergeHeap;
		for (int i = 0; i < k; ++i)
		{
			std::pair <T, int> tmp;
			tmpStream[i] >> tmp.first;
			--totalNumberOfItems;
			tmp.second = i;
			mergeHeap.push_back(tmp);

			/*
			std::cout << tmp.second << " " << tmp.first << std::endl;
			*/
		}
		///\warning swap должен быть определён для T
		///\warning mergeCmp сравнивает пары <T, int>
		///\todo написать так, чтобы компаратор для типа T превращался
		///в компаратор для пары
		std::make_heap(mergeHeap.begin(), mergeHeap.end(), mergecmp);
		while (totalNumberOfItems > 0)
		{
			writer << mergeHeap.front().first << separator;
			if (!tmpStream[mergeHeap.front().second].eof())
			{
				std::pair <T, int> tmp;
				int iNowFile = mergeHeap.front().second;		
				tmpStream[iNowFile] >> tmp.first;
				tmp.second = iNowFile;
				--totalNumberOfItems;
				/*
				std::cout << tmp.second << " " << tmp.first << std::endl;
				*/

				mergeHeap.push_back(tmp);
				std::push_heap(mergeHeap.begin(),mergeHeap.end(), mergecmp);

				/*
				std::cout << "Now state of Heap" << std::endl;
				std::cout << "min " << mergeHeap.front().second << " "
				                    << mergeHeap.front().first << std::endl;
				std::cout << "elements" << std::endl;
				for (int i = 0; i < mergeHeap.size(); ++i)
					std::cout << mergeHeap[i].second << " "
				 			  << mergeHeap[i].first << std::endl;
				std::cout << "=======" << std::endl;
				*/
			}
			std::pop_heap(mergeHeap.begin(),mergeHeap.end(), mergecmp);
			mergeHeap.pop_back();
			
			/*
			std::cout << "Now state of Heap" << std::endl;
			std::cout << "min " << mergeHeap.front().second << " "
			                    << mergeHeap.front().first << std::endl;
			std::cout << "elements" << std::endl;
			for (int i = 0; i < mergeHeap.size(); ++i)
				std::cout << mergeHeap[i].second << " "
			 			  << mergeHeap[i].first << std::endl;
			std::cout << "=======" << std::endl;
			*/

			//волшебная константа, благодаря которой разговариваем с юзером
			if (totalNumberOfItems % 1000000 == 0)
				std::cout << totalNumberOfItems << " remaining" << std::endl;
		}
		std::sort_heap(mergeHeap.begin(), mergeHeap.end(), mergecmp);
		for (int i = 0; i < mergeHeap.size(); ++i)
			writer << mergeHeap[i].first << separator;
	}
};