#include "CardGame.hpp"

CardGame::CardGame() {
    game_state = A_PREPARE;
    update_dialog();
}

void CardGame::update_dialog() {
    std::map<uint32_t, std::string> choice_text_map;
    std::map<uint32_t, uint32_t> choice_to_next_dialog_map;
    std::string round_result_text;
    std::string player_prepare_text;
    switch (game_state) {
        case A_PREPARE:
            dialog.set_choice_text_map({
                {1, "(1)Ready!"}
            });
            dialog.set_choice_to_next_dialog_map({
                {1, 100}
            });
            player_prepare_text = "A's turn. B should avoid watching. Tell me if ready.";
            if (round_id > 0 && A_first) {
                player_prepare_text += " By the way, A may ask B a yes-or-no question now. Remember, each player has one chance to lie";
            }
            dialog.set_text_paragraphs({
                TextParagraph(player_prepare_text)
            });
            break;
        case A_TURN:
            for (auto card : A_cards_remaining) {
                choice_text_map[card] = "(" + std::to_string(card) + ")" + std::to_string(card);
            }
            dialog.set_choice_text_map(choice_text_map);

            for (auto card : A_cards_remaining) {
                choice_to_next_dialog_map[card] = 100;
            }
            dialog.set_choice_to_next_dialog_map(choice_to_next_dialog_map);

            dialog.set_text_paragraphs({
                TextParagraph("A's turn. Choose a card.")
            });
            break;
        case B_PREPARE:
            dialog.set_choice_text_map({
                {1, "(1)Ready!"}
            });
            dialog.set_choice_to_next_dialog_map({
                {1, 100}
            });
            player_prepare_text = "B's turn. A should avoid watching. Tell me if ready.";
            if (round_id > 0 && !A_first) {
                player_prepare_text += " By the way, B may ask A a yes-or-no question now. Remember, each player has one chance to lie";
            }
            dialog.set_text_paragraphs({
                TextParagraph(player_prepare_text)
            });
            break;
        case B_TURN:
            for (auto card : B_cards_remaining) {
                choice_text_map[card] = "(" + std::to_string(card) + ")" + std::to_string(card);
            }
            dialog.set_choice_text_map(choice_text_map);

            for (auto card : B_cards_remaining) {
                choice_to_next_dialog_map[card] = 100;
            }
            dialog.set_choice_to_next_dialog_map(choice_to_next_dialog_map);

            dialog.set_text_paragraphs({
                TextParagraph("B's turn. Choose a card.")
            });
            break;
        case ROUND_RESULT:
            dialog.set_choice_text_map({
                {1, "(1)Next round!"}
            });
            dialog.set_choice_to_next_dialog_map({
                {1, 100}
            });

            if (round_results[round_id] == A_WIN) {
                round_result_text = "A wins this round.";
            } else if (round_results[round_id] == B_WIN) {
                round_result_text = "B wins this round.";
            } else {
                round_result_text = "Draw this round.";
            }
            dialog.set_text_paragraphs({
                TextParagraph("Round result: " + round_result_text)
            });
            break;
        case GAME_RESULT:
            dialog.set_choice_text_map({});
            dialog.set_choice_to_next_dialog_map({});
            std::string game_result_text;
            if (A_score > B_score) {
                game_result_text = "A wins the game.";
            } else if (A_score < B_score) {
                game_result_text = "B wins the game.";
            } else {
                game_result_text = "Draw the game.";
            }
            dialog.set_text_paragraphs({
                TextParagraph("Game result: " + game_result_text + "Thank you for playing!")
            });
            break;
    }
}

void CardGame::transition_to_next_state() {
    switch (game_state) {
        case A_PREPARE:
            game_state = A_TURN;
            break;
        case A_TURN:
            if (A_card_placing == 0) {
                return;
            }
            if (round_results.size() == round_id + 1) {
                game_state = ROUND_RESULT;
            } else {
                game_state = B_PREPARE;
            }
            break;
        case B_PREPARE:
            game_state = B_TURN;
            break;
        case B_TURN:
            if (B_card_placing == 0) {
                return;
            }
            if (round_results.size() == round_id + 1) {
                game_state = ROUND_RESULT;
            } else {
                game_state = A_PREPARE;
            }
            break;
        case ROUND_RESULT:
            if (round_id == 4) {
                game_state = GAME_RESULT;
                break;
            }
            if (A_first)
                game_state = A_PREPARE;
            else
                game_state = B_PREPARE;

            // if (round_results[round_id] == A_WIN) {
            //     game_state = A_PREPARE;
            // } else if (round_results[round_id] == B_WIN) {
            //     game_state = B_PREPARE;
            // } else {    // Draw
            //     if (A_first)
            //         game_state = B_PREPARE;
            //     else
            //         game_state = A_PREPARE;
            // }
            round_id++;
            break;
        case GAME_RESULT:
            break;
    }
    update_dialog();
}

bool CardGame::A_play_card(uint32_t card_id) {
    if (A_cards_remaining.find(card_id) == A_cards_remaining.end()) {
        return false;
    }
    A_cards_remaining.erase(card_id);
    A_card_placing = card_id;
    return true;
}

bool CardGame::B_play_card(uint32_t card_id) {
    if (B_cards_remaining.find(card_id) == B_cards_remaining.end()) {
        return false;
    }
    B_cards_remaining.erase(card_id);
    B_card_placing = card_id;
    return true;
}

bool CardGame::finish_round() {
    if (A_card_placing == 0 || B_card_placing == 0) {
        return false;
    }
    
    std::string result_string;
    assert(round_results.size() == round_id);
    if (A_card_placing == B_card_placing) {
        result_string = "Draw!";
        round_results.push_back(DRAW);
        A_first = !A_first;
    } else if (A_card_placing > B_card_placing) {
        A_score++;
        result_string = "A wins!";
        round_results.push_back(A_WIN);
        A_first = true;
    } else {
        B_score++;
        result_string = "B wins!";
        round_results.push_back(B_WIN);
        A_first = false;
    }
    return true;
}

void CardGame::handle_choice(uint32_t choice) {
    bool play_card_success;
    switch (game_state) {
        case A_TURN:
            play_card_success = A_play_card(choice);
            if (play_card_success && !A_first)
                finish_round();
            if (play_card_success)
                transition_to_next_state();
            break;
        case B_TURN:
            play_card_success = B_play_card(choice);
            if (play_card_success && A_first)
                finish_round();
            if (play_card_success)
                transition_to_next_state();
            break;
        default:
            if (choice == 1)
                transition_to_next_state();
            break;
    }
}