use std::io::{stdin, Read};

#[derive(Clone, Copy)]
struct Rb<const N: usize> {
    pub slice: [u8; N],
    ptr: usize,
}

fn bitcount(mut v: u32) -> u32 {
    v = v - ((v >> 1) & 0x55555555);
    v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
    (((v + (v >> 4)) & 0xF0F0F0F) * 0x1010101) >> 24
}

impl<const N: usize> Rb<N> {
    fn new() -> Self { Rb { slice: [0; N], ptr: 0 } }
    fn push(&mut self, elem: u8) {
        self.slice[self.ptr] = elem;
        self.ptr = (self.ptr + 1) % N;
    }
    fn all_unique(self) -> bool {
        let combined = self.slice
            .iter()
            .map(|b|1u32<<(b - 96u8))
            .reduce(|a, b| a | b)
            .unwrap();
        bitcount(combined) == N.try_into().unwrap()
    }
}

fn main() {
    // let mut buf: [u8; 1] = [0];
    let mut package_rb: Rb<4> = Rb::new();
    let mut message_rb: Rb<14> = Rb::new();
    let mut package_i = None;
    let mut message_i = None;
    let mut i = 0;
    for b in stdin().lock().bytes() {
        if b.is_err() {
            break;
        }
        let c = b.unwrap();
        i += 1;
        
        package_rb.push(c);
        message_rb.push(c);

        if i <= 4 {
            continue;
        }

        if package_rb.all_unique() && package_i.is_none() {
            package_i = Some(i);
        }
        if message_rb.all_unique() && message_i.is_none() {
            message_i = Some(i);
        }
        if package_i.is_some() && message_i.is_some() {
            break;
        }
    }
    println!("part1: {:?}", package_i.unwrap());
    println!("part2: {:?}", message_i.unwrap());
}
