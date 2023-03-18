//
// Created by bjo on 3/17/23.
//

#include "ui/page.hpp"
namespace UI::Widgets {
    void Page::setFocus(bool f) {
        if (f) {
            if (current_child>0) {
                if (childs[0] != nullptr)
                    childs[0]->setFocus(true);
                for (uint8_t i = 1; i <= current_child; i++)
                    if (childs[i] != nullptr)
                        childs[i]->setFocus(false);
            }
        } else {
            if (current_child>0) {
                for (uint8_t i = 0; i <= current_child; i++)
                    if (childs[i] != nullptr)
                        childs[i]->setFocus(f);
            }
        }
    }

    void Page::setVisible(bool f) {
        visible = f;
        for (uint8_t i = 0; i <= current_child; i++)
            if (childs[i] != nullptr)
                childs[i]->setVisible(f);
    }
}
