#ifndef DIGITAL_SORTER

#define DIGITAL_SORTER 

#include <memory.h>
#include <vector>
#include <iostream>

template<class T> class IntegerDigitExtractor {
    public:
        int getCntDigits(T number){
            if (number == 0)
                return 1;
            if (number < 0)
                number = -number;
            int cnt_digits = 0;
            while (number > 0){
                number /= MOD;
                cnt_digits++;
            }
            return cnt_digits;
        }

        int getDigit(T number, int pos){
            if (pos >= getCntDigits(number))
                return MOD;
            //std::cout << number;
            bool is_negative = false;
            if (number < 0){
                is_negative = true;
                number = -number;
            }
            for (int i = 0; i < pos; i++)
                number /= MOD;
            int digit = number % MOD;
            if (is_negative)
                digit = -digit;
            //std::cout << ' ' << pos <<  ' ' << digit << "\n";
            return digit + MOD;
        }

    private:
        static const unsigned int MOD = 1 << 16;
};

template<class T, class DigitExtractor> class DigitalSorter {
    public:
        void operator() (std::vector<T> &to_sort){
            int cnt_digits = 0;
            for (std::vector<int>::iterator it = to_sort.begin(); 
                    it != to_sort.end(); it++)
                cnt_digits = 
                    std::max(cnt_digits, extractor.getCntDigits(*it));
            for (int number = 0; number < cnt_digits; number++){
                sortByDigit(number, to_sort);
            }
        }

        void sortByDigit(int digit_number, std::vector<T> &to_sort){
            memset(cnt, 0, sizeof(cnt));
            for (std::vector<int>::iterator it = to_sort.begin(); 
                    it != to_sort.end(); it++)
                cnt[extractor.getDigit(*it, digit_number)]++;
            start[0] = 0;
            for (int i = 0; i < (int)MAX_DIGIT; i++)
                start[i] = start[i - 1] + cnt[i - 1];
            std::vector<T> buf(to_sort.size());
            for (std::vector<int>::iterator it = to_sort.begin(); 
                    it != to_sort.end(); it++){
                int digit = extractor.getDigit(*it, digit_number);
                buf[start[digit]] = *it;
                start[digit]++;
            }
            to_sort = buf;
        }

    private:
        static const unsigned int MAX_DIGIT = (1 << 17) + 2;
        DigitExtractor extractor;
        int cnt[MAX_DIGIT];
        int start[MAX_DIGIT];
};

#endif
