#include "Nehnutelnost.h"
#include "Parcela.h"  // Skutočné zahrnutie Parcela.h v .cpp súbore
#include <algorithm>


void Nehnutelnost::removeParcela(Parcela* parcela) {
    auto it = std::find(parcely.begin(), parcely.end(), parcela);
    if (it != parcely.end()) {
        parcely.erase(it);  // Odstráni Parcela z listu parcely
        parcela->removeNehnutelnost(this);  // Obojsmerne zruší prepojenie v Parcela
    }
}

void Nehnutelnost::clearParcely() {
    for (Parcela* parcela : parcely) {
        parcela->removeNehnutelnost(this);
    }
    parcely.clear();
}
