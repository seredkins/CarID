ЧТОБЫ ЗАПУСТИТЬ ПРИЛОЖЕНИЕ НУЖНА БИБЛИОТЕКА QT ДЛЯ ПРОСМОТРА В Qt Creator ОТКРОЙТЕ ФАЙЛ .pro И НАЖМИТЕ "НАСТРОИТЬ ПРОЕКТ"

ВЕРСИЯ BETA -- НУЖНЫ ДОРАБОТКИ И ИСПРАВЛЕНИЯ

ССЫЛКА НА ФОТОГРАФИИ ЦИФР И ТЕСТЫ:

Чтобы испытать:

Скачайте файл по ссылке
В файле mainwindow.cpp проекта в функции detect() переопределите переменную pathToDigitsFolder Укажите свой путь к папке Digits в скачанном файле
Откройте любую картинку из папки Plates скачанного файла
Нажмите "Detect"
Это приложение определяет автомобильный номер по картинке Оно использует сохраненные изображения цифр разных размеров и с разным освещением Приложение пробует изменить размер каждой цифры, накладывает получившуюся картинку на каждый пиксель изображения Затем считает специальную метрику -- SSD(Sum of Squared Divisions)
В зависимости от ее значения принимает решение: есть ли тут цифра или нет
