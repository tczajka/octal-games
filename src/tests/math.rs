use crate::math::{gcd, lcm};

#[test]
fn test_gcd() {
    assert_eq!(gcd(0, 10), 10);
    assert_eq!(gcd(10, 0), 10);
    assert_eq!(gcd(3, 13), 1);
    assert_eq!(gcd(16, 12), 4);
    assert_eq!(gcd(14 * 8, 21 * 8), 7 * 8);
}

#[test]
fn test_lcm() {
    assert_eq!(lcm(12, 16), 48);
}
