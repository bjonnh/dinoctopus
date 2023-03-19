//
// Created by bjo on 3/17/23.
//

#include "ui/page.hpp"
#include "ui/ui.hpp"

namespace UI::Widgets {
    template class Page<UI_TYPE>;

    template<typename D>
    void Page<D>::setFocus(bool f) {
        if (f) {
            if (this->current_child > 0) {
                if (this->childs[0] != nullptr)
                    this->childs[0]->setFocus(true);
                for (uint8_t i = 1; i <= this->current_child; i++)
                    if (this->childs[i] != nullptr)
                        this->childs[i]->setFocus(false);
            }
        } else {
            if (this->current_child > 0) {
                for (uint8_t i = 0; i <= this->current_child; i++)
                    if (this->childs[i] != nullptr)
                        this->childs[i]->setFocus(f);
            }
        }
    }

    template<typename D>
    void Page<D>::setVisible(bool f) {
        this->visible = f;
        for (uint8_t i = 0; i <= this->current_child; i++)
            if (this->childs[i] != nullptr)
                this->childs[i]->setVisible(f);
    }
}
