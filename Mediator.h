#include "Nehnutelnost.h"
#include "Parcela.h"
#include <unordered_map>
#include <vector>
#include <algorithm>

class Mediator {
private:
    std::unordered_map<Parcela*, std::vector<Nehnutelnost*>> parcelaToNehnutelnosti;
    std::unordered_map<Nehnutelnost*, std::vector<Parcela*>> nehnutelnostToParcely;

public:
    void link(Parcela* parcela, Nehnutelnost* nehnutelnost) {
        parcelaToNehnutelnosti[parcela].push_back(nehnutelnost);
        nehnutelnostToParcely[nehnutelnost].push_back(parcela);
    }

    void unlink(Parcela* parcela, Nehnutelnost* nehnutelnost) {
        auto& parcelaLinks = parcelaToNehnutelnosti[parcela];
        parcelaLinks.erase(std::remove(parcelaLinks.begin(), parcelaLinks.end(), nehnutelnost), parcelaLinks.end());

        auto& nehnutelnostLinks = nehnutelnostToParcely[nehnutelnost];
        nehnutelnostLinks.erase(std::remove(nehnutelnostLinks.begin(), nehnutelnostLinks.end(), parcela), nehnutelnostLinks.end());
    }

    std::vector<Nehnutelnost*> getLinksFromParcela(Parcela* parcela) const {
        auto it = parcelaToNehnutelnosti.find(parcela);
        return (it != parcelaToNehnutelnosti.end()) ? it->second : std::vector<Nehnutelnost*>{};
    }

    std::vector<Parcela*> getLinksFromNehnutelnost(Nehnutelnost* nehnutelnost) const {
        auto it = nehnutelnostToParcely.find(nehnutelnost);
        return (it != nehnutelnostToParcely.end()) ? it->second : std::vector<Parcela*>{};
    }

};
