
#include "Swapper.h"

#include <iostream>
using namespace std;

Swapper::Swapper(Memory* memory, SwapArea* swap_area) {
    this->memory = memory;
    this->swap_area = swap_area;
}

void Swapper::swapIn(Page* page) {
    int swap_number = page->getSwapAddress();
    swap_area->removePage(page);
    int frame_number = memory->getFreeFrameIndex();
    if (frame_number != -1) {
        memory->setPageToFrame(frame_number, page);
        cout << "Se localizo pagina " << page->getPageNumber() << " del proceso " << page->getProcessId() << " en el marco "
             << swap_number << " del area de swapping y se cargo al marco " << frame_number << " de la memoria real.\n";
    }
}

FIFOSwapper::FIFOSwapper(Memory* memory, SwapArea* swap_area) : Swapper(memory, swap_area) {}

Page* FIFOSwapper::swapOut() {
    vector<Frame>& frames = memory->getFrames();
    int first_inserted_frame, first_inserted_time;

    // Itera por los frames de memoria para buscar la página que se insertó primero
    for (int i = 0; i < frames.size(); i++) {
        Frame& frame = frames[i];
        if (frame.isFree()) continue;
        int curr_alloc_time = frame.getAllocationTime();

        // Si no se ha registrado un frame o el frame entró a memoria antes que el frame registrado
        if (i == 0 || curr_alloc_time < first_inserted_time) {
            first_inserted_frame = i;
            first_inserted_time = curr_alloc_time;
        }
    }
    Page* swapped_page = frames[first_inserted_frame].getPage();
    memory->freeFrame(swapped_page->getFrameNumber());
    swap_area->insertPage(swapped_page);

    cout << "Pagina " << swapped_page->getPageNumber() << " del proceso " << swapped_page->getProcessId() << " swappeada al marco " << swapped_page->getSwapAddress() << " del area de swapping.\n";
    return swapped_page;
}

LRUSwapper::LRUSwapper(Memory* memory, SwapArea* swap_area) : Swapper(memory, swap_area) {}

Page* LRUSwapper::swapOut() {
    vector<Frame>& frames = memory->getFrames();
    int least_used_frame, least_used_time;

    // Itera por los frames de memoria para buscar la página que se usó
    for (int i = 0; i < frames.size(); i++) {
        Frame& frame = frames[i];
        if (frame.isFree()) continue;
        int curr_last_use = frame.getPage()->getLastUse();

        // Si no se ha registrado un frame o el frame se usó antes que el frame registrado
        if (i == 0 || curr_last_use < least_used_time) {
            least_used_frame = i;
            least_used_time = curr_last_use;
        }
    }

    Page* swapped_page = frames[least_used_frame].getPage();
    memory->freeFrame(swapped_page->getFrameNumber());
    swap_area->insertPage(swapped_page);

    cout << "Pagina " << swapped_page->getPageNumber() << " del proceso " << swapped_page->getProcessId() << " swappeada al marco " << swapped_page->getSwapAddress() << " del area de swapping.\n";
    return swapped_page;
}