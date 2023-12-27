use crate::small_set::SmallSet;

#[test]
fn iterators() {
    let mut s: SmallSet = [1, 2, 5].into_iter().collect();
    s.insert(6);
    s.remove(5);
    assert!(s.contains(6));
    assert!(!s.contains(5));
    assert!(s.into_iter().eq([1, 2, 6]));
}

#[test]
fn bitops() {
    let a: SmallSet = [1, 2, 5].into_iter().collect();
    let b: SmallSet = [2, 5, 7].into_iter().collect();
    let a_and_b: SmallSet = [2, 5].into_iter().collect();
    let a_or_b: SmallSet = [1, 2, 5, 7].into_iter().collect();
    let a_and_not_b: SmallSet = [1].into_iter().collect();

    assert_eq!(a & b, a_and_b);
    let mut x = a;
    x &= b;
    assert_eq!(x, a_and_b);

    assert_eq!(a | b, a_or_b);
    let mut x = a;
    x |= b;
    assert_eq!(x, a_or_b);

    assert_eq!(a.and_not(b), a_and_not_b);
}
