pub fn gcd(mut a: u64, mut b: u64) -> u64 {
    if a == 0 && b == 0 {
        panic!("gcd(0, 0)");
    }
    if a == 0 {
        return b;
    }
    if b == 0 {
        return a;
    }
    let az = a.trailing_zeros();
    let bz = b.trailing_zeros();
    a >>= az;
    b >>= bz;
    let shift = az.min(bz);
    while a != b {
        // a, b odd
        if a < b {
            b -= a;
            b >>= b.trailing_zeros();
        } else {
            a -= b;
            a >>= a.trailing_zeros();
        }
    }
    a << shift
}
