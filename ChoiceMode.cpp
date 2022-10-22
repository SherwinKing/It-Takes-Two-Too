#include "ChoiceMode.hpp"
#include "GL.hpp"

Load<ImageData> background_image_load(
    LoadTagDefault,
    []() -> ImageData * {
        // std::ifstream background_image_file(data_path("/background.png"), std::ios::binary);
        // unsigned int width, height;
        // load_png(data_path("/background.png"), &width, &height, &image_data_ptr->pixels, LowerLeftOrigin);
        ImageData * image_data_ptr = new ImageData();
        load_png(data_path("/background.png"), &image_data_ptr->size, &image_data_ptr->pixels, LowerLeftOrigin);
        // image_data_ptr->size = glm::uvec2(width, height);
        return image_data_ptr;
    }
);

Load< std::vector<ChoiceDialog> > choice_dialog_vector_load(
    LoadTagDefault,
    []() -> std::vector<ChoiceDialog> * {
        std::vector<ChoiceDialog> * choice_dialog_vector_ptr = new std::vector<ChoiceDialog>();
        std::ifstream dialog_metadata_file(data_path("/dialogs/metadata.txt"));
        std::string dialog_filepath;
        
        while (std::getline(dialog_metadata_file, dialog_filepath)) {
            std::ifstream dialog_file(data_path(dialog_filepath));
            choice_dialog_vector_ptr->push_back(ChoiceDialog(dialog_file));
        }
        return choice_dialog_vector_ptr;
    }
);

ChoiceMode::ChoiceMode() : 
    choice_dialogs(std::move(*choice_dialog_vector_load)) {
        current_dialog_id = 0;
        current_dialog_ptr = &choice_dialogs[current_dialog_id];
    }

void ChoiceMode::go_to_next_dialog(uint32_t choice) {
    if (choice == 0)
        return;

    if (card_game.is_active) {
        current_dialog_ptr = &card_game.get_current_dialog();
        return;
    }

    try {
        current_dialog_id = current_dialog_ptr->get_next_dialog_id(choice);
    } catch (std::out_of_range & e) {
        // do nothing
        return;
    }

    // special game state dialog
    if (current_dialog_id == 100) {
        // TODO: implement
        card_game.is_active = true;
        current_dialog_ptr = &card_game.get_current_dialog();
        return;
    } else {
        current_dialog_ptr = &choice_dialogs[current_dialog_id];
    }
    return;
}

void ChoiceMode::handle_choice(uint32_t choice) {
    if (card_game.is_active) {
        card_game.handle_choice(choice);
        return;
    }
}

bool ChoiceMode::handle_event(SDL_Event const & event, glm::uvec2 const &window_size_input) {
    resize(window_size_input);
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_1) {
            one.pressed = true;
        } else if (event.key.keysym.sym == SDLK_2) {
            two.pressed = true;
        } else if (event.key.keysym.sym == SDLK_3) {
            three.pressed = true;
        } else if (event.key.keysym.sym == SDLK_4) {
            four.pressed = true;
        } else if (event.key.keysym.sym == SDLK_5) {
            five.pressed = true;
        } else if (event.key.keysym.sym == SDLK_SPACE) {
            space.pressed = true;
        }
        return true;
    }
    return false;
}

void ChoiceMode::update(float elapsed) {
    if (one.pressed) {
        if (current_dialog_ptr->activate_choice(1))
            current_choice = 1;
    } else if (two.pressed) {
        if (current_dialog_ptr->activate_choice(2))
            current_choice = 2;
    } else if (three.pressed) {
        if (current_dialog_ptr->activate_choice(3))
            current_choice = 3;
    } else if (four.pressed) {
        if (current_dialog_ptr->activate_choice(4))
            current_choice = 4;
    } else if (five.pressed) {
        if (current_dialog_ptr->activate_choice(5))
            current_choice = 5;
    } else if (space.pressed) {
        // if choice is invalid, do nothing
        handle_choice(current_choice);
        go_to_next_dialog(current_choice);
        current_choice = 0;
    }

    //reset button presses:
    one.pressed = false;
    two.pressed = false;
    three.pressed = false;
    four.pressed = false;
    five.pressed = false;
    space.pressed = false;
}
void ChoiceMode::draw(glm::uvec2 const &drawable_size) {
    // Eye protecting warm yellow background
    glClearColor(1, 1, 0.9, 0.5);
    glClear(GL_COLOR_BUFFER_BIT);

    //draw background
    image_renderer.render_image(*background_image_load, 0, 0);

    //render the current dialog
    current_dialog_ptr->render(text_renderer);
}
