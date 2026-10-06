#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_sprite_ptr.h"
#include "graphics/nyan_sprite.h"

int main()
{
    // Инициализируем ядро Game Boy Advance
    bn::core::init();

    // Начальные координаты Нян Кэта на экране (разрешение GBA 240x160)
    fixed cat_x = 0;
    fixed cat_y = 0;
    fixed velocity_y = 0;
    bool is_jumping = false;

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

        // Обновляем экран GBA (60 кадров в секунду)
        bn::core::update();
    }
}
