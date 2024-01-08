use crate::small_set::SmallSet;

#[derive(Copy, Clone, Debug, Eq, PartialEq)]
pub struct PeriodicSet {
    preperiod_len: u64,
    preperiod: SmallSet,
    period_len: u64,
    period: SmallSet,
}

impl PeriodicSet {
    pub fn preperiod_len(&self) -> u64 {
        self.preperiod_len
    }

    pub fn period_len(&self) -> u64 {
        self.period_len
    }

    pub fn contains(&self, x: u64) -> bool {
        if x < self.preperiod_len {
            self.preperiod.contains(x)
        } else {
            self.period
                .contains((x - self.preperiod_len) % self.period_len)
        }
    }

    pub fn new(
        preperiod_len: u64,
        preperiod: SmallSet,
        mut period_len: u64,
        mut period: SmallSet,
    ) -> Self {
        assert!(preperiod
            .and_not(SmallSet::all_below(preperiod_len))
            .is_empty());
        assert!(period.and_not(SmallSet::all_below(period_len)).is_empty());

        for new_period_len in 1..period_len {
            if period_len % new_period_len == 0
                && period.and_not(SmallSet::all_below(new_period_len)) >> new_period_len
                    == period & SmallSet::all_below(period_len - new_period_len)
            {
                period_len = new_period_len;
                period &= SmallSet::all_below(new_period_len);
                break;
            }
        }

        Self::with_minimum_period(preperiod_len, preperiod, period_len, period)
    }

    fn with_minimum_period(
        mut preperiod_len: u64,
        mut preperiod: SmallSet,
        period_len: u64,
        mut period: SmallSet,
    ) -> Self {
        while preperiod_len > 0
            && preperiod.contains(preperiod_len - 1) == period.contains(period_len - 1)
        {
            preperiod_len -= 1;
            preperiod &= SmallSet::all_below(preperiod_len);
            period = (period & SmallSet::all_below(period_len - 1)) << 1
                | (period & SmallSet::single(period_len - 1)) >> (period_len - 1);
        }
        Self {
            preperiod_len,
            preperiod,
            period_len,
            period,
        }
    }
}
