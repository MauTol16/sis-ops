#include "SwapArea.h"

SwapArea::SwapArea() {
    pages = std::vector<Page *>(0);
}

std::vector<Page *> &SwapArea::getPages() {
    return pages;
}

void SwapArea::insertPage(Page *page) {
    // Este if checa si el Swap Area esta lleno.
    if (pages.size() >= 256) {
        // No agrega el apuntador de la pagina.
        // Deberia mostrar algun tipo de error.
        return;
    }

    // No esta lleno, inserta la nueva pagina.
    pages.push_back(page);
    page->setSwapAddress(pages.size() - 1);
}

Page *SwapArea::findPage(int process_id, int virtual_address) {
    // Recorre el vector que contiene las paginas.
    for (Page *page : pages) {
        // Encuentra y regresa la pagina si el process_id coincide y si el virtual address esta en el rango correcto.
        if (page->getProcessId() == process_id && virtual_address >= page->getVirtualAddressStart() && virtual_address < page->getVirtualAddressStart() + 16) {
            return page;
        }
    }

    // Regresa nullptr si no encuentra la pagina correcta.
    return nullptr;
}

void SwapArea::removePage(Page *page) {
    // Busca la pagina `page` y le asigna el apuntador resultado a la variable temp.
    Page *temp = findPage(page->getProcessId(), page->getVirtualAddressStart());

    // Itera el vector de paginas y borra la page del vector.
    for (int i = 0; i < pages.size(); i++) {
        if (pages[i] == temp) {
            pages.erase(pages.begin() + i);
            break;
        }
    }
}
