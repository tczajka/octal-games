use crate::bitset::Bitset;

#[test]
fn test_bitset() {
    let mut a = Bitset::empty(8);
    assert_eq!(a.first_set(), None);
    a.set(7);
    assert_eq!(a.first_set(), Some(7));
    a.set(8);
    assert_eq!(a.first_set(), Some(7));
    a.clear(7);
    assert_eq!(a.first_set(), Some(8));
    a.clear_all();
    assert_eq!(a.first_set(), None);
    a.set(240);
    assert_eq!(a.first_set(), Some(240));
    a.set_all();
    assert_eq!(a.first_set(), Some(0));
    let mut b = a.clone();
    assert_eq!(a, b);
    b.clear_all();
    assert_ne!(a, b);
    b.clone_from(&a);
    assert_eq!(a, b);
}