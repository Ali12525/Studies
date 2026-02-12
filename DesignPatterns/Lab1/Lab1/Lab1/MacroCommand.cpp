#include "MacroCommand.h"

// Выполняет все команды макроса в порядке их добавления.
void MacroCommand::Execute(AppState& state, HWND hwnd) {
    for (auto& c : children) c->Execute(state, hwnd);
}

// Отменяет все команды макроса в обратном порядке.
void MacroCommand::Unexecute(AppState& state, HWND hwnd) {
    for (auto it = children.rbegin(); it != children.rend(); ++it)
        (*it)->Unexecute(state, hwnd);
}

// Создаёт глубокую копию макроса (клонирует все дочерние команды).
std::unique_ptr<ICommand> MacroCommand::Clone() const {
    auto mc = std::make_unique<MacroCommand>();
    for (const auto& c : children)
        mc->children.push_back(c->Clone());
    return mc;
}