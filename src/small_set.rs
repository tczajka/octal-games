use std::ops::{BitAnd, BitAndAssign, BitOr, BitOrAssign};

#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub struct SmallSet(u64);

impl SmallSet {
    const EMPTY: Self = SmallSet(0);

    pub fn single(x: u64) -> Self {
        assert!(x < 64);
        SmallSet(1 << x)
    }

    pub fn contains(self, x: u64) -> bool {
        self & Self::single(x) != Self::EMPTY
    }

    pub fn insert(&mut self, x: u64) {
        *self |= Self::single(x)
    }

    pub fn remove(&mut self, x: u64) {
        *self = self.and_not(Self::single(x))
    }

    pub fn and_not(self, other: Self) -> Self {
        Self(self.0 & !other.0)
    }

    pub fn smallest(self) -> Option<u64> {
        if self.0 == 0 {
            None
        } else {
            Some(self.0.trailing_zeros().into())
        }
    }
}

impl BitAnd for SmallSet {
    type Output = Self;

    fn bitand(self, rhs: Self) -> Self {
        Self(self.0 & rhs.0)
    }
}

impl BitAndAssign for SmallSet {
    fn bitand_assign(&mut self, rhs: Self) {
        *self = *self & rhs;
    }
}

impl BitOr for SmallSet {
    type Output = Self;

    fn bitor(self, rhs: Self) -> Self {
        Self(self.0 | rhs.0)
    }
}

impl BitOrAssign for SmallSet {
    fn bitor_assign(&mut self, rhs: Self) {
        *self = *self | rhs;
    }
}

impl FromIterator<u64> for SmallSet {
    fn from_iter<T: IntoIterator<Item = u64>>(iter: T) -> Self {
        let mut res = Self::EMPTY;
        for x in iter {
            res.insert(x);
        }
        res
    }
}

impl IntoIterator for SmallSet {
    type Item = u64;
    type IntoIter = SmallSetIterator;

    fn into_iter(self) -> SmallSetIterator {
        SmallSetIterator(self)
    }
}

#[derive(Debug)]
pub struct SmallSetIterator(SmallSet);

impl Iterator for SmallSetIterator {
    type Item = u64;

    fn next(&mut self) -> Option<u64> {
        let x = self.0.smallest()?;
        self.0.remove(x);
        Some(x)
    }
}
