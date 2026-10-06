#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_sprite_ptr.h"
#include "graphics/nyan_sprite.h"

int main()
{
    // Инициализируем ядро Game Boy Advance
    bn::core::init();

    // Начальные координаты Нян Кэта на экране (разрешение GBA 240x160)
    bn::fixed cat_x = 0;
    bn::fixed cat_y = 0;
    bn::fixed velocity_y = 0;
    bool is_jumping = false;

    // 🥛 Логика комбо и бутылок молока
    int milk_combo = 0;              // Счётчик комбо
    bn::fixed milk_bottle_x = 60;    // Позиция первой бутылки молока по X
    bn::fixed milk_bottle_y = 20;    // Позиция первой бутылки молока по Y
    bool milk_visible = true;        // Статус бутылки (активна/съедена)

    while(true)
    {
        // 🕹 Считываем нажатия кнопок на GBA (или DualShock в эмуляторе)
        if(bn::keypad::held(bn::keypad::key_type::LEFT))
        {
            cat_x -= 2; // Бежим влево
        }
        else if(bn::keypad::held(bn::keypad::key_type::RIGHT))
        {
            cat_x += 2; // Бежим вправо
        }

        // Прыжок на кнопку А
        if(bn::keypad::pressed(bn::keypad::key_type::A) && !is_jumping)
        {
            velocity_y = -6; // Сила толчка вверх
            is_jumping = true;
        }

        // Простая физика гравитации 16-битного платформера
        if(is_jumping)
        {
            cat_y += velocity_y;
            velocity_y += 0.3; // Гравитация тянет вниз

            // Земля на уровне y = 40 пикселей
            if(cat_y >= 40)
            {
                cat_y = 40;
                is_jumping = false;
                velocity_y = 0;
            }
        }

        // 🥛 Проверка хитбокса: если Нян Кэт касается активной бутылки молока
        if(milk_visible && (cat_x >= milk_bottle_x - 12 && cat_x <= milk_bottle_x + 12) 
                        && (cat_y >= milk_bottle_y - 12 && cat_y <= milk_bottle_y + 12))
        {
            milk_visible = false;   // Бутылка исчезает
            milk_combo += 1;        // Комбо увеличивается на +1!
            
            // Логика спавна новой бутылки в случайном месте для бесконечного комбо
            milk_bottle_x = -80 + (milk_combo * 20) % 160; 
            milk_bottle_y = 10 + (milk_combo * 15) % 40;
            milk_visible = true;    // Появляется новая бутылка
        }

        // Обновляем экран GBA (60 кадров в секунду)
        bn::core::update();
    }
}
