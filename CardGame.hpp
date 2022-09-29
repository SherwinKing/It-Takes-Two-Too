#include <set>
#include "ChoiceDialog.hpp"

class CardGameChoiceDialog : public ChoiceDialog {
public:
    CardGameChoiceDialog(std::ifstream & file) : ChoiceDialog(file) {}

    CardGameChoiceDialog() : ChoiceDialog() {}

    virtual uint32_t get_next_dialog_id(uint32_t choice) override {
        return 0;
    }

    inline void set_choice_to_next_dialog_map(std::map<uint32_t, uint32_t> choice_to_next_dialog_map) {
        this->choice_to_next_dialog_map = choice_to_next_dialog_map;
    }

    inline void set_choice_text_map(std::map<uint32_t, std::string> choice_text_map) {
        this->choice_text_map = choice_text_map;
    }

    inline void set_text_paragraphs(std::vector<TextParagraph> text_paragraphs) {
        this->text_paragraphs = text_paragraphs;
    }
};

enum GameState {
    A_PREPARE,
    A_TURN,
    B_PREPARE,
    B_TURN,
    ROUND_RESULT,
    GAME_RESULT
};

enum RoundResult {
    A_WIN,
    B_WIN,
    DRAW
};

class CardGame {
protected:
    GameState game_state;
    CardGameChoiceDialog dialog = CardGameChoiceDialog();

    void update_dialog();
    void transition_to_next_state();

public:
    CardGame();
    std::set<std::uint32_t> A_cards_remaining = {1, 2, 3, 4, 5};
    std::set<std::uint32_t> B_cards_remaining = {1, 2, 3, 4, 5};
    std::vector<RoundResult> round_results;


    uint32_t round_id = 0;

    uint32_t A_score = 0;
    uint32_t B_score = 0;

    uint32_t A_card_placing;
    uint32_t B_card_placing;

    bool A_first = true;

    void handle_choice(uint32_t choice);

    bool A_play_card(uint32_t card_id);
    bool B_play_card(uint32_t card_id);
    bool finish_round();

    bool is_active = false;

    inline CardGameChoiceDialog & get_current_dialog() {
        return dialog;
    }
};