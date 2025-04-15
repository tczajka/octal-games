#[derive(Debug, Eq, PartialEq, Hash)]
pub struct Bitset {
    log_len: u8,
    words: Vec<u64>,
}

impl Bitset {
    pub fn empty(log_len: u8) -> Self {
        assert!(log_len >= 6 && log_len <= 16);
        let words = vec![0; 1 << (log_len - 6)];
        Self { log_len, words }
    }

    pub fn full(log_len: u8) -> Self {
        assert!(log_len >= 6 && log_len <= 16);
        let words = vec![u64::MAX; 1 << (log_len - 6)];
        Self { log_len, words }
    }

    pub fn clear_all(&mut self) {
        for word in &mut self.words {
            *word = 0;
        }
    }

    pub fn set_all(&mut self) {
        for word in &mut self.words {
            *word = u64::MAX;
        }
    }

    pub fn clear(&mut self, x: u16) {
        self.words[usize::from(x >> 6)] &= !(1 << (x & 63));
    }

    pub fn set(&mut self, x: u16) {
        self.words[usize::from(x >> 6)] |= 1 << (x & 63);
    }

    pub fn first_set(&self) -> Option<u16> {
        for (word, i) in self.words.iter().copied().zip(0..) {
            if word != 0 {
                let j = u16::try_from(word.trailing_zeros()).unwrap();
                return Some(i << 6 | j);
            }
        }
        None
    }
}

impl Clone for Bitset {
    fn clone(&self) -> Self {
        Self {
            log_len: self.log_len,
            words: self.words.clone(),
        }
    }

    fn clone_from(&mut self, source: &Self) {
        self.log_len = source.log_len;
        self.words.clone_from(&source.words);
    }
}