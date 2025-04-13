use std::str::FromStr;

use octal_games::game::Game;

#[test]
fn test_game_names() {
    let names: Vec<(&str, &str)> = vec![
        ("4.0123456u7", "4.0123456u7"),
        ("0.12300", ".123"),
        ("4u.00", "4u"),
        (".00", "0"),
        ("4.(1)", "4.(1)"),
        ("0.(1)", ".(1)"),
        ("4.23120(1)", "4.23120(1)"),
    ];

    for &(input, expected) in &names {
        let Ok(game) = Game::from_str(input) else {
            panic!("Failed to parse {input}");
        };
        let game_str = game.to_string();
        assert_eq!(game_str, expected, "For input {input} expected {expected} got {game_str}");
    }

    let invalid_names: Vec<&str> = vec![
        "",
        ".",
        "0.8",
        "0.3u",
        "44",
        "44.",
        "4.(3)",
    ];

    for &input in &invalid_names {
        assert!(Game::from_str(input).is_err(), "Expected error for input: {input}");
    }
}