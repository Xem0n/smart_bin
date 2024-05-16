  // why modulo is not modulo in js?!?!?!?
export default function modulo(num: number, mod: number): number {
  return ((num % mod) + mod) % mod;
}
