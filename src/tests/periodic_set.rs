use crate::periodic_set::PeriodicSet;

#[test]
fn contains() {
    // .xx.(x.x)
    let s = PeriodicSet::new(
        4,
        [1, 2].into_iter().collect(),
        3,
        [0, 2].into_iter().collect(),
    );
    assert_eq!(s.preperiod_len(), 4);
    assert_eq!(s.period_len(), 3);
    assert!((0..10)
        .map(|x| s.contains(x))
        .eq([false, true, true, false, true, false, true, true, false, true,]));
}

#[test]
fn reduce() {
    // .xx.(x.x)
    let s = PeriodicSet::new(
        4,
        [1, 2].into_iter().collect(),
        3,
        [0, 2].into_iter().collect(),
    );

    // .xx.x(.xx.xx)
    let s2 = PeriodicSet::new(
        5,
        [1, 2, 4].into_iter().collect(),
        6,
        [1, 2, 4, 5].into_iter().collect(),
    );

    assert_eq!(s2, s);
}
