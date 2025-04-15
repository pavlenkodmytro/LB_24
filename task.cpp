#include <iostream>  // Підключаємо інструмент для базікання з консоллю (cin, cout)
#include <string>    // Вмикаємо роботу з текстом (рядками)
#include <numeric>   // Тут живе готова магічна штука std::gcd (треба C++17!)
#include <cmath>     // Беремо звідси функцію abs, щоб робити числа добрими (невід'ємними)
#include <stdexcept> // Потрібно, щоб кидатися помилками, якщо щось не так
#include <limits>    // Для перевірки на переповнення (хоча ми її не робимо в порівнянні)

// Кажемо: "Друже компілятор, не змушуй мене писати std:: перед усім."
using namespace std;

// --- Креслення для створення супер-об'єктів "Дріб" ---
class Fraction {
private: // Секретна зона: сюди зась стороннім!
    long long numerator;   // Чисельник (верх). long long - для більших чисел.
    long long denominator; // Знаменник (низ). Теж великий про всяк випадок.

    // Метод "Внутрішній перфекціоніст" - приводить дріб до ладу
    void reduce() {
        // Якщо знаменник 0 - це біда, ловимо в конструкторі.
        if (denominator == 0) return; // Просто про всяк випадок.

        // Нуль - це просто 0/1.
        if (numerator == 0) {
            denominator = 1;
            return;
        }

        // Шукаємо найбільший спільний дільник (НСД) магією std::gcd
        long long common_divisor = std::gcd(std::abs(numerator), std::abs(denominator));

        // Якщо НСД не нуль (майже завжди, крім випадку 0/0, який ми обробили)
        if (common_divisor != 0) {
             numerator /= common_divisor;   // Скорочуємо верх
             denominator /= common_divisor; // Скорочуємо низ
        }

        // Мінус має жити тільки зверху!
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
    } // Кінець прибирання

public: // Публічна зона: кнопки та екранчики для користування

    // "Фабрика дробів" (Конструктор) - створює новий дріб.
    Fraction(long long num = 0, long long den = 1) : numerator(num), denominator(den) {
        // СВЯТА ПЕРЕВІРКА! Знаменник НЕ НУЛЬ!
        if (den == 0) {
            // Кидаємо помилку! Хай знають!
            // Оскільки в main НЕМАЄ try-catch, програма тут впаде з незрозумілою помилкою типу "terminate called after throwing an instance..."
            throw std::invalid_argument("Знаменник нуль? Ти серйозно?!");
        }
        // Одразу прибираємось
        reduce();
    }

    // "Перекладач на людську мову" (текст)
    string toString() const { // const - обіцянка нічого не міняти
        if (denominator == 1) return to_string(numerator); // Ціле число
        if (numerator == 0) return "0"; // Нуль

        string sign = "";
        long long num_abs = numerator;
        if (numerator < 0) {
            sign = "-";
            num_abs = -num_abs;
        }

        long long whole_part = num_abs / denominator;
        long long fraction_part = num_abs % denominator;

        string result = sign;
        if (whole_part > 0) {
            result += to_string(whole_part);
            if (fraction_part > 0) {
                result += " " + to_string(fraction_part) + "/" + to_string(denominator);
            }
        } else if (fraction_part > 0) {
             result += to_string(fraction_part) + "/" + to_string(denominator);
        }
        return result;
    }

    // "Перекладач на комп'ютерну мову" (число з комою)
    double toDouble() const {
        return static_cast<double>(numerator) / denominator;
    }

    // "Сирий вигляд" - просто "число/число"
    string getRawString() const {
         return std::to_string(numerator) + "/" + std::to_string(denominator);
     }


    // --- Арифметичні Суперсили ---
    // Створюють НОВИЙ дріб-результат! (Ми їх не використовуємо в цьому main, але вони є)

    Fraction plus(const Fraction& that) const {
        long long new_num = numerator * that.denominator + that.numerator * denominator;
        long long new_den = denominator * that.denominator;
        return Fraction(new_num, new_den);
    }

    Fraction minus(const Fraction& that) const {
        long long new_num = numerator * that.denominator - that.numerator * denominator;
        long long new_den = denominator * that.denominator;
        return Fraction(new_num, new_den);
    }

    Fraction times(const Fraction& that) const {
        long long new_num = numerator * that.numerator;
        long long new_den = denominator * that.denominator;
        return Fraction(new_num, new_den);
    }

    Fraction by(const Fraction& that) const {
        if (that.numerator == 0) { // Перевірка ділення на нуль!
            // Теж кидаємо помилку, яка призведе до падіння програми
            throw std::invalid_argument("Ділити на нуль?! Геть звідси!");
        }
        long long new_num = numerator * that.denominator;
        long long new_den = denominator * that.numerator;
        return Fraction(new_num, new_den);
    }

    // --- НОВІ Суперсили: Порівняння! ---
    // Відповідають "так" (true) або "ні" (false)

    // "Чи я більший за отой дріб 'that'?"
    bool isGreaterThan(const Fraction& that) const {
        // Хитрий трюк: порівнюємо "навхрест" a/b > c/d  <=>  a*d > c*b
        return static_cast<long long>(this->numerator) * that.denominator > static_cast<long long>(that.numerator) * this->denominator;
    }

    // "Чи я менший за отой дріб 'that'?"
    bool isLessThan(const Fraction& that) const {
        // Так само навхрест: a/b < c/d  <=>  a*d < c*b
        return static_cast<long long>(this->numerator) * that.denominator < static_cast<long long>(that.numerator) * this->denominator;
    }

    // "Чи ми з дробом 'that' однаковісінькі?"
    bool isEqual(const Fraction& that) const {
        // Оскільки ми *завжди* скорочуємо дроби, можна просто порівняти компоненти
        return this->numerator == that.numerator && this->denominator == that.denominator;
    }

}; // Кінець креслення класу Fraction

// --- Головний Режисер Шоу (функція main) ---
int main() {
    long long num1, den1, num2, den2; // Коробки для чисел
    char slash; // Коробка для '/'

    // УВАГА! Блок try-catch видалено за запитом.
    // Програма впаде, якщо ввести некоректні дані (наприклад, знаменник 0).

    cout << "Введи перший дріб (типу 3/4): ";
    cin >> num1 >> slash >> den1; // Читаємо

    cout << "Тепер другий дріб (типу 1/3): ";
    cin >> num2 >> slash >> den2; // Читаємо

    // Створюємо два об'єкти-дроби.
    // ПАДІННЯ ТУТ, якщо знаменник був 0!
    Fraction f1(num1, den1);
    Fraction f2(num2, den2);

    cout << "\nПорівнюємо " << f1.getRawString() << " та " << f2.getRawString() << "...\n" << endl;

    // --- Використовуємо Сили Порівняння ---
    // Перевіряємо, хто більший, менший чи рівний
    if (f1.isGreaterThan(f2)) {
        // Якщо f1 > f2
        cout << f1.getRawString() << " > " << f2.getRawString() << endl;
    } else if (f1.isLessThan(f2)) {
        // Якщо f1 < f2
        cout << f1.getRawString() << " < " << f2.getRawString() << endl;
    } else {
        // Якщо вони рівні
        cout << f1.getRawString() << " = " << f2.getRawString() << endl;
    }

    // Якщо ми тут - значить, все пройшло без падінь.
    return 0; // Все чікі-пукі!
}
