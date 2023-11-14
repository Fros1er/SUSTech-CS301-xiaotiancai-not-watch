#include "button.hpp"

uint32_t button_last_updated[BUTTON_LEN];

template <size_t I, size_t N>
static void button_update_each(uint32_t now) {
    if (button_last_updated[I] && now - button_last_updated[I] > BUTTON_ANTISHAKE_DELAY) {
        button_last_updated[I] = 0;
        button_cb<(Button)I>();
    }
    if constexpr (I < N - 1) {
        button_update_each<I + 1, N>(now);
    }
}

void button_init() {
    for (int i = 0; i < BUTTON_LEN; i++) {
        button_last_updated[i] = 0;
    }
}

void button_update() {
    button_update_each<0, BUTTON_LEN>(HAL_GetTick());
}
