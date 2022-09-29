#pragma once
#include <unordered_map>
#include <map>
#include <string>
#include <fstream>
#include "TextParagraph.hpp"
#include "TextRenderer.hpp"

class ChoiceDialog {
protected:
    uint32_t font_size = 30;
    uint32_t id;
    std::map<uint32_t, uint32_t> choice_to_next_dialog_map;
    std::map<uint32_t, std::string> choice_text_map;
    std::vector<TextParagraph> text_paragraphs;

    uint32_t current_choice_active = 0;

public:
    ChoiceDialog(std::ifstream & file);

    ChoiceDialog();
    // ChoiceDialog(std::unordered_map<uint32_t, uint32_t> && choice_to_next_dialog_map,
    //              std::unordered_map<uint32_t, std::string> && choice_text_map,
    //              std::vector<TextParagraph> && text_paragraphs);

    inline virtual bool activate_choice(uint32_t choice) {
        if (choice_to_next_dialog_map.find(choice) == choice_to_next_dialog_map.end()) {
            return false;
        }
        current_choice_active = choice;
        return true;
    }

    inline virtual uint32_t get_current_choice_active() {
        return current_choice_active;
    }

    virtual uint32_t get_next_dialog_id(uint32_t choice);

    void render(TextRenderer & text_renderer);
};