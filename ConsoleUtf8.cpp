// ConsoleUtf8.cpp
// Весь Windows-специфичный код изолирован в этом файле: <windows.h> подключается здесь
// и больше нигде, поэтому его макросы и имена (min, max, byte) не могут конфликтовать
// с остальным проектом.
#include "ConsoleUtf8.h"

#ifdef _WIN32
#define NOMINMAX              // не определять макросы min/max
#define WIN32_LEAN_AND_MEAN   // не подключать лишнее (в т.ч. rpcndr.h с типом byte)
#include <windows.h>
#endif

void enableUtf8Console() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);   // кодовая страница UTF-8
#endif
}
