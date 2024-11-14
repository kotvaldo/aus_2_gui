#include "Parcela.h"
#include "Nehnutelnost.h"
#include <algorithm>

void Parcela::removeNehnutelnost(Nehnutelnost* nehnut) {
    auto it = std::find(nehnutelnosti.begin(), nehnutelnosti.end(), nehnut);
    if (it != nehnutelnosti.end()) {
        nehnutelnosti.erase(it);
        nehnut->removeParcela(this);
    }
}

void Parcela::clearNehnutelnosti() {
    for (Nehnutelnost* nehnut : nehnutelnosti) {
        nehnut->removeParcela(this);
    }
    nehnutelnosti.clear();
}

