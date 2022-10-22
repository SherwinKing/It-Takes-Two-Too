#pragma once
#include "Mode.hpp"
#include "ChoiceDialog.hpp"
#include "CardGame.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"
#include "ImageRenderer.hpp"
#include "load_save_png.hpp"


const std::string font_file_path_from_dist = "font/Wellfleet/Wellfleet-Regular.ttf";

class ChoiceMode: public Mode {
protected:
    //input tracking:
    struct Button {
        bool pressed = false;
    } one, two, three, four, five, space;

    // Game state
    uint32_t round_id = 0;
    uint32_t current_choice = 0;
    std::vector<ChoiceDialog> choice_dialogs;

    uint32_t current_dialog_id = 0;
    ChoiceDialog * current_dialog_ptr;


    TextRenderer text_renderer;
    ImageRenderer image_renderer;
    glm::uvec2 window_size;

    CardGame card_game;

    void handle_choice(uint32_t choice);
    void go_to_next_dialog(uint32_t choice);

public:
    ChoiceMode();
    virtual bool handle_event(SDL_Event const & event, glm::uvec2 const & window_size) override;
    virtual void update(float elapsed) override;
    virtual void draw(glm::uvec2 const &drawable_size) override;

    inline virtual void resize(glm::uvec2 const &window_size_input) {
        window_size = window_size_input;
        image_renderer.resize(window_size[0], window_size[1]);
        text_renderer.resize(window_size[0], window_size[1]);
    }
};