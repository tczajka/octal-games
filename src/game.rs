use std::{error::Error, fmt::{self, Display, Formatter}, str::FromStr};

/// An octal game.
#[derive(Clone)]
pub struct Game {
    /// Whole heaps that can be eliminated.
    /// x -> 0
    /// Sorted.
    eliminations: Vec<u64>,

    /// Everything >= `eliminations_ge` can be eliminated.
    /// x -> 0
    eliminations_ge: Option<u64>,

    /// Heap reductions allowed, but not to 0.
    /// a -> a - x
    /// Sorted.
    reductions: Vec<u64>,

    /// Heap splits allowed, but not to 0.
    /// a -> b, c
    /// If a = b + c + x
    /// Sorted.
    splits: Vec<u64>,

    /// Unequal splits allowed, but not to 0.
    /// a -> b, c
    /// If a = b + c + x, b != c
    /// Sorted.
    unequal_splits: Vec<u64>,
}

impl Display for Game {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        let mut eliminations_iter = self.eliminations.iter().copied().peekable();
        let mut reductions_iter = self.reductions.iter().copied().peekable();
        let mut splits_iter = self.splits.iter().copied().peekable();
        let mut unequal_splits_iter = self.unequal_splits.iter().copied().peekable();

        for x in 0u64.. {
            let mut digit: u8 = 0;
            let mut split_unequal = false;
            let mut something_left = false;

            if let Some(&y) = eliminations_iter.peek() {
                something_left = true;
                if y == x {
                    digit |= 1;
                    eliminations_iter.next();
                }
            }

            if let Some(&y) = reductions_iter.peek() {
                something_left = true;
                if y == x {
                    digit |= 2;
                    reductions_iter.next();
                }
            }

            if let Some(&y) = unequal_splits_iter.peek() {
                something_left = true;
                if y == x {
                    digit |= 4;
                    split_unequal = true;
                    unequal_splits_iter.next();
                }
            }

            if let Some(&y) = splits_iter.peek() {
                something_left = true;
                if y == x {
                    digit |= 4;
                    split_unequal = false;
                    splits_iter.next();
                }
            }

            if let Some(from) = self.eliminations_ge {
                if x >= from {
                    digit |= 1;
                } else {
                    something_left = true;
                }
            }
            let sep = if x == 1 { "." } else { "" };
            if !something_left {
                match (x, digit) {
                    (0, 0) => write!(f, "0")?,
                    (0, 1) => write!(f, "1.(1)")?,
                    (_, 0) => {}
                    (_, 1) => write!(f, "{sep}(1)")?,
                    _ => unreachable!(),
                }
                break;
            }
            if x != 0 || digit != 0 {
                write!(f, "{sep}{digit}")?;
                if split_unequal {
                    write!(f, "u")?;
                }
            }
        }
        Ok(())
    }
}

impl FromStr for Game {
    type Err = GameParseError;

    fn from_str(s: &str) -> Result<Self, GameParseError> {
        let mut game = Game {
            eliminations: vec![],
            eliminations_ge: None,
            reductions: vec![],
            splits: vec![],
            unequal_splits: vec![],
        };

        let mut chars = s.chars().peekable();
        let mut x = 0u64;
        let mut seen_period = false;
        let mut seen_digit = false;

        while let Some(c) = chars.next() {
            match c {
                '.' => {
                    if seen_period || x > 1 {
                        return Err(GameParseError);
                    }
                    seen_period = true;
                    x = 1;
                }
                '0'..='7' => {
                    if x >= 1 && !seen_period {
                        return Err(GameParseError);
                    }
                    let digit = c.to_digit(8).unwrap();
                    if digit & 1 != 0 {
                        if x == 0 {
                            return Err(GameParseError);
                        }
                        game.eliminations.push(x);
                    }
                    if digit & 2 != 0 {
                        if x == 0 {
                            return Err(GameParseError);
                        }
                        game.reductions.push(x);
                    }
                    if digit & 4 != 0 {
                        if Some(&'u') == chars.peek() {
                            chars.next();
                            game.unequal_splits.push(x);
                        } else {
                            game.splits.push(x);
                        }
                    }
                    x += 1;
                    seen_digit = true;
                }
                '(' => {
                    if !seen_period {
                        return Err(GameParseError);
                    }
                    if chars.next() != Some('1') ||
                        chars.next() != Some(')') ||
                        chars.next() != None {
                        return Err(GameParseError);
                    }

                    game.eliminations_ge = Some(x);
                    seen_digit = true;
                    break;
                 }
                _ => return Err(GameParseError),
            }
        }

        if !seen_digit {
            return Err(GameParseError);
        }

        Ok(game)
    }
}

#[derive(Clone, Debug)]
pub struct GameParseError;

impl Display for GameParseError {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        write!(f, "GameParseError")
    }
}

impl Error for GameParseError {}