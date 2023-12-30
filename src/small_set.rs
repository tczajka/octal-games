use std::ops::{BitAnd, BitAndAssign, BitOr, BitOrAssign, Shl, ShlAssign, Shr, ShrAssign};

const BITS: u64 = 64;
type Repr = u64;

#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub struct SmallSet(Repr);

impl SmallSet {
    pub const EMPTY: Self = SmallSet(0);

    pub fn is_empty(self) -> bool {
        self == Self::EMPTY
    }

    pub fn single(x: u64) -> Self {
        assert!(x < BITS);
        SmallSet(1 << x)
    }

    pub fn all_below(x: u64) -> Self {
        assert!(x <= BITS);
        if x == BITS {
            Self(Repr::MAX)
        } else {
            Self((1 << x) - 1)
        }
    }

    pub fn contains(self, x: u64) -> bool {
        !(self & Self::single(x)).is_empty()
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
        if self.is_empty() {
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

impl Shl<u64> for SmallSet {
    type Output = Self;

    fn shl(self, x: u64) -> Self {
        if self.is_empty() {
            return Self::EMPTY;
        }
        assert!(x < BITS);
        assert!(self.and_not(Self::all_below(BITS - x)).is_empty());
        Self(self.0 << x)
    }
}

impl ShlAssign<u64> for SmallSet {
    fn shl_assign(&mut self, x: u64) {
        *self = *self << x;
    }
}

impl ShrAssign<u64> for SmallSet {
    fn shr_assign(&mut self, x: u64) {
        *self = *self >> x;
    }
}

impl Shr<u64> for SmallSet {
    type Output = Self;

    fn shr(self, x: u64) -> Self {
        if self.is_empty() {
            return Self::EMPTY;
        }
        assert!(x < BITS);
        assert!((self & Self::all_below(x)).is_empty());
        Self(self.0 >> x)
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
