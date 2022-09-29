#include "ChoiceDialog.hpp"

ChoiceDialog::ChoiceDialog(std::ifstream & file) {

    // Similar way of loading assets as game1
    std::string line;
    // first line: dialog id
    std::getline(file, line);
    id = std::stoi(line);
    // second line: number of choices
    std::getline(file, line);
    uint32_t choice_num = std::stoi(line);

    // parse choices
    for (uint32_t i = 0; i < choice_num; i++) {
        std::getline(file, line);
        std::stringstream line_sstream(line);
        uint32_t choice;
        uint32_t next_dialog_id;
        std::string choice_text;
        line_sstream >> choice >> next_dialog_id;
        std::getline(line_sstream, choice_text);

        choice_to_next_dialog_map[choice] = next_dialog_id;
        choice_text_map[choice] = choice_text;
    }

    // parse text paragraphs
    while (std::getline(file, line)) {
        if (line.size() == 0) {
            continue;;
        }
        // Currently font size and color are fixed
        text_paragraphs.push_back(TextParagraph(line, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), font_size));
    }
}

ChoiceDialog::ChoiceDialog() {}

uint32_t ChoiceDialog::get_next_dialog_id(uint32_t choice) {
    if (choice_to_next_dialog_map.find(choice) == choice_to_next_dialog_map.end()) {
        throw std::runtime_error("ChoiceDialog::get_next_dialog_id: Invalid choice.");
    }
    return choice_to_next_dialog_map[choice];
}

void ChoiceDialog::render(TextRenderer & text_renderer) {
    float x = -0.8f;
    float y = 0.8f;
    for (TextParagraph & paragraph : text_paragraphs) {
        y = paragraph.render(x, y, text_renderer);
        y -= text_renderer.get_line_height_by_font_size(font_size);
    }
    y -= text_renderer.get_line_height_by_font_size(font_size);
    
    for (auto & choice : choice_text_map) {
        if (choice.first == current_choice_active) {
            text_renderer.render_text(choice.second, x, y, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), font_size);
        } else {
            text_renderer.render_text(choice.second, x, y, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), font_size);
        }
        y -= text_renderer.get_line_height_by_font_size(font_size);
    }
}