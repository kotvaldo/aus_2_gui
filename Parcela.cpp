#include "Parcela.h"
#include "Nehnutelnost.h"
#include <algorithm>

void Parcela::removeNehnutelnost(Nehnutelnost* nehnut) {
    auto it = std::find(nehnutelnosti.begin(), nehnutelnosti.end(), nehnut);
    if (it != nehnutelnosti.end()) {
        nehnutelnosti.erase(it);  // Odstráni Nehnutelnost z listu nehnutelnosti
        nehnut->removeParcela(this);  // Obojsmerne zruší prepojenie v Nehnutelnost
    }
}

void Parcela::clearNehnutelnosti() {
    for (Nehnutelnost* nehnut : nehnutelnosti) {
        nehnut->removeParcela(this);  // Odstráni referenciu na túto parcelu v každej Nehnutelnost
    }
    nehnutelnosti.clear();  // Vyprázdnime zoznam nehnutelností
}

